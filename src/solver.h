#ifndef SOLVER_H_
#define SOLVER_H_

#include "state_space_system.h"
#include "utils.h"
#include <functional>

/// @brief Base solver.
/// @tparam T Typename of class (double/float)
template <typename T>
class Solver
{
protected:
    int buffer_size_ = 64;
    StateSpaceSystem<T> &system_;

public:
    /// @brief Typename of class
    typedef T value_type;
    /// @brief Initialization with a state space system.
    /// @param system State space system.
    Solver(StateSpaceSystem<T> &system) : system_(system) {};
    /// @brief Solve state space system
    /// @param input Input of system. Given as a pointer.
    /// @param output Output of system. Given as a pointer.
    void virtual process(T *input, T *output) = 0;
    int set_buffer_size(const int &buffer_size);
};

/// @brief CPP-based solver.
/// @tparam T Typename of class (double/float)
template <typename T>
class NativeSolver : public Solver<T>
{
private:
    T *x, *x1, *y;

public:
    /// @brief Initialization with a state space system.
    /// @param system State space system.
    NativeSolver(StateSpaceSystem<T> &system);
    ~NativeSolver();
    /// @brief Solve state space system
    /// @param input Input of system. Given as a pointer.
    /// @param output Output of system. Given as a pointer.
    void process(T *input, T *output);
};

/// @brief CBLAS_XGEMV-based (matrix-vector multiplication) solver.
/// @tparam T Typename of class (double/float)
template <typename T>
class XGEMVSolver : public Solver<T>
{
private:
    T *x, *x1;
    std::function<void()> Ax;
    std::function<void(T *, int)> Bu;
    T zero = 0;
    T one = 1;

public:
    /// @brief Initialization with a state space system
    /// @param system State space system
    XGEMVSolver(StateSpaceSystem<T> &system);
    ~XGEMVSolver();
    void process(T *input, T *output);
};

/// @brief CBLAS_XGEMV-based (matrix-vector multiplication) solver. Uses a simple n/m ratio to choose row-major vs column-major BLAS calls for Bu.
/// @tparam T Typename of class (double/float)
template <typename T>
class XGEMVSolverV1 : public Solver<T>
{
private:
    T *x, *x1;
    std::function<void()> Ax;
    std::function<void(T *, int)> Bu;
    T zero = 0;
    T one = 1;

public:
    /// @brief Initialization with a state space system
    /// @param system State space system
    XGEMVSolverV1(StateSpaceSystem<T> &system);
    ~XGEMVSolverV1();
    void process(T *input, T *output);
};

/// @brief CBLAS_XGEMV-based (matrix-vector multiplication) solver. Uses a log-scaled heuristic of the n/m ratio to choose row-major vs column-major BLAS calls for Bu.
/// @tparam T Typename of class (double/float)
template <typename T>
class XGEMVSolverV2 : public Solver<T>
{
private:
    T *x, *x1;
    std::function<void()> Ax;
    std::function<void(T *, int)> Bu;
    T zero = 0;
    T one = 1;

public:
    /// @brief Initialization with a state space system
    /// @param system State space system
    XGEMVSolverV2(StateSpaceSystem<T> &system);
    ~XGEMVSolverV2();
    void process(T *input, T *output);
};

/// @brief CBLAS_XGEMM-based (matrix-matrix multiplication) solver.
template <typename T>
class XGEMMSolver : public Solver<T>
{
private:
    T *X;

public:
    /// @brief Initialization with a state space system
    /// @param system State space system
    XGEMMSolver(StateSpaceSystem<T> &system);
    ~XGEMMSolver();
    void process(T *input, T *output);
};

#endif // SOLVER_H_
