#include "renderer.h"
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <atomic>
#include <chrono>

template <typename T>
void Renderer<T>::render()
{
    std::cout << "Render start" << std::endl;
    std::thread processor(&Renderer::process_data, this);
    std::thread loader(&Renderer::load_input, this);

    processor.join();
    loader.join();
}

template <typename T>
void Renderer<T>::add_data(T *input)
{
    {
        std::lock_guard<std::mutex>
            lock(buffer_mutex_);
        buffer_.emplace(input);
    }
    buffer_cv_.notify_one();
}

template <typename T>
void Renderer<T>::process_data()
{
    while (!stop_flag_.load())
    {
        std::unique_lock<std::mutex> lock(buffer_mutex_);
        buffer_cv_.wait(lock, [this]
                        { return !buffer_.empty() || stop_flag_.load(); });

        while (!buffer_.empty())
        {
            auto input = buffer_.front();
            buffer_.pop();
            lock.unlock();

            T *output = (T *)malloc(solver_.output_size() * sizeof(T));
            solver_.process(input, output);

            if (output_callback_)
            {
                output_callback_(output);
            }
            {
                std::lock_guard<std::mutex> output_lock(output_mutex_);
                processed_outputs_.push(output);
                output_cv_.notify_one();
            }
            free(input);
            // free(output);

            std::cout << "..." << std::endl;

            lock.lock();
        }
    }
}

template <typename T>
void Renderer<T>::register_output_callback(std::function<void(T *)> callback)
{
    output_callback_ = callback;
}

template class Renderer<float>;
template class Renderer<double>;

/* RNG-Input Renderer */

#include <random>

template <typename T>
RandomRenderer<T>::RandomRenderer(Solver<T> &solver, double run_duration) : Renderer<T>(solver), run_duration_(std::chrono::duration<double>(run_duration)) {}

template <typename T>
T *RandomRenderer<T>::get_output()
{
    if (this->processed_outputs_.empty())
    {
        return nullptr;
    }
    auto output = this->processed_outputs_.front();
    std::cout << this->processed_outputs_.size() << std::endl;
    this->processed_outputs_.pop();
    return output; // Return the raw pointer
}

template <typename T>
void RandomRenderer<T>::load_input()
{
    std::seed_seq seed{42};
    std::mt19937 gen(seed);
    std::uniform_real_distribution<> dis(0.0, 1.0);
    std::uniform_int_distribution<> disint(100, 1000);

    auto start_time = std::chrono::high_resolution_clock::now();
    while (!this->stop_flag_.load())
    {
        T *input = (T *)malloc(this->solver_.input_size() * sizeof(T));
        for (int j = 0; j < this->solver_.input_size(); ++j)
        {
            input[j] = dis(gen);
        }
        this->add_data(input);
        std::this_thread::sleep_for(std::chrono::milliseconds(disint(gen)));
        if (this->run_duration_.count() > 0)
        {
            auto current_time = std::chrono::high_resolution_clock::now();
            if (std::chrono::duration_cast<std::chrono::duration<double>>(current_time - start_time) >= this->run_duration_)
            {
                this->stop_flag_.store(true);
                this->buffer_cv_.notify_all(); // Ensure to wake up the processor thread
                break;
            }
        }
    }
}

template class RandomRenderer<float>;
template class RandomRenderer<double>;

/* Jack Renderer */

template <typename T>
JackRenderer<T>::JackRenderer(Solver<T> &solver, int n_channels) : Renderer<T>(solver), n_channels_(n_channels)
{
    client_ = jack_client_open("State Space System", JackNullOption, &status_, NULL);

    // allocate array
    input_port_ = new jack_port_t *[n_channels_];
    for (int i = 0; i < n_channels_; i++)
    {
        std::string tmp_str = "input_" + std::to_string(i + 1);
        input_port_[i] = jack_port_register(client_, tmp_str.c_str(),
                                            JACK_DEFAULT_AUDIO_TYPE,
                                            JackPortIsInput, 0);
    }

    output_port_ = new jack_port_t *[n_channels_];
    for (int i = 0; i < n_channels_; i++)
    {
        std::string tmp_str = "output_" + std::to_string(i + 1);
        output_port_[i] = jack_port_register(client_, tmp_str.c_str(),
                                             JACK_DEFAULT_AUDIO_TYPE,
                                             JackPortIsOutput, 0);
    }

    out_ = new jack_default_audio_sample_t *;
    in_ = new jack_default_audio_sample_t *;
}

template <typename T>
void JackRenderer<T>::load_input()
{
    // Set up the callback function
    jack_set_process_callback(client_, [](jack_nframes_t nframes, void *arg) -> int
                              { return static_cast<JackRenderer<T> *>(arg)->process_jack_buffer(nframes); }, this);

    // Activate the JACK client
    if (jack_activate(client_))
    {
        std::cerr << "Cannot activate JACK client" << std::endl;
        return;
    }

    // Connect ports (optional, if needed)
    const char **ports;
    if ((ports = jack_get_ports(client_, nullptr, nullptr, JackPortIsPhysical | JackPortIsOutput)) == nullptr)
    {
        std::cerr << "Cannot find any physical capture ports" << std::endl;
    }
    else
    {
        for (int i = 0; i < n_channels_ && ports[i] != nullptr; ++i)
        {
            if (jack_connect(client_, ports[i], jack_port_name(input_port_[i])))
            {
                std::cerr << "Cannot connect input ports" << std::endl;
            }
        }
        jack_free(ports);
    }

    // Keep the thread alive to maintain the JACK connection
    while (!this->stop_flag_.load())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    // Deactivate the JACK client on stopping
    jack_deactivate(client_);
    jack_client_close(client_);
}

template <typename T>
int JackRenderer<T>::process_jack_buffer(jack_nframes_t nframes)
{
    for (int i = 0; i < n_channels_; ++i)
    {
        in_[i] = static_cast<jack_default_audio_sample_t *>(jack_port_get_buffer(input_port_[i], nframes));
    }

    // Assuming that T is of type float or double for audio samples
    T *input = static_cast<T *>(malloc(n_channels_ * nframes * sizeof(T)));
    for (int i = 0; i < nframes; ++i)
    {
        for (int j = 0; j < n_channels_; ++j)
        {
            input[i * n_channels_ + j] = static_cast<T>(in_[j][i]);
        }
    }

    this->add_data(input);
    return 0; // Indicate successful processing
}

template <typename T>
T *JackRenderer<T>::get_output()
{
    if (this->processed_outputs_.empty())
    {
        return nullptr;
    }
    auto output = this->processed_outputs_.front();
    std::cout << this->processed_outputs_.size() << std::endl;
    this->processed_outputs_.pop();

    jack_nframes_t nframes = jack_get_buffer_size(client_);

    for (int i = 0; i < n_channels_; ++i)
    {
        out_[i] = static_cast<jack_default_audio_sample_t *>(jack_port_get_buffer(output_port_[i], nframes));
    }

    for (int i = 0; i < nframes; ++i)
    {
        for (int j = 0; j < n_channels_; ++j)
        {
            out_[j][i] = static_cast<jack_default_audio_sample_t>(output[i * n_channels_ + j]);
        }
    }

    return output; // Return the raw pointer
}

template class JackRenderer<float>;
template class JackRenderer<double>;
