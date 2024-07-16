#include "renderer.h"
#include <iostream>
#include <jack/types.h>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <atomic>
#include <chrono>

/* Jack Renderer */

template <typename T>
JackRenderer<T>::JackRenderer(Solver<T> &solver, int n_inputs, int n_outputs) : Renderer<T>(solver), n_inputs_(n_inputs), n_outputs_(n_outputs)
{
    this->client_ = jack_client_open("State Space System", JackNullOption, &status_, NULL);

    // connect the callback function
    jack_set_process_callback(this->client_, this->callback_process, this);

    // allocate array
    input_port_ = new jack_port_t *[n_inputs_];
    for (int i = 0; i < n_inputs_; i++)
    {
        std::string tmp_str = "input_" + std::to_string(i + 1);
        input_port_[i] = jack_port_register(client_, tmp_str.c_str(),
                                            JACK_DEFAULT_AUDIO_TYPE,
                                            JackPortIsInput, 0);
    }

    output_port_ = new jack_port_t *[n_outputs_];
    for (int i = 0; i < n_outputs_; i++)
    {
        std::string tmp_str = "output_" + std::to_string(i + 1);
        output_port_[i] = jack_port_register(client_, tmp_str.c_str(),
                                             JACK_DEFAULT_AUDIO_TYPE,
                                             JackPortIsOutput, 0);
    }
    jack_nframes_t buffer_size = jack_get_buffer_size(this->client_);

    this->solver_.set_buffer_size(buffer_size);

    const int max_buffer_size = 2048;
    u = (T *)malloc(n_inputs * max_buffer_size * sizeof(T));
    y = (T *)malloc(n_outputs * max_buffer_size * sizeof(T));

    jack_set_buffer_size_callback(this->client_, this->callback_buffer_size, this);
}

template <typename T>
void JackRenderer<T>::render()
{
    out_ = new jack_default_audio_sample_t *;
    in_ = new jack_default_audio_sample_t *;

    jack_activate(this->client_);

    sleep(-1);
}

template <typename T>
int JackRenderer<T>::callback_process(jack_nframes_t x, void *object)
{
    return static_cast<JackRenderer *>(object)->process(x);
}

template <typename T>
int JackRenderer<T>::callback_buffer_size(jack_nframes_t x, void *object)
{
    std::cout << x << " | "  << std::endl;
    return static_cast<JackRenderer *>(object)->solver_.set_buffer_size(x);
}

template <typename T>
int JackRenderer<T>::process(jack_nframes_t nframes)
{

    // get input buffers
    for (int i = 0; i < n_inputs_; i++)
    {
        in_[i] = (jack_default_audio_sample_t *)
            jack_port_get_buffer(this->input_port_[i], jack_get_buffer_size(client_));
    }

    for (int i = 0; i < n_outputs_; i++)
    {
        out_[i] = (jack_default_audio_sample_t *)
            jack_port_get_buffer(this->output_port_[i], jack_get_buffer_size(client_));
    }

    for (int i = 0; i < nframes; i++)
    {
        for (int j = 0; j < n_inputs_; j++)
        {
            u[j + i * n_inputs_] = in_[j][i];
        }
    }

    this->solver_.process(u, y);

    for (int i = 0; i < nframes; i++)
    {
        for (int j = 0; j < n_outputs_; j++)
        {
            out_[j][i] = y[j + i * n_outputs_];
        }
    }

    return 0;
}

template class JackRenderer<float>;
template class JackRenderer<double>;
