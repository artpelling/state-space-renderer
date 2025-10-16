#ifndef UTILS_H_
#define UTILS_H_

#include <cblas.h>
#include "hdf5.h"

#define UNDERLINE "\033[4m"
#define CLOSEUNDERLINE "\033[0m"

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

/* HDF5 Routines */

/// @brief HDF5 backend dataset info structure
/// Used to construct MatrixData structure
struct dataset_info
{
    hid_t id;
    hsize_t shape[2];
};

/// @brief Matrix dataset for storing state space in HDF5 format
/// Contains pointers to matrices A, B, C, D and their dimensions n, m, p
/// @note Matrices are stored in col-major order, default of HDF5
/// @tparam T data type of matrices
template <typename T>
struct MatrixData
{
    T *A;
    T *B;
    T *C;
    T *D;
    hsize_t n, m, p;
    MatrixStructure matstruct;
};

/// @brief Input/output sets for storing solution to a state space in HDF5 format
/// Contains pointers to input and output data and their dimensions
/// @note Vectors are stored in col-major order, default of HDF5
/// @tparam T data type of matrices
template <typename T>
struct TestData
{
    T *u;
    T *y;
    hsize_t m, p, buffer_size;
};

/// @brief Import HDF5 state space system to a MatrixData structure
/// @param filename Name of the HDF5 file to load
/// @return MatrixData containing matrices A, B, C, D and their dimensions n, m, p
/// @note Datas are stored in column-major order, default of HDF5
/// @tparam T data type of matrices
template <typename T>
MatrixData<T> load_matrices_from_hdf5(const char *filename);

/// @brief Import HDF5 state space system to a TestData structure
/// @param filename Name of the HDF5 file to load
/// @return TestData containing vector series of input and output and their dimensions buffer size
/// @note Datas are stored in column-major order, default of HDF5
/// @tparam T data type of matrices
template <typename T>
TestData<T> load_test_from_hdf5(const char *filename);

/* Simple utility function / Math */

typedef CBLAS_ORDER CBLAS_LAYOUT; /* this for backward compatibility */

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

/// @brief Transform general matrix storage to band storage.
/// @tparam T data type
/// @param A Matrix
/// @param n Matrix order
/// @param ku Number of superdiagonals
/// @param kl Number of subdiagonals
/// @return
template <typename T>
T *general_to_band_storage(T *A, int n, int ku, int kl);

/// @brief Transform banded matrix storage to general storage.
/// @tparam T data type
/// @param A Matrix
/// @param n Matrix order
/// @param ku Number of superdiagonals
/// @param kl Number of subdiagonals
/// @return
template <typename T>
T *band_to_general_storage(T *A, int n, int ku, int kl);

/// @brief Transform general matrix storage to diagonal matrix storage (band).
/// @tparam T data type
/// @param A Matrix
/// @param n Matrix order
/// @return
template <typename T>
T *general_to_diagonal(T *A, int n);

/// @brief Transform general matrix storage to tridiagonal matrix storage (band).
/// @tparam T data type
/// @param A Matrix
/// @param n Matrix order
/// @return
template <typename T>
T *general_to_tridiagonal(T *A, int n);

/// @brief Transform general matrix storage to hessenberg matrix storage.
/// @tparam T data type
/// @param A Matrix
/// @param n Matrix order
/// @param band Full band storage
/// @return
template <typename T>
T *general_to_hessenberg(T *A, int n, bool band);

/* LEVEL 1 ROUTINES */

/// @brief Standard BLAS routint of vector-scalar product.
/// @tparam T data type
/// @param N Number of elements in vector
/// @param alpha Scalar a
/// @param X Array
/// @param incX Increment of elements of X
/// @param Y Array
/// @param incY Increment of elements of Y
template <typename T>
void XAXPY(const int N, const T alpha, const T *X, const int incX, T *Y, const int incY);

/* LEVEL 2 ROUTINES */

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

/// @brief Standard BLAS routine of general band matrix-vector multiplication
/// @tparam T data type
/// @param layout
/// @param TransA
/// @param M
/// @param N
/// @param kl
/// @param ku
/// @param alpha
/// @param A
/// @param lda
/// @param X
/// @param incX
/// @param beta
/// @param Y
/// @param incY
template <typename T>
void XGBMV(CBLAS_LAYOUT layout, CBLAS_TRANSPOSE TransA, const int M, const int N,
           const int kl, const int ku, const T alpha, const T *A, const int lda,
           const T *X, const int incX, const T beta,
           T *Y, const int incY);

/// @brief Standard BLAS routine of triangular matrix-vector multiplication
/// @tparam T data type
/// @param layout
/// @param uplo
/// @param TransA
/// @param diag
/// @param N
/// @param A
/// @param lda
/// @param X
/// @param incX
template <typename T>
void XTRMV(CBLAS_LAYOUT layout, CBLAS_UPLO uplo, CBLAS_TRANSPOSE TransA,
           CBLAS_DIAG diag, const int N, const T *A, const int lda,
           T *X, const int incX);

/* LEVEL 3 ROUTINES */

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
