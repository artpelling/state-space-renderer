#ifndef RENDERER_H_
#define RENDERER_H_

#include "solver.h"

/// @brief Base renderer class
/// @tparam T Typename of class (double/float)
template <typename T>
class Renderer
{
protected:
    Solver<T> &solver_;

public:
    /// @brief Initialization with a solver.
    /// @param solver Solver of the renderer.
    Renderer(Solver<T> &solver) : solver_(solver) {};
    virtual void render() = 0;
};

#include <jack/jack.h>

/// @brief Jack Renderer.
/// @tparam T Typename of class (double/float)
template <typename T>
class JackRenderer : public Renderer<T>
{
private:
    /// Variables
    T *u, *y;

    /// @brief number of channels
    int n_inputs_;
    int n_outputs_;

    /// @brief the jack client
    jack_client_t *client_;
    /// @brief gets the status from the jack server
    jack_status_t status_;
    /// @brief the jack input ports
    jack_port_t **input_port_;
    /// @brief the jack output ports
    jack_port_t **output_port_;

    ///
    /// \brief in
    jack_default_audio_sample_t **in_;
    ///
    /// \brief out
    jack_default_audio_sample_t **out_;

    ///
    /// \brief Process step in Jack format. Wraps up data of the actual process for Jack.
    /// \param nframes
    /// \return
    ///
    int process(jack_nframes_t nframes);

    ///
    /// \brief callback_process
    ///         is used to access the members of this
    ///         class in the static mode
    /// \param x number of samples in the buffer
    /// \param object void pointer
    /// \return
    ///
    static int callback_process(jack_nframes_t x, void *object);
    static int callback_buffer_size(jack_nframes_t x, void *object);

public:
    /// @brief Initialization with a solver.
    /// @param solver Solver of the Renderer.
    /// @param n_inputs Number of input channels (m).
    /// @param n_outputs Number of output channels (p).
    JackRenderer(Solver<T> &solver, int n_inputs, int n_outputs);
    /// @brief Runs the renderer.
    void render();
};

#endif
