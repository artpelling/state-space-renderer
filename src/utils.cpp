#include <cblas.h>
#include <cstring>
#include "solver.h"
#include "utils.h"

/* HDF5 imports*/

auto get_dataset_info = [](hid_t file_id, const char *name)
{
    dataset_info info;
    info.id = H5Dopen2(file_id, name, H5P_DEFAULT);
    hid_t space = H5Dget_space(info.id);

    int ndims = H5Sget_simple_extent_ndims(space);
    hsize_t shape[2] = {1, 1}; // default shape

    if (ndims == 1)
    {
        H5Sget_simple_extent_dims(space, &shape[0], NULL);
        shape[1] = 1;
    }
    else if (ndims == 2)
    {
        H5Sget_simple_extent_dims(space, shape, NULL);
    }

    info.shape[0] = shape[0];
    info.shape[1] = shape[1];

    return info;
};

template <typename T>
MatrixData<T> load_matrices_from_hdf5(const char *filename)
{
    hid_t file_id = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT);

    dataset_info Ainfo = get_dataset_info(file_id, "A");
    dataset_info Binfo = get_dataset_info(file_id, "B");
    dataset_info Cinfo = get_dataset_info(file_id, "C");
    dataset_info Dinfo = get_dataset_info(file_id, "D");
    dataset_info ninfo = get_dataset_info(file_id, "n");
    dataset_info minfo = get_dataset_info(file_id, "m");
    dataset_info pinfo = get_dataset_info(file_id, "p");
    dataset_info structure_info = get_dataset_info(file_id, "structure");

    hsize_t n, m, p;

    H5Dread(ninfo.id, H5T_NATIVE_HSIZE, H5S_ALL, H5S_ALL, H5P_DEFAULT, &n);
    H5Dread(minfo.id, H5T_NATIVE_HSIZE, H5S_ALL, H5S_ALL, H5P_DEFAULT, &m);
    H5Dread(pinfo.id, H5T_NATIVE_HSIZE, H5S_ALL, H5S_ALL, H5P_DEFAULT, &p);

    int structure_val;

    H5Dread(structure_info.id, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, &structure_val);

    T *A = (T *)malloc(Ainfo.shape[0] * Ainfo.shape[1] * sizeof(T));
    T *B = (T *)malloc(Binfo.shape[0] * Binfo.shape[1] * sizeof(T));
    T *C = (T *)malloc(Cinfo.shape[0] * Cinfo.shape[1] * sizeof(T));
    T *D = (T *)malloc(Dinfo.shape[0] * Dinfo.shape[1] * sizeof(T));

    hid_t dtype = std::is_same<T, float>::value ? H5T_NATIVE_FLOAT : H5T_NATIVE_DOUBLE;

    H5Dread(Ainfo.id, dtype, H5S_ALL, H5S_ALL, H5P_DEFAULT, A);
    H5Dread(Binfo.id, dtype, H5S_ALL, H5S_ALL, H5P_DEFAULT, B);
    H5Dread(Cinfo.id, dtype, H5S_ALL, H5S_ALL, H5P_DEFAULT, C);
    H5Dread(Dinfo.id, dtype, H5S_ALL, H5S_ALL, H5P_DEFAULT, D);

    H5Fclose(file_id);

    return MatrixData<T>{A, B, C, D, n, m, p, static_cast<MatrixStructure>(structure_val)};
}

// Instantiation
template MatrixData<float> load_matrices_from_hdf5<float>(const char *filename);
template MatrixData<double> load_matrices_from_hdf5<double>(const char *filename);

template <typename T>
TestData<T> load_test_from_hdf5(const char *filename)
{
    hid_t file_id = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT);

    dataset_info uinfo = get_dataset_info(file_id, "u");
    dataset_info yinfo = get_dataset_info(file_id, "y");
    dataset_info minfo = get_dataset_info(file_id, "m");
    dataset_info pinfo = get_dataset_info(file_id, "p");
    dataset_info bufferinfo = get_dataset_info(file_id, "bf_size");

    hsize_t buffer_size, m, p;

    H5Dread(minfo.id, H5T_NATIVE_HSIZE, H5S_ALL, H5S_ALL, H5P_DEFAULT, &m);
    H5Dread(pinfo.id, H5T_NATIVE_HSIZE, H5S_ALL, H5S_ALL, H5P_DEFAULT, &p);
    H5Dread(bufferinfo.id, H5T_NATIVE_HSIZE, H5S_ALL, H5S_ALL, H5P_DEFAULT, &buffer_size);

    T *u = (T *)malloc(uinfo.shape[0] * uinfo.shape[1] * sizeof(T));
    T *y = (T *)malloc(yinfo.shape[0] * yinfo.shape[1] * sizeof(T));

    hid_t dtype = std::is_same<T, float>::value ? H5T_NATIVE_FLOAT : H5T_NATIVE_DOUBLE;

    H5Dread(uinfo.id, dtype, H5S_ALL, H5S_ALL, H5P_DEFAULT, u);
    H5Dread(yinfo.id, dtype, H5S_ALL, H5S_ALL, H5P_DEFAULT, y);

    H5Fclose(file_id);

    return TestData<T>{u, y, m, p, buffer_size};
}

// Instantiation
template TestData<float> load_test_from_hdf5<float>(const char *filename);
template TestData<double> load_test_from_hdf5<double>(const char *filename);

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