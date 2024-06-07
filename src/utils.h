#ifndef UTILS_H_
#define UTILS_H_

#include <cblas.h>
#include "../cnpy/cnpy.h"

#define UNDERLINE "\033[4m"
#define CLOSEUNDERLINE "\033[0m"

typedef CBLAS_ORDER CBLAS_LAYOUT; /* this for backward compatibility */
/// @brief Prints Cnpy matrix.
/// @param matrix
void print_data(cnpy::NpyArray matrix);

/// @brief Prints a pointer data
/// @tparam T data type
/// @param data data pointer
/// @param n row size
/// @param m column size
template <typename T>
void print_data(T *data, int n, int m);

/// @brief Frobenius error of 2 matrix
/// @tparam T data type
/// @param x1 matrix pointer 1
/// @param x2 matrox pointer 2
/// @return
template <typename T>
T l2err(T *x1, T *x2);

/// @brief Standard BLAS routine of matrix-vector multiplication
/// @tparam T data type
/// @param layout
/// @param TransA
/// @param M
/// @param N
/// @param alpha
/// @param A
/// @param lda
/// @param X
/// @param incX
/// @param beta
/// @param Y
/// @param incY
template <typename T>
void XGEMV(CBLAS_LAYOUT layout, CBLAS_TRANSPOSE TransA, const int M, const int N,
           const T alpha, const T *A, const int lda,
           const T *X, const int incX, const T beta,
           T *Y, const int incY);

/// @brief Standard BLAS routine of matrix-matrix multiplication
/// @tparam T data type
/// @param layout
/// @param TransA
/// @param M
/// @param N
/// @param alpha
/// @param A
/// @param lda
/// @param X
/// @param incX
/// @param beta
/// @param Y
/// @param incY
template <typename T>
void XGEMM(CBLAS_LAYOUT layout, CBLAS_TRANSPOSE TransA, CBLAS_TRANSPOSE TransB, const int M, const int N, const int K, const T alpha, const T *A, const int lda,
           const T *B, const int ldb, const T beta, T *C, const int ldc);

#endif // UTILS_H_
