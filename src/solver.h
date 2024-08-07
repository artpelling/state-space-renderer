#ifndef SOLVER_H_
#define SOLVER_H_

#include "state_space_system.h"
#include "utils.h"

/// @brief Base solver.
template <typename T>
class Solver
{
protected:
    int buffer_size_ = 64;
    StateSpaceSystem<T> &system_;

public:
    /// @brief Typename of class
    typedef T value_type;
    /// @brief Initialization with system
    /// @param system State space system.
    /// @param buffer_size Number of dataframe
    Solver(StateSpaceSystem<T> &system) : system_(system){};
    /// @brief Solve state space system
    /// @param input Input
    /// @param output Output
    void virtual process(T *input, T *output) = 0;
    int set_buffer_size(const int &buffer_size);
};


/// @brief CPP-based solver.
template <typename T>
class NativeSolver : public Solver<T>
{
private:
    T *x, *x1, *y;

public:
    NativeSolver(StateSpaceSystem<T> &system);
    ~NativeSolver();
    void process(T *input, T *output);
};

/// @brief CBLAS_XGEMV-based solver.
template <typename T>
class XGEMVSolver : public Solver<T>
{
private:
    T *x, *x1;

public:
    XGEMVSolver(StateSpaceSystem<T> &system);
    ~XGEMVSolver();
    void process(T *input, T *output);
};

/// @brief CBLAS_XGEMM-based solver.
template <typename T>
class XGEMMSolver : public Solver<T>
{
private:
    T *X;

public:
    XGEMMSolver(StateSpaceSystem<T> &system);
    ~XGEMMSolver();
    void process(T *input, T *output);
};

#endif // SOLVER_H_
