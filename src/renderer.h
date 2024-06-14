#ifndef RENDERER_H_
#define RENDERER_H_

#include "solver.h"
#include <queue>
#include <thread>             // For std::thread
#include <mutex>              // For std::mutex, std::lock_guard, std::unique_lock
#include <condition_variable> // For std::condition_variable
#include <atomic>             // For std::atomic

template <typename T>
class Renderer
{
protected:
    Solver<T> &solver_;
    std::queue<T *> buffer_;
    std::mutex buffer_mutex_;
    std::condition_variable buffer_cv_;
    std::atomic<bool> stop_flag_{false};

    virtual void load_data() = 0;
    void process_data(T *output);
    void add_data(const T *input);

public:
    Renderer(Solver<T> &solver) : solver_(solver){};
    ~Renderer();
    void render(T *output);
};

/// @brief Random Renderer.
template <typename T>
class RandomRenderer : public Renderer<T>
{
public:
    RandomRenderer(Solver<T> &solver);
    ~RandomRenderer();
    void feed_data();
};

#endif