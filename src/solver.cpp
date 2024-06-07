#include <cstring>
#include <omp.h>
#include "solver.h"
#include "utils.h"

/* CPP-based solver */
template <typename T>
NativeSolver<T>::NativeSolver(StateSpaceSystem<T> &system) : Solver<T>(system)
{
    int n = this->system_.shape().n;
    int m = this->system_.shape().m;
    int p = this->system_.shape().p;

    u = (T *)calloc(m, sizeof(T));
    x = (T *)calloc(n, sizeof(T));
    x1 = (T *)calloc(n, sizeof(T));
    y = (T *)calloc(p, sizeof(T));
}

template <typename T>
NativeSolver<T>::~NativeSolver()
{
    free(u);
    free(x);
    free(x1);
    free(y);
}

template <typename T>
void NativeSolver<T>::process(T *input, T *output, int dataframes)
{
    int n = this->system_.shape().n;
    int m = this->system_.shape().m;
    int p = this->system_.shape().p;

    for (int i = 0; i < dataframes; i++)
    {
        for (int j = 0; j < m; j++)
        {
            u[j] = input[j * dataframes + i];
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
            output[j * dataframes + i] = y[j];
        }

        for (int j = 0; j < n; j++)
        {
            x[j] = x1[j]; // Move to next time step
        }
    }
}

/* CBLAS_XGEMV-based solver */
template <typename T>
XGEMVSolver<T>::XGEMVSolver(StateSpaceSystem<T> &system) : Solver<T>(system)
{
    int n = this->system_.shape().n;
    int m = this->system_.shape().m;
    int p = this->system_.shape().p;

    u = (T *)calloc(m, sizeof(T));
    x = (T *)calloc(n, sizeof(T));
    x1 = (T *)calloc(n, sizeof(T));
    y = (T *)calloc(p, sizeof(T));
}

template <typename T>
XGEMVSolver<T>::~XGEMVSolver()
{
    free(u);
    free(x);
    free(x1);
    free(y);
}

template <typename T>
void XGEMVSolver<T>::process(T *input, T *output, int dataframes)
{
    int n = this->system_.shape().n;
    int m = this->system_.shape().m;
    int p = this->system_.shape().p;
    T zero = 0;
    T one = 1;

    for (int i = 0; i < dataframes; i++)
    {
        for (int j = 0; j < m; j++)
        {
            u[j] = input[j * dataframes + i];
        }

        XGEMV(CblasRowMajor, CblasNoTrans, p, m, one, this->system_.D(), m, u, 1, zero, y, 1);  // y = Du
        XGEMV(CblasRowMajor, CblasNoTrans, p, n, one, this->system_.C(), n, x, 1, one, y, 1);   // y = y + Cx
        XGEMV(CblasRowMajor, CblasNoTrans, n, n, one, this->system_.A(), n, x, 1, zero, x1, 1); // x1 = Ax
        XGEMV(CblasRowMajor, CblasNoTrans, n, m, one, this->system_.B(), m, u, 1, one, x1, 1);  // x1 = x1 + Bu

        for (int j = 0; j < p; j++)
        {
            output[j * dataframes + i] = y[j];
        }

        for (int j = 0; j < n; j++)
        {
            x[j] = x1[j]; // Move to next time step
        }
    }
}

/* CBLAS_XGEMV-based solver */
template <typename T>
XGEMVSolverV2<T>::XGEMVSolverV2(StateSpaceSystem<T> &system) : Solver<T>(system)
{
    int n = this->system_.shape().n;
    int m = this->system_.shape().m;
    int p = this->system_.shape().p;

    u = (T *)calloc(m, sizeof(T));
    x = (T *)calloc(n, sizeof(T));
    x1 = (T *)calloc(n, sizeof(T));
    y = (T *)calloc(p, sizeof(T));
}

template <typename T>
XGEMVSolverV2<T>::~XGEMVSolverV2()
{
    free(u);
    free(x);
    free(x1);
    free(y);
}

template <typename T>
void XGEMVSolverV2<T>::process(T *input, T *output, int dataframes)
{
    int n = this->system_.shape().n;
    int m = this->system_.shape().m;
    int p = this->system_.shape().p;
    T zero = 0;
    T one = 1;

    for (int i = 0; i < dataframes; i++)
    {
        for (int j = 0; j < m; j++)
        {
#pragma omp simd
            u[j] = input[j * dataframes + i];
        }

        XGEMV(CblasRowMajor, CblasNoTrans, p, m, one, this->system_.D(), m, u, 1, zero, y, 1);  // y = Du
        XGEMV(CblasRowMajor, CblasNoTrans, p, n, one, this->system_.C(), n, x, 1, one, y, 1);   // y = y + Cx
        XGEMV(CblasRowMajor, CblasNoTrans, n, n, one, this->system_.A(), n, x, 1, zero, x1, 1); // x1 = Ax
        XGEMV(CblasRowMajor, CblasNoTrans, n, m, one, this->system_.B(), m, u, 1, one, x1, 1);  // x1 = x1 + Bu

        for (int j = 0; j < p; j++)
        {
#pragma omp simd
            output[j * dataframes + i] = y[j];
        }

        std::swap(x, x1);
        // x = std::move(x1);
    }
    std::swap(x, x1);
}

/* CBLAS_XGEMV-based solver */
template <typename T>
XGEMMSolver<T>::XGEMMSolver(StateSpaceSystem<T> &system) : Solver<T>(system)
{
    int n = this->system_.shape().n;

    x = (T *)calloc(n, sizeof(T));
    x1 = (T *)calloc(n, sizeof(T));
}

template <typename T>
XGEMMSolver<T>::~XGEMMSolver()
{
    free(x);
    free(x1);
}

template <typename T>
void XGEMMSolver<T>::process(T *input, T *output, int dataframes)
{
    int n = this->system_.shape().n;
    int m = this->system_.shape().m;
    int p = this->system_.shape().p;
    T zero = 0;
    T one = 1;

    X = (T *)calloc(n * dataframes, sizeof(T));
    Y = (T *)calloc(p * dataframes, sizeof(T));

    U = std::move(input);

    XGEMM(CblasRowMajor, CblasNoTrans, CblasNoTrans, n, dataframes, m, one, this->system_.B(), m, U, dataframes, zero, X, dataframes);

    for (int i = 0; i < dataframes; i++)
    {
        for (int j = 0; j < n; j++)
        {
            x[j] = X[i + j * dataframes];
            if (i != dataframes - 1)
            {
                x1[j] = X[i + 1 + j * dataframes];
            }
            else
            {
                x1[j] = X[j * dataframes];
            }
        }

        XGEMV(CblasRowMajor, CblasNoTrans, n, n, one, this->system_.A(), n, x, 1, one, x1, 1);
    }

    XGEMM(CblasRowMajor, CblasNoTrans, CblasNoTrans, p, dataframes, n, one, this->system_.C(), n, X, dataframes, zero, Y, dataframes);
    XGEMM(CblasRowMajor, CblasNoTrans, CblasNoTrans, p, dataframes, m, one, this->system_.D(), m, U, dataframes, one, Y, dataframes);

    memcpy(output, Y, p * dataframes * sizeof(T));

    free(U);
    free(Y);
}

template class Solver<double>;
template class Solver<float>;

template class NativeSolver<double>;
template class NativeSolver<float>;

template class XGEMVSolver<double>;
template class XGEMVSolver<float>;

template class XGEMVSolverV2<double>;
template class XGEMVSolverV2<float>;

template class XGEMMSolver<double>;
template class XGEMMSolver<float>;