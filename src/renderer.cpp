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
    std::thread loader(&Renderer::load_data, this);

    processor.join();
    loader.join();
}

template <typename T>
void Renderer<T>::add_data(T *input)
{
    {
        std::lock_guard<std::mutex> lock(buffer_mutex_);
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
void RandomRenderer<T>::load_data()
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

template class Renderer<float>;
template class Renderer<double>;

template class RandomRenderer<float>;
template class RandomRenderer<double>;
