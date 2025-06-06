#include <cblas.h>
#include <cstring>
#include "solver.h"
#include "utils.h"

/* HDF5 imports*/

template <typename T>
MatrixData<T> load_matrices_from_hdf5(const char *filename)
{
    hid_t file_id = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT);

    auto get_dataset_info = [](hid_t file_id, const char *name)
    {
        dataset_info info;
        info.id = H5Dopen2(file_id, name, H5P_DEFAULT);
        hid_t space = H5Dget_space(info.id);
        H5Sget_simple_extent_dims(space, info.shape, NULL);
        return info;
    };

    dataset_info Ainfo = get_dataset_info(file_id, "A");
    dataset_info Binfo = get_dataset_info(file_id, "B");
    dataset_info Cinfo = get_dataset_info(file_id, "C");
    dataset_info Dinfo = get_dataset_info(file_id, "D");

    hsize_t n = Ainfo.shape[0], m = Binfo.shape[1], p = Cinfo.shape[0];

    T *A = (T *)malloc(n * Ainfo.shape[1] * sizeof(T));
    T *B = (T *)malloc(Binfo.shape[0] * m * sizeof(T));
    T *C = (T *)malloc(p * Cinfo.shape[1] * sizeof(T));
    T *D = (T *)malloc(Dinfo.shape[0] * Dinfo.shape[1] * sizeof(T));

    hid_t dtype = std::is_same<T, float>::value ? H5T_NATIVE_FLOAT : H5T_NATIVE_DOUBLE;

    H5Dread(Ainfo.id, dtype, H5S_ALL, H5S_ALL, H5P_DEFAULT, A);
    H5Dread(Binfo.id, dtype, H5S_ALL, H5S_ALL, H5P_DEFAULT, B);
    H5Dread(Cinfo.id, dtype, H5S_ALL, H5S_ALL, H5P_DEFAULT, C);
    H5Dread(Dinfo.id, dtype, H5S_ALL, H5S_ALL, H5P_DEFAULT, D);

    H5Fclose(file_id);

    return MatrixData<T>{A, B, C, D, n, m, p};
}

// Instantiation
template MatrixData<float> load_matrices_from_hdf5<float>(const char *filename);
template MatrixData<double> load_matrices_from_hdf5<double>(const char *filename);

/* Simple utility functions*/

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