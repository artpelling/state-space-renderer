#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <atomic>
#include <chrono>

class Solver
{
public:
    void process(double *input, double *output, size_t size)
    {
        // Example process: Copy input to output
        for (size_t i = 0; i < size; ++i)
        {
            output[i] = input[i];
        }
    }
};

class Renderer
{
public:
    Renderer() : solver(new Solver) {}
    ~Renderer() { delete solver; }

    void render()
    {
        std::thread processor(&Renderer::process_data, this);

        // Simulate adding data to the buffer
        for (int i = 0; i < 10; ++i)
        {
            double input[] = {static_cast<double>(i), static_cast<double>(i * 2), static_cast<double>(i * 3)};
            add_data(input, 3);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }

        // Signal the processing thread to stop
        stop_flag.store(true);
        buffer_cv.notify_all();

        // Wait for the processing thread to finish
        processor.join();
    }

    std::vector<std::vector<double>> get_processed_outputs() const
    {
        std::lock_guard<std::mutex> lock(output_mutex);
        return processed_outputs;
    }

private:
    Solver *solver;
    std::queue<std::vector<double>> buffer;
    std::mutex buffer_mutex;
    std::condition_variable buffer_cv;
    std::atomic<bool> stop_flag{false};

    std::vector<std::vector<double>> processed_outputs;
    mutable std::mutex output_mutex;

    void process_data()
    {
        while (!stop_flag.load())
        {
            std::unique_lock<std::mutex> lock(buffer_mutex);
            buffer_cv.wait(lock, [this]
                           { return !buffer.empty() || stop_flag.load(); });

            while (!buffer.empty())
            {
                auto data = buffer.front();
                buffer.pop();
                lock.unlock();

                std::vector<double> output(data.size());
                solver->process(data.data(), output.data(), data.size());

                {
                    std::lock_guard<std::mutex> output_lock(output_mutex);
                    processed_outputs.push_back(output);
                }

                // Print processed data to the console immediately
                std::cout << "Processed data: ";
                for (const auto &val : output)
                {
                    std::cout << val << " ";
                }
                std::cout << std::endl; // std::endl flushes the stream

                lock.lock();
            }
        }
    }

    void add_data(const double *input, size_t size)
    {
        {
            std::lock_guard<std::mutex> lock(buffer_mutex);
            buffer.emplace(input, input + size);
        }
        buffer_cv.notify_one();
    }
};

int main()
{
    Renderer renderer;
    renderer.render();

    // Retrieve and display processed outputs
    std::vector<std::vector<double>> outputs = renderer.get_processed_outputs();
    for (const auto &output : outputs)
    {
        std::cout << "Retrieved processed data: ";
        for (double val : output)
        {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}