#include "renderer.h"
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <atomic>
#include <chrono>

template <typename T>
void Renderer<T>::render(T *output)
{
    std::thread processor(&Renderer::process_data, output, this);

    load_data();

    stop_flag_.store(true);
    buffer_cv_.notify_all();

    processor.join();
}

template <typename T>
void Renderer<T>::add_data(const T *input)
{
    {
        std::lock_guard<std::mutex> lock(buffer_mutex_);
        buffer_.emplace_back(input);
    }
    buffer_cv_.notify_one();
}

template <typename T>
void Renderer<T>::process_data(T *output)
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

            T *output(data.size());
            solver_.process(input, output);

            // Print processed data to the console immediately
            std::cout << "Processed data.";
            std::cout << std::endl;

            lock.lock();
        }
    }
}
