#ifndef STATE_SPACE_SYSTEM_H_
#define STATE_SPACE_SYSTEM_H_

#include <stdlib.h>
#include <iostream>
#include <array>

#include "utils.h"

/* Utility structs */

struct system_shape
{
    int n, m, p;
};

/* Matrix Structure Enumerator */

/// @brief Matrix structure
enum MatrixStructure
{
    /// @brief General matrix
    General = 0,
    /// @brief Triangular matrix
    Triangular = 1,
    /// @brief Diagonal matrix
    Diagonal = 2,
    /// @brief Tridiagonal matrix
    Tridiagonal = 3,
    /// @brief Hessenberg matrix with banded storage
    FullHessenberg = 4,
    /// @brief Hessenberg matrix with mix of banded and triangular matrix storage
    MixedHessenberg = 5
};

/// @brief Utility for deducing matrix structures with string. Useful for passing arguments in main.
/// @param matstruct_str String of matrix structure.
/// @return MatrixStructure object.
MatrixStructure string_to_matstruct(const std::string &matstruct_str);

/* Base State Space System */

/// @brief Class of a system of states, consisting of state matrices A, B, C, D.
/// @tparam T datatype
template <typename T>
class StateSpaceSystem
{

protected:
    int n_ = 0, m_ = 0, p_ = 0;
    MatrixStructure A_type_ = General;
    T *A_, *B_, *C_, *D_;

public:
    /* Constructors */
    /// @brief Unallocated initialization.
    /// Warning: no shape defined!
    StateSpaceSystem();
    /// @brief Allocated initialization. All set to zero.
    /// @param n dimension of A. nxn.
    /// @param m column size of B. nxm.
    /// @param p row size of C. pxn.
    StateSpaceSystem(int n, int m, int p);
    /// @brief Allocated initialization. All set to value of the defined A, B, C, D.
    /// @param A Matrix A
    /// @param B Matrix B
    /// @param C Matrix C
    /// @param D Matrix D
    /// @param n dimension of A. nxn.
    /// @param m column size of B. nxm.
    /// @param p row size of C. pxn.
    /// @param A_type Structure of matrix A. \n
    /// General, Triangular, Diagonal, Tridiagonal, FullHessenberg, MixedHessenberg.
    StateSpaceSystem(T *A, T *B, T *C, T *D, int n, int m, int p, MatrixStructure A_type);

    /* Destructors */
    /// @brief System destructor.
    ~StateSpaceSystem();

    /* Utility methods */
    /// @brief Prints system information and all matrices.
    void info();

    /// @brief Matrix structure of A
    /// @return MatrixStructure variable of A.
    MatrixStructure matrix_struct();

    /// @brief State matrix A
    T *A();
    /// @brief State matrix B
    T *B();
    /// @brief State matrix C
    T *C();
    /// @brief State matrix D
    T *D();

    /// @brief Element of state matrix A
    /// @param i Row index
    /// @param j Column index
    T A(int i, int j);
    /// @brief Element of state matrix B
    /// @param i Row index
    /// @param j Column index
    T B(int i, int j);
    /// @brief Element of state matrix C
    /// @param i Row index
    /// @param j Column index
    T C(int i, int j);
    /// @brief Element of state matrix D
    /// @param i Row index
    /// @param j Column index
    T D(int i, int j);

    /// @brief Gets a struct of the systems shape, n, m, p.
    struct system_shape shape();
};
#endif // STATE_SPACE_SYSTEM_H_
