#include "renderer.h"
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <atomic>
#include <chrono>

/* Jack Renderer */

template <typename T>
JackRenderer<T>::JackRenderer(Solver<T> &solver, int n_channels) : Renderer<T>(solver), n_channels_(n_channels)
{
    this->client_ = jack_client_open("State Space System", JackNullOption, &status_, NULL);

    // connect the callback function
    jack_set_process_callback(this->client_, this->callback_process, this);

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

    u = (T *)malloc(this->solver_.input_size() * sizeof(T));
    y = (T *)malloc(this->solver_.output_size() * sizeof(T));
}

template <typename T>
void JackRenderer<T>::render()
{
    out_ = new jack_default_audio_sample_t *;
    in_ = new jack_default_audio_sample_t *;

    jack_activate(this->client_);

    // connect inputs
    for (int i = 0; i < n_channels_; i++)
    {
        std::string tmp_str = "system:capture_" + std::to_string(i + 1);
        jack_connect(client_, tmp_str.c_str(), jack_port_name(input_port_[i]));
    }
    // connect outputs
    for (int i = 0; i < n_channels_; i++)
    {
        std::string tmp_str = "system:playback_" + std::to_string(i + 1);
        jack_connect(client_, jack_port_name(output_port_[i]), tmp_str.c_str());
    }

    // run forever
    sleep(-1);
}

template <typename T>
int JackRenderer<T>::callback_process(jack_nframes_t x, void *object)
{
    return static_cast<JackRenderer *>(object)->process(x);
}

template <typename T>
int JackRenderer<T>::process(jack_nframes_t nframes)
{

    // get input buffers
    for (int i = 0; i < n_channels_; i++)
    {
        out_[i] = (jack_default_audio_sample_t *)
            jack_port_get_buffer(this->output_port_[i], jack_get_buffer_size(client_));
        in_[i] = (jack_default_audio_sample_t *)
            jack_port_get_buffer(this->input_port_[i], jack_get_buffer_size(client_));
    }

    for (int i = 0; i < nframes; i++)
    {
        for (int j = 0; j < n_channels_; j++)
        {
            u[j + i * n_channels_] = in_[j][i];
        }
    }

    this->solver_.process(u, y);

    for (int i = 0; i < nframes; i++)
    {
        for (int j = 0; j < n_channels_; j++)
        {
            out_[j][i] = y[j + i * n_channels_];
        }
    }

    return 0;
}

template class JackRenderer<float>;
template class JackRenderer<double>;
