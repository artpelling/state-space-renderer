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

/* Base State Space System */

/// @brief Class of a system of states, consisting of state matrices A, B, C, D.
/// @tparam T datatype
template <typename T>
class StateSpaceSystem
{

protected:
    int n_ = 0, m_ = 0, p_ = 0;
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
    StateSpaceSystem(T *A, T *B, T *C, T *D, int n, int m, int p);

    /* Destructors */
    /// @brief System destructor.
    ~StateSpaceSystem();

    /* Utility methods */
    /// @brief Prints system information and all matrices.
    void info();

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
