#include <cstring>
#include "solver.h"
#include "utils.h"

template class Solver<double>;
template class Solver<float>;

/* CPP-based solver */
template <typename T>
NativeSolver<T>::NativeSolver(StateSpaceSystem<T> &system, const int &dataframes) : Solver<T>(system, dataframes)
{
    int n = this->system_.shape().n;
    int m = this->system_.shape().m;
    int p = this->system_.shape().p;
    this->dataframes_ = dataframes;

    x = (T *)calloc(n, sizeof(T));
    x1 = (T *)calloc(n, sizeof(T));
    y = (T *)calloc(p, sizeof(T));
}

template <typename T>
NativeSolver<T>::~NativeSolver()
{
    free(x);
    free(x1);
    free(y);
}

template <typename T>
void NativeSolver<T>::process(T *input, T *output)
{
    int n = this->system_.shape().n;
    int m = this->system_.shape().m;
    int p = this->system_.shape().p;

    for (int i = 0; i < this->dataframes_; i++)
    {
        for (int j = 0; j < p; j++)
        {
            y[j] = 0;
            // D*u
            for (int k = 0; k < m; k++)
            {
                y[j] += this->system_.D(j, k) * input[k + i * m];
            }
            // C*x
            for (int k = 0; k < n; k++)
            {
                y[j] += this->system_.C(j, k) * x[k];
            }
        }

        for (int j = 0; j < n; j++)
        {
            x1[j] = 0;
            // A*x
            for (int k = 0; k < n; k++)
            {
                x1[j] += this->system_.A(j, k) * x[k];
            }
            // B*u
            for (int k = 0; k < m; k++)
            {
                x1[j] += this->system_.B(j, k) * input[k + i * m];
            }
        }

        for (int j = 0; j < p; j++)
        {
            output[j + i * p] = y[j];
        }

        std::swap(x, x1);
    }
    std::swap(x, x1);
}

template class NativeSolver<double>;
template class NativeSolver<float>;

/* CBLAS_XGEMV-based solver */
template <typename T>
XGEMVSolver<T>::XGEMVSolver(StateSpaceSystem<T> &system, const int &dataframes) : Solver<T>(system, dataframes)
{
    int n = this->system_.shape().n;
    int m = this->system_.shape().m;
    int p = this->system_.shape().p;
    this->dataframes_ = dataframes;

    x = (T *)calloc(n, sizeof(T));
    x1 = (T *)calloc(n, sizeof(T));
}

template <typename T>
XGEMVSolver<T>::~XGEMVSolver()
{
    free(x);
    free(x1);
}

template <typename T>
void XGEMVSolver<T>::process(T *input, T *output)
{
    int n = this->system_.shape().n;
    int m = this->system_.shape().m;
    int p = this->system_.shape().p;
    T zero = 0;
    T one = 1;

    for (int i = 0; i < this->dataframes_; i++)
    {
        XGEMV(CblasColMajor, CblasNoTrans, p, m, one, this->system_.D(), p, input + i * m, 1, zero, output + i * p, 1); // y = Du
        XGEMV(CblasColMajor, CblasNoTrans, p, n, one, this->system_.C(), p, x, 1, one, output + i * p, 1);              // y = y + Cx
        XGEMV(CblasColMajor, CblasNoTrans, n, n, one, this->system_.A(), n, x, 1, zero, x1, 1);                         // x1 = Ax
        XGEMV(CblasColMajor, CblasNoTrans, n, m, one, this->system_.B(), n, input + i * m, 1, one, x1, 1);              // x1 = x1 + Bu

        std::swap(x, x1);
    }
    std::swap(x, x1);
}

template class XGEMVSolver<double>;
template class XGEMVSolver<float>;

/* CBLAS_XGEMM-based solver */
template <typename T>
XGEMMSolver<T>::XGEMMSolver(StateSpaceSystem<T> &system, const int &dataframes) : Solver<T>(system, dataframes)
{
    int n = this->system_.shape().n;
    this->dataframes_ = dataframes;

    X = (T *)calloc(n * (this->dataframes_ + 1), sizeof(T));
}

template <typename T>
XGEMMSolver<T>::~XGEMMSolver()
{
    free(X);
}

template <typename T>
void XGEMMSolver<T>::process(T *input, T *output)
{
    int n = this->system_.shape().n;
    int m = this->system_.shape().m;
    int p = this->system_.shape().p;
    T zero = 0;
    T one = 1;

    // X = BU
    XGEMM(CblasColMajor, CblasNoTrans, CblasNoTrans, n, this->dataframes_, m, one, this->system_.B(), n, input, m, zero, X + n, n);

    // x1 = Ax+BU
    for (int i = 1; i < this->dataframes_; i++)
    {
        XGEMV(CblasColMajor, CblasNoTrans, n, n, one, this->system_.A(), n, X + n * (i - 1), 1, one, X + n * i, 1);
    }

    // Y = CX+DU
    XGEMM(CblasColMajor, CblasNoTrans, CblasNoTrans, p, this->dataframes_, n, one, this->system_.C(), p, X, n, zero, output, p);
    XGEMM(CblasColMajor, CblasNoTrans, CblasNoTrans, p, this->dataframes_, m, one, this->system_.D(), p, input, m, one, output, p);

    for (int j = 0; j < n; j++)
    {
        std::swap(X[j], X[j + this->dataframes_]);
    }
}

template class XGEMMSolver<double>;
template class XGEMMSolver<float>;

/* Eigen MV-based solver */
template <typename T>
EigenMVSolver<T>::EigenMVSolver(StateSpaceSystem<T> &system, const int &dataframes) : Solver<T>(system, dataframes)
{
    int n = this->system_.shape().n;
    int m = this->system_.shape().m;
    int p = this->system_.shape().p;
    this->dataframes_ = dataframes;

    x = vector_t::Zero(n);
}

template <typename T>
EigenMVSolver<T>::~EigenMVSolver()
{
}

template <typename T>
void EigenMVSolver<T>::process(T *input, T *output)
{
    int n = this->system_.shape().n;
    int m = this->system_.shape().m;
    int p = this->system_.shape().p;

    const_matrix_map A(this->system_.A(), n, n);
    const_matrix_map B(this->system_.B(), n, m);
    const_matrix_map C(this->system_.C(), p, n);
    const_matrix_map D(this->system_.D(), p, m);

    const_matrix_map u(input, m, this->dataframes_);
    matrix_map y(output, p, this->dataframes_);

    for (int i = 0; i < this->dataframes_; i++)
    {
        y.col(i) = C * x + D * u.col(i);
        x = A * x + B * u.col(i);
    }
}

template class EigenMVSolver<double>;
template class EigenMVSolver<float>;

/* Eigen MM-based solver */
template <typename T>
EigenMMSolver<T>::EigenMMSolver(StateSpaceSystem<T> &system, const int &dataframes) : Solver<T>(system, dataframes)
{
    int n = this->system_.shape().n;
    int m = this->system_.shape().m;
    int p = this->system_.shape().p;
    this->dataframes_ = dataframes;

    X = matrix_t::Zero(n, this->dataframes_ + 1);
}

template <typename T>
EigenMMSolver<T>::~EigenMMSolver()
{
}

template <typename T>
void EigenMMSolver<T>::process(T *input, T *output)
{
    int n = this->system_.shape().n;
    int m = this->system_.shape().m;
    int p = this->system_.shape().p;

    const_matrix_map A(this->system_.A(), n, n);
    const_matrix_map B(this->system_.B(), n, m);
    const_matrix_map C(this->system_.C(), p, n);
    const_matrix_map D(this->system_.D(), p, m);

    const_matrix_map U(input, m, this->dataframes_);
    matrix_map Y(output, p, this->dataframes_);

    X.rightCols(this->dataframes_) = B * U;
    for (int i = 0; i < this->dataframes_; i++)
    {
        X.col(i + 1) = A * X.col(i) + X.col(i + 1);
    }
    Y = C * X.leftCols(this->dataframes_) + D * U;

    X.col(0) = X.col(this->dataframes_);
}

template class EigenMMSolver<double>;
template class EigenMMSolver<float>;