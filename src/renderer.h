#ifndef RENDERER_H_
#define RENDERER_H_

#include "solver.h"
#include <queue>
#include <thread>             // For std::thread
#include <mutex>              // For std::mutex, std::lock_guard, std::unique_lock
#include <condition_variable> // For std::condition_variable
#include <atomic>             // For std::atomic
#include <functional>

template <typename T>
class Renderer
{
protected:
    Solver<T> &solver_;

    std::atomic<bool> stop_flag_{false};

    std::queue<T *> buffer_;
    std::mutex buffer_mutex_;
    std::condition_variable buffer_cv_;

    std::queue<T *> processed_outputs_; // Queue to store processed outputs as raw pointers
    std::mutex output_mutex_;
    std::condition_variable output_cv_;

    virtual void load_data() = 0;
    void process_data();
    void add_data(T *input);

    std::function<void(T *)> output_callback_;

public:
    Renderer(Solver<T> &solver) : solver_(solver){};
    void render();
    virtual T *get_output() = 0;

    void register_output_callback(std::function<void(T *)> callback);
};

/// @brief Random Renderer.
template <typename T>
class RandomRenderer : public Renderer<T>
{
private:
    void load_data();
    std::chrono::duration<double> run_duration_;

public:
    RandomRenderer(Solver<T> &solver, double run_duration = 0);
    T *get_output();
};

#endif