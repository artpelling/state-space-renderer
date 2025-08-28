#include "renderer.h"
#include <iostream>
#include <jack/jack.h>
#include <spa/param/format.h>       // defines the media‐type ids (e.g. “audio”)
#include <spa/param/format-utils.h> // helper macros around those
#include <spa/pod/builder.h>        // for spa_pod_builder
#include <spa/param/audio/raw.h>
#include <spa/param/audio/raw-utils.h>

/* Pipewire Renderer */

template <typename T>
PipewireRenderer<T>::PipewireRenderer(Solver<T> &solver, int n_inputs, int n_outputs)
    : Renderer<T>(solver), n_inputs_(n_inputs), n_outputs_(n_outputs)
{
    std::cerr << "PipewireRenderer ctor start\n";
    std::cerr << "About to call pw_init()\n";
    pw_init(nullptr, nullptr); // Initialize PipeWire system

    loop_ = pw_main_loop_new(nullptr);                                   // Create PipeWire main loop
    context_ = pw_context_new(pw_main_loop_get_loop(loop_), nullptr, 0); // Create context
    core_ = pw_context_connect(context_, nullptr, 0);                    // Connect to the PipeWire core

    // Register callback for audio processing
    struct pw_stream_events stream_events = {};
    stream_events.version = PW_VERSION_STREAM_EVENTS;
    stream_events.process = &PipewireRenderer<T>::callback_process;
    stream_ = pw_stream_new_simple(
        pw_main_loop_get_loop(loop_),
        "State Space Renderer",
        nullptr, // props
        &stream_events,
        this // callback data
    );

    spa_pod_builder b = SPA_POD_BUILDER_INIT(params_buffer_, sizeof(params_buffer_));
    const struct spa_pod *params[1];
    // initialize raw audio info (note the fields: format, channels, rate)
    struct spa_audio_info_raw info = SPA_AUDIO_INFO_RAW_INIT(
        SPA_AUDIO_FORMAT_F32,
        n_inputs_,
        48000);

    params[0] = spa_format_audio_raw_build(&b,
                                           SPA_PARAM_EnumFormat,
                                           &info);

    pw_stream_flags flags = static_cast<pw_stream_flags>(
        PW_STREAM_FLAG_AUTOCONNECT |
        PW_STREAM_FLAG_MAP_BUFFERS);

    int res = pw_stream_connect(stream_, PW_DIRECTION_INPUT, PW_ID_ANY, flags, params, 1);
    if (res < 0)
    {
        int err = res;
        fprintf(stderr, "pw_stream_connect failed: %d (%s)\n", err, pw_stream_state_as_string(pw_stream_get_state(stream_, nullptr)));
        exit(1);
    }

    // Allocate audio buffers
    this->solver_.set_buffer_size(2048);
    u = new T[n_inputs * 2048];
    y = new T[n_outputs * 2048];
    std::cerr << "PipewireRenderer ctor end\n";
}

template <typename T>
PipewireRenderer<T>::~PipewireRenderer()
{
    delete[] u;
    delete[] y;
    pw_stream_destroy(stream_);
    pw_context_destroy(context_);
    pw_main_loop_destroy(loop_);
}

template <typename T>
void PipewireRenderer<T>::callback_process(void *data)
{
    auto *self = static_cast<PipewireRenderer<T> *>(data);

    struct pw_buffer *buffer = pw_stream_dequeue_buffer(self->stream_);
    if (!buffer || !buffer->buffer)
        return;

    const spa_data *data_buf = &buffer->buffer->datas[0];

    // Total number of bytes of audio
    uint32_t size_in_bytes = data_buf->chunk->size;

    // Sample format: assume 32-bit float (4 bytes)
    uint32_t bytes_per_sample = sizeof(T);

    // Number of channels — you should store this when you negotiate the format
    uint32_t n_channels = self->n_inputs_; // or n_outputs_, depending on stream direction

    // Calculate number of frames:
    uint32_t nframes = size_in_bytes / (bytes_per_sample * n_channels);

    // Example: Assume interleaved float buffers (real code should check format!)
    T *in = (T *)buffer->buffer->datas[0].data;
    T *out = (T *)buffer->buffer->datas[1].data;

    // Fill input array for solver
    for (int i = 0; i < nframes; ++i)
        for (int j = 0; j < self->n_inputs_; ++j)
            self->u[j + i * self->n_inputs_] = in[i * self->n_inputs_ + j];

    // Process using solver
    self->solver_.process(self->u, self->y);

    // Write output back to buffer
    for (int i = 0; i < nframes; ++i)
        for (int j = 0; j < self->n_outputs_; ++j)
            out[i * self->n_outputs_ + j] = self->y[j + i * self->n_outputs_];

    pw_stream_queue_buffer(self->stream_, buffer); // Re-queue the buffer
}

template <typename T>
void PipewireRenderer<T>::render()
{
    const char *state_str = pw_stream_state_as_string(pw_stream_get_state(stream_, nullptr));
    fprintf(stderr, "Stream state before loop: %s\n", state_str);
    pw_main_loop_run(loop_); // Start PipeWire loop — this blocks the thread
    fprintf(stderr, "Stream state after loop: %s\n", state_str);
}

template class PipewireRenderer<float>;
template class PipewireRenderer<double>;

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
}

template <typename T>
int JackRenderer<T>::callback_process(jack_nframes_t x, void *object)
{
    return static_cast<JackRenderer *>(object)->process(x);
}

template <typename T>
int JackRenderer<T>::callback_buffer_size(jack_nframes_t x, void *object)
{
    std::cout << x << " | " << std::endl;
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
