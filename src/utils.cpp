#include <cblas.h>
#include <cstring>
#include "solver.h"
#include "utils.h"

template <typename T>
void print_data(T *data, int n, int m)
{
    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = 0; j < m; j++)
        {
            std::cout << " " << data[i + n * j] << " ";
        }
        std::cout << std::endl;
    }
}

template void print_data<float>(float *data, int n, int m);
template void print_data<double>(double *data, int n, int m);

// L2 error
template <typename T>
T l2err(T *x, T *y)
{
    T diff;
    T l2err = 0;
    for (size_t i = 0; i < sizeof(x); i++)
    {
        diff = abs(x[i] - y[i]);
        l2err += diff * diff;
    }
    return l2err;
}

template float l2err<float>(float *x, float *y);
template double l2err<double>(double *x, double *y);

/*Matrix storage conversions*/

template <typename T>
T *general_to_band_storage(T *A, int n, int ku, int kl)
{
    int k;
    int lda = 1 + ku + kl;
    T *A_;

    A_ = (T *)malloc(lda * n * sizeof(T));
    for (int j = 0; j < n; j++)
    {
        k = ku - j;
        for (int i = std::max(0, j - ku); i < std::min(n, j + kl + 1); i++)
        {
            A_[(k + i) + j * lda] = A[i + j * n];
        }
    }

    return A_;
}

template float *general_to_band_storage(float *A, int n, int ku, int kl);
template double *general_to_band_storage(double *A, int n, int ku, int kl);

template <typename T>
T *band_to_general_storage(T *A, int n, int ku, int kl)
{
    int k;
    int lda = 1 + ku + kl;
    T *A_;

    A_ = (T *)malloc(n * n * sizeof(T));
    for (int j = 0; j < n; j++)
    {
        k = ku - j;
        for (int i = std::max(0, j - ku); i < std::min(n, j + kl + 1); i++)
        {
            A_[i + j * n] = A[(k + i) + j * lda];
        }
    }

    return A_;
}

template float *band_to_general_storage(float *A, int n, int ku, int kl);
template double *band_to_general_storage(double *A, int n, int ku, int kl);

template <typename T>
T *general_to_diagonal(T *A, int n)
{
    return general_to_band_storage(A, n, 0, 0);
}

template float *general_to_diagonal(float *A, int n);
template double *general_to_diagonal(double *A, int n);

template <typename T>
T *general_to_tridiagonal(T *A, int n)
{
    return general_to_band_storage(A, n, 1, 1);
}

template float *general_to_tridiagonal(float *A, int n);
template double *general_to_tridiagonal(double *A, int n);

template <typename T>
T *general_to_hessenberg(T *A, int n, bool band)
{
    if (band)
    {
        return general_to_band_storage(A, n, n - 1, 1);
    }
    else
    {
        T *A_;
        A_ = (T *)malloc((n + 2) * n * sizeof(T));
        memcpy(A_, A, n * n * sizeof(T));
        memcpy(A_ + n * n, general_to_band_storage(A, n, 0, 1), 2 * n * sizeof(T));
        for (size_t i = 0; i < n; i++)
        {
            A_[n * n + 2 * i] = 0;
        }

        return A_;
    }
}

template float *general_to_hessenberg(float *A, int n, bool band);
template double *general_to_hessenberg(double *A, int n, bool band);

/* LEVEL 1 ROUTINES */

// CBLAS AXPY template
template <>
void XAXPY<float>(const int N, const float alpha, const float *X, const int incX, float *Y, const int incY)
{
    cblas_saxpy(N, alpha, X, incX, Y, incY);
}
template <>
void XAXPY<double>(const int N, const double alpha, const double *X, const int incX, double *Y, const int incY)
{
    cblas_daxpy(N, alpha, X, incX, Y, incY);
}

/* LEVEL 2 ROUTINES */

// CBLAS GEMV template
template <>
void XGEMV<float>(CBLAS_LAYOUT layout, CBLAS_TRANSPOSE TransA, const int M, const int N,
                  const float alpha, const float *A, const int lda,
                  const float *X, const int incX,
                  const float beta, float *Y, const int incY)
{
    cblas_sgemv(layout, TransA, M, N, alpha, A, lda, X, incX, beta, Y, incY);
}
template <>
void XGEMV<double>(CBLAS_LAYOUT layout, CBLAS_TRANSPOSE TransA, const int M, const int N,
                   const double alpha, const double *A, const int lda,
                   const double *X, const int incX,
                   const double beta, double *Y, const int incY)
{
    cblas_dgemv(layout, TransA, M, N, alpha, A, lda, X, incX, beta, Y, incY);
}

// CBLAS GBMV template
template <>
void XGBMV<float>(CBLAS_LAYOUT layout, CBLAS_TRANSPOSE TransA,
                  const int M, const int N, const int kl, const int ku,
                  const float alpha, const float *A, const int lda,
                  const float *X, const int incX,
                  const float beta, float *Y, const int incY)
{
    cblas_sgbmv(layout, TransA, M, N, kl, ku, alpha, A, lda, X, incX, beta, Y, incY);
}
template <>
void XGBMV<double>(CBLAS_LAYOUT layout, CBLAS_TRANSPOSE TransA,
                   const int M, const int N, const int kl, const int ku,
                   const double alpha, const double *A, const int lda,
                   const double *X, const int incX,
                   const double beta, double *Y, const int incY)
{
    cblas_dgbmv(layout, TransA, M, N, kl, ku, alpha, A, lda, X, incX, beta, Y, incY);
}

// CBLAS TRMV template
template <>
void XTRMV<float>(CBLAS_LAYOUT layout, CBLAS_UPLO uplo, CBLAS_TRANSPOSE TransA,
                  CBLAS_DIAG diag, const int N, const float *A, const int lda,
                  float *X, const int incX)
{
    cblas_strmv(layout, uplo, TransA, diag, N, A, lda, X, incX);
}

template <>
void XTRMV<double>(CBLAS_LAYOUT layout, CBLAS_UPLO uplo, CBLAS_TRANSPOSE TransA,
                   CBLAS_DIAG diag, const int N, const double *A, const int lda,
                   double *X, const int incX)
{
    cblas_dtrmv(layout, uplo, TransA, diag, N, A, lda, X, incX);
}

/* LEVEL 3 Routine */

// CBLAS GEMM template
template <>
void XGEMM<float>(CBLAS_LAYOUT layout, CBLAS_TRANSPOSE TransA, CBLAS_TRANSPOSE TransB, const int M, const int N, const int K,
                  const float alpha, const float *A, const int lda,
                  const float *B, const int ldb,
                  const float beta, float *C, const int ldc)
{
    cblas_sgemm(layout, TransA, TransB, M, N, K, alpha, A, lda, B, ldb, beta, C, ldc);
}
template <>
void XGEMM<double>(CBLAS_LAYOUT layout, CBLAS_TRANSPOSE TransA, CBLAS_TRANSPOSE TransB, const int M, const int N, const int K,
                   const double alpha, const double *A, const int lda,
                   const double *B, const int ldb,
                   const double beta, double *C, const int ldc)
{
    cblas_dgemm(layout, TransA, TransB, M, N, K, alpha, A, lda, B, ldb, beta, C, ldc);
}