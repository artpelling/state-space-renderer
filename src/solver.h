#ifndef SOLVER_H_
#define SOLVER_H_

#include "state_space_system.h"
#include "utils.h"


/// @brief Base solver.
template <typename T>
class Solver
{
    protected:
        int n_, m_, p_;
        StateSpaceSystem<T> system_;
    public:
    /// @brief Initialization with system
    /// @param system State space system.
    Solver(StateSpaceSystem<T> system);
    /// @brief Solve state space system
    /// @param input Input
    /// @param output Output
    /// @param dataframes Number of dataframe
    void virtual process(T *input, T *output, int dataframes) = 0;
};

/// @brief CPP-based solver.
template <typename T>
class NativeSolver : public Solver<T>
{
private:
    T *u, *x, *x1, *y;

public:
    NativeSolver(StateSpaceSystem<T> system);
    ~NativeSolver();
    void process(T *input, T *output, int dataframes);
};

/// @brief CBLAS_XGEMV-based solver.
template <typename T>
class XGEMVSolver : public Solver<T>
{
private:
        T *u, *x, *x1, *y;

public:
    XGEMVSolver(StateSpaceSystem<T> system);
    ~XGEMVSolver();
    void process(T *input, T *output, int dataframes);
};

// /// @brief CBLAS_XGEMM-based solver.
// template <typename T>
// class XGEMMSolver : public Solver<T>
// {
// private:
//     T *U, *X, *X1, *Y, *x, *x1;
// };

#endif // SOLVER_H_
