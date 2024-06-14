#include <cstring>
#include "solver.h"
#include "utils.h"

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
        for (int j = 0; j < m; j++)
        {
            u[j] = input[j + i * m]
        }

        for (int j = 0; j < p; j++)
        {
            y[j] = 0;
            // D*u
            for (int k = 0; k < m; k++)
            {
                y[j] += this->system_.D(j, k) * u[k];
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
                x1[j] += this->system_.B(j, k) * u[k];
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
        X[j] = X[j + this->dataframes_];
    }
}

template class Solver<double>;
template class Solver<float>;

template class NativeSolver<double>;
template class NativeSolver<float>;

template class XGEMVSolver<double>;
template class XGEMVSolver<float>;

template class XGEMMSolver<double>;
template class XGEMMSolver<float>;