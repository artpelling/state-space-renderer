#include <cblas.h>
#include <cnpy.h>
#include "solver.h"
#include "utils.h"

void print_data(cnpy::NpyArray matrix)
{
    for (size_t i = 0; i < matrix.shape[0]; i++)
    {
        for (size_t j = 0; j < matrix.shape[1]; j++)
        {
            std::cout << " " << matrix.data<double>()[j + matrix.shape[1] * i] << " ";
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
            std::cout << " " << data[j + m * i] << " ";
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
