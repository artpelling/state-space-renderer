#include <cblas.h>
#include "../cnpy/cnpy.h"
#include "solver.h"
#include "utils.h"

void print_data(cnpy::NpyArray matrix)
{
    for (size_t i = 0; i < matrix.shape[0]; i++)
    {
        for (size_t j = 0; j < matrix.shape[1]; j++)
        {
            std::cout << " " << matrix.data<double>()[i + matrix.shape[0] * j] << " ";
        }
        std::cout << std::endl;
    }
}

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
        for (int i = std::max(0, j - ku); i < min(n, j + kl + 1); i++)
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
        for (int i = std::max(0, j - ku); i < min(n, j + kl + 1); i++)
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
            A_[n * n + i + i * 2] = 0;
        }

        return A_;
    }
}

template float *general_to_hessenberg(float *A, int n, bool band);
template double *general_to_hessenberg(double *A, int n, bool band);

// CBLAS template
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

// CBLAS template
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
