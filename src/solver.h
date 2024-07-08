#ifndef SOLVER_H_
#define SOLVER_H_

#include "state_space_system.h"
#include "utils.h"
#define EIGEN_USE_BLAS
#include <eigen/Eigen/Core>

/// @brief Base solver.
template <typename T>
class Solver
{
protected:
    int dataframes_;
    StateSpaceSystem<T> &system_;

public:
    /// @brief Typename of class
    typedef T value_type;
    /// @brief Initialization with system
    /// @param system State space system.
    /// @param dataframes Number of dataframe
    Solver(StateSpaceSystem<T> &system, const int &dataframes) : system_(system){};
    /// @brief Solve state space system
    /// @param input Input
    /// @param output Output
    void virtual process(T *input, T *output) = 0;
};

/// @brief CPP-based solver.
template <typename T>
class NativeSolver : public Solver<T>
{
private:
    T *x, *x1, *y;

public:
    NativeSolver(StateSpaceSystem<T> &system, const int &dataframes);
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
    XGEMVSolver(StateSpaceSystem<T> &system, const int &dataframes);
    ~XGEMVSolver();
    void process(T *input, T *output);
};

/// @brief CBLAS_XGEMM-based solver.
template <typename T>
class XGEMMSolver : public Solver<T>
{
private:
    T *X;
    typedef Eigen::Matrix<float, 1, Eigen::Dynamic> MatrixType;
    typedef Eigen::Map<MatrixType> MapType;

public:
    XGEMMSolver(StateSpaceSystem<T> &system, const int &dataframes);
    ~XGEMMSolver();
    void process(T *input, T *output);
};

/// @brief Eigen MV-based solver.
template <typename T>
class EigenMVSolver : public Solver<T>
{
private:
    typedef Eigen::Matrix<T, Eigen::Dynamic, 1> vector_t;
    typedef Eigen::Map<vector_t> vector_map;
    typedef Eigen::Map<const vector_t> const_vector_map;

    typedef Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> matrix_t;
    typedef Eigen::Map<matrix_t> matrix_map;
    typedef Eigen::Map<const matrix_t> const_matrix_map;

    vector_t x;

public:
    EigenMVSolver(StateSpaceSystem<T> &system, const int &dataframes);
    ~EigenMVSolver();
    void process(T *input, T *output);
};

/// @brief Eigen MM-based solver.
template <typename T>
class EigenMMSolver : public Solver<T>
{
private:
    typedef Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> matrix_t;
    typedef Eigen::Map<matrix_t> matrix_map;
    typedef Eigen::Map<const matrix_t> const_matrix_map;

    matrix_t X;

public:
    EigenMMSolver(StateSpaceSystem<T> &system, const int &dataframes);
    ~EigenMMSolver();
    void process(T *input, T *output);
};

#endif // SOLVER_H_
