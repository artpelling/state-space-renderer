#ifndef RENDERER_H_
#define RENDERER_H_

#include "solver.h"
#include <queue>

template <typename T>
class Renderer
{
protected:
    Solver<T> &solver_;
    std::queue<T *> input_buff;
    T *output;

public:
    Renderer(Solver<T> &solver, int &dataframes) : solver_(solver){};
    ~Renderer();
    T *run()
    {
        output = (T *)calloc(p * dataframes, sizeof(double));
    }
};

/// @brief Random Renderer.
template <typename T>
class RandomRenderer : public Renderer<T>
{
public:
    RandomRenderer(Solver<T> &solver, int &dataframes);
    ~RandomRenderer();
    void feed_data();
};

#endif