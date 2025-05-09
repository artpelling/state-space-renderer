#include <cstring>
#include "solver.h"
#include "utils.h"
#include <math.h>

template <typename T>
int Solver<T>::set_buffer_size(const int &buffer_size)
{
    this->buffer_size_ = buffer_size;
    return 0;
}

/* CPP-based solver */
template <typename T>
NativeSolver<T>::NativeSolver(StateSpaceSystem<T> &system) : Solver<T>(system)
{
    int n = this->system_.shape().n;
    int m = this->system_.shape().m;
    int p = this->system_.shape().p;

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

    for (int i = 0; i < this->buffer_size_; i++)
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
            switch (this->system_.matrix_struct())
            {
            case General:
                for (int k = 0; k < n; k++)
                {
                    x1[j] += this->system_.A(j, k) * x[k];
                }
                break;

            case Triangular:
                for (int k = j; k < n; k++)
                {
                    x1[j] += this->system_.A(j, k) * x[k];
                }
                break;

            case Diagonal:
                x1[j] += this->system_.A(j, j) * x[j];
                break;

            case Tridiagonal:
                for (int k = std::max(0, j - 1); k < std::min(n, j + 2); k++)
                {
                    x1[j] += this->system_.A(j, k) * x[k];
                }
                break;

            case FullHessenberg:
            case MixedHessenberg:
                for (int k = std::max(0, j - 1); k < n; k++)
                {
                    x1[j] += this->system_.A(j, k) * x[k];
                }
                break;

            default:
                for (int k = 0; k < n; k++)
                {
                    x1[j] += this->system_.A(j, k) * x[k];
                }
                break;
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

/* CBLAS_XGEMV-based solver */
template <typename T>
XGEMVSolver<T>::XGEMVSolver(StateSpaceSystem<T> &system) : Solver<T>(system)
{
    int n = this->system_.shape().n;
    int m = this->system_.shape().m;
    int p = this->system_.shape().p;

    x = (T *)calloc(n, sizeof(T));
    switch (this->system_.matrix_struct())
    {
    case General:
    case Diagonal:
    case Tridiagonal:
    case FullHessenberg:
    case MixedHessenberg:
        x1 = (T *)calloc(n, sizeof(T));
        break;

    case Triangular:
        break;

    default:
        throw std::invalid_argument("Not valid matrix structure!");
        break;
    }

    switch (this->system_.matrix_struct())
    {
    case General:
        Ax = [this, n]()
        {
            XGEMV(CblasColMajor, CblasNoTrans, n, n, one, this->system_.A(), n, x, 1, zero, x1, 1); // x1 = Ax
        };
        break;

    case Triangular:
        Ax = [this, n]()
        {
            XTRMV(CblasColMajor, CblasUpper, CblasNoTrans, CblasNonUnit, n, this->system_.A(), n, x, 1); // x = Ax
        };
        break;

    case Diagonal:
        Ax = [this, n]()
        {
            XGBMV(CblasColMajor, CblasNoTrans, n, n, 0, 0, one, this->system_.A(), 1, x, 1, zero, x1, 1); // x1 = Ax
        };
        break;

    case Tridiagonal:
        Ax = [this, n]()
        {
            XGBMV(CblasColMajor, CblasNoTrans, n, n, 1, 1, one, this->system_.A(), 3, x, 1, zero, x1, 1); // x1 = Ax
        };
        break;

    case FullHessenberg:
        Ax = [this, n]()
        {
            XGBMV(CblasColMajor, CblasNoTrans, n, n, 1, n - 1, one, this->system_.A(), n + 1, x, 1, zero, x1, 1); // x1 = Ax
        };
        break;

    case MixedHessenberg:
        Ax = [this, n]()
        {
            XGBMV(CblasColMajor, CblasNoTrans, n, n, 1, 0, one, this->system_.A() + n * n, 2, x, 1, zero, x1, 1); // x1 = A_lowerband*x
            XTRMV(CblasColMajor, CblasUpper, CblasNoTrans, CblasNonUnit, n, this->system_.A(), n, x, 1);          // x = A_triangular*x
            XAXPY(n, one, x, 1, x1, 1);                                                                           // x1 = x1 + x
        };
        break;

    default:
        throw std::invalid_argument("Not valid matrix structure!");
    }

    switch (this->system_.matrix_struct())
    {
    case General:
    case Diagonal:
    case Tridiagonal:
    case FullHessenberg:
    case MixedHessenberg:
        Bu = [this, n, m](T *input, int i)
        {
            XGEMV(CblasColMajor, CblasNoTrans, n, m, one, this->system_.B(), n, input + i * m, 1, one, x1, 1);
        };
        break;

    case Triangular:
        Bu = [this, n, m](T *input, int i)
        {
            XGEMV(CblasColMajor, CblasNoTrans, n, m, one, this->system_.B(), n, input + i * m, 1, one, x, 1); // x = x + Bu
        };
        break;

    default:
        throw std::invalid_argument("Not valid matrix structure!");
    }
}

template <typename T>
XGEMVSolver<T>::~XGEMVSolver()
{
    free(x);
    switch (this->system_.matrix_struct())
    {
    case General:
    case Diagonal:
    case Tridiagonal:
    case FullHessenberg:
    case MixedHessenberg:
        free(x1);
        break;

    case Triangular:
        break;

    default:
        free(x1);
        break;
    }
}

template <typename T>
void XGEMVSolver<T>::process(T *input, T *output)
{
    int n = this->system_.shape().n;
    int m = this->system_.shape().m;
    int p = this->system_.shape().p;
    T zero = 0;
    T one = 1;

    for (int i = 0; i < this->buffer_size_; i++)
    {
        XGEMV(CblasColMajor, CblasNoTrans, p, m, one, this->system_.D(), p, input + i * m, 1, zero, output + i * p, 1); // y = Du
        XGEMV(CblasColMajor, CblasNoTrans, p, n, one, this->system_.C(), p, x, 1, one, output + i * p, 1);              // y = y + Cx
        Ax();
        Bu(input, i);

        if (this->system_.matrix_struct() != Triangular)
        {
            std::swap(x, x1);
        }
    }
    switch (this->system_.matrix_struct())
    {
    case General:
    case Diagonal:
    case Tridiagonal:
    case FullHessenberg:
    case MixedHessenberg:
        std::swap(x, x1);
        break;

    case Triangular:
        break;

    default:
        throw std::invalid_argument("Not valid matrix structure!");
        break;
    }
}

/* CBLAS_XGEMV-based solver */
template <typename T>
XGEMVSolverV1<T>::XGEMVSolverV1(StateSpaceSystem<T> &system) : Solver<T>(system)
{
    int n = this->system_.shape().n;
    int m = this->system_.shape().m;
    int p = this->system_.shape().p;

    x = (T *)calloc(n, sizeof(T));
    switch (this->system_.matrix_struct())
    {
    case General:
    case Diagonal:
    case Tridiagonal:
    case FullHessenberg:
    case MixedHessenberg:
        x1 = (T *)calloc(n, sizeof(T));
        break;

    case Triangular:
        break;

    default:
        x1 = (T *)calloc(n, sizeof(T));
        break;
    }

    switch (this->system_.matrix_struct())
    {
    case General:
        Ax = [this, n]()
        {
            XGEMV(CblasRowMajor, CblasNoTrans, n, n, one, this->system_.A(), n, x, 1, zero, x1, 1); // x1 = Ax
        };
        break;

    case Triangular:
        Ax = [this, n]()
        {
            XTRMV(CblasRowMajor, CblasUpper, CblasNoTrans, CblasNonUnit, n, this->system_.A(), n, x, 1); // x = Ax
        };
        break;

    case Diagonal:
        Ax = [this, n]()
        {
            XGBMV(CblasRowMajor, CblasNoTrans, n, n, 0, 0, one, this->system_.A(), 1, x, 1, zero, x1, 1); // x1 = Ax
        };
        break;

    case Tridiagonal:
        Ax = [this, n]()
        {
            XGBMV(CblasRowMajor, CblasNoTrans, n, n, 1, 1, one, this->system_.A(), 3, x, 1, zero, x1, 1); // x1 = Ax
        };
        break;

    case FullHessenberg:
        Ax = [this, n]()
        {
            XGBMV(CblasRowMajor, CblasNoTrans, n, n, 1, n - 1, one, this->system_.A(), n + 1, x, 1, zero, x1, 1); // x1 = Ax
        };
        break;

    case MixedHessenberg:
        Ax = [this, n]()
        {
            XGBMV(CblasRowMajor, CblasNoTrans, n, n, 1, 0, one, this->system_.A() + n * n, 2, x, 1, zero, x1, 1); // x1 = A_lowerband*x
            XTRMV(CblasRowMajor, CblasUpper, CblasNoTrans, CblasNonUnit, n, this->system_.A(), n, x, 1);          // x = A_triangular*x
            XAXPY(n, one, x, 1, x1, 1);                                                                           // x1 = x1 + x
        };
        break;

    default:
        throw std::invalid_argument("Not valid matrix structure!");
    }

    switch (this->system_.matrix_struct())
    {
    case General:
    case Diagonal:
    case Tridiagonal:
    case FullHessenberg:
    case MixedHessenberg:
        Bu = [this, n, m](T *input, int i)
        {
            if (T(n) / T(m) < 1)
            {
                XGEMV(CblasRowMajor, CblasNoTrans, n, m, one, this->system_.B(), m, input + i * m, 1, one, x1, 1);
            }
            else
            {
                XGEMV(CblasColMajor, CblasNoTrans, n, m, one, this->system_.B(), n, input + i * m, 1, one, x1, 1);
            }
        };
        break;

    case Triangular:
        Bu = [this, n, m](T *input, int i)
        {
            if (T(n) / T(m) < 1)
            {
                XGEMV(CblasRowMajor, CblasNoTrans, n, m, one, this->system_.B(), m, input + i * m, 1, one, x, 1); // x = x + Bu
            }
            else
            {
                XGEMV(CblasColMajor, CblasNoTrans, n, m, one, this->system_.B(), n, input + i * m, 1, one, x, 1); // x = x + Bu
            }
        };
        break;

    default:
        throw std::invalid_argument("Not valid matrix structure!");
    }
}

template <typename T>
XGEMVSolverV1<T>::~XGEMVSolverV1()
{
    free(x);
    switch (this->system_.matrix_struct())
    {
    case General:
    case Diagonal:
    case Tridiagonal:
    case FullHessenberg:
    case MixedHessenberg:
        free(x1);
        break;

    case Triangular:
        break;

    default:
        free(x1);
        break;
    }
}

template <typename T>
void XGEMVSolverV1<T>::process(T *input, T *output)
{
    int n = this->system_.shape().n;
    int m = this->system_.shape().m;
    int p = this->system_.shape().p;

    for (int i = 0; i < this->buffer_size_; i++)
    {
        XGEMV(CblasColMajor, CblasNoTrans, p, m, one, this->system_.D(), p, input + i * m, 1, zero, output + i * p, 1); // y = Du
        XGEMV(CblasColMajor, CblasNoTrans, p, n, one, this->system_.C(), p, x, 1, one, output + i * p, 1);              // y = y + Cx
        Ax();
        Bu(input, i);

        if (this->system_.matrix_struct() != Triangular)
        {
            std::swap(x, x1);
        }
    }
    switch (this->system_.matrix_struct())
    {
    case General:
    case Diagonal:
    case Tridiagonal:
    case FullHessenberg:
    case MixedHessenberg:
        std::swap(x, x1);
        break;

    case Triangular:
        break;

    default:
        throw std::invalid_argument("Not valid matrix structure!");
        break;
    }
}

/* CBLAS_XGEMV-based solver */
template <typename T>
XGEMVSolverV2<T>::XGEMVSolverV2(StateSpaceSystem<T> &system) : Solver<T>(system)
{
    int n = this->system_.shape().n;
    int m = this->system_.shape().m;
    int p = this->system_.shape().p;

    x = (T *)calloc(n, sizeof(T));
    switch (this->system_.matrix_struct())
    {
    case General:
    case Diagonal:
    case Tridiagonal:
    case FullHessenberg:
    case MixedHessenberg:
        x1 = (T *)calloc(n, sizeof(T));
        break;

    case Triangular:
        break;

    default:
        x1 = (T *)calloc(n, sizeof(T));
        break;
    }

    switch (this->system_.matrix_struct())
    {
    case General:
        Ax = [this, n]()
        {
            XGEMV(CblasRowMajor, CblasNoTrans, n, n, one, this->system_.A(), n, x, 1, zero, x1, 1); // x1 = Ax
        };
        break;

    case Triangular:
        Ax = [this, n]()
        {
            XTRMV(CblasRowMajor, CblasUpper, CblasNoTrans, CblasNonUnit, n, this->system_.A(), n, x, 1); // x = Ax
        };
        break;

    case Diagonal:
        Ax = [this, n]()
        {
            XGBMV(CblasRowMajor, CblasNoTrans, n, n, 0, 0, one, this->system_.A(), 1, x, 1, zero, x1, 1); // x1 = Ax
        };
        break;

    case Tridiagonal:
        Ax = [this, n]()
        {
            XGBMV(CblasRowMajor, CblasNoTrans, n, n, 1, 1, one, this->system_.A(), 3, x, 1, zero, x1, 1); // x1 = Ax
        };
        break;

    case FullHessenberg:
        Ax = [this, n]()
        {
            XGBMV(CblasRowMajor, CblasNoTrans, n, n, 1, n - 1, one, this->system_.A(), n + 1, x, 1, zero, x1, 1); // x1 = Ax
        };
        break;

    case MixedHessenberg:
        Ax = [this, n]()
        {
            XGBMV(CblasRowMajor, CblasNoTrans, n, n, 1, 0, one, this->system_.A() + n * n, 2, x, 1, zero, x1, 1); // x1 = A_lowerband*x
            XTRMV(CblasRowMajor, CblasUpper, CblasNoTrans, CblasNonUnit, n, this->system_.A(), n, x, 1);          // x = A_triangular*x
            XAXPY(n, one, x, 1, x1, 1);                                                                           // x1 = x1 + x
        };
        break;

    default:
        throw std::invalid_argument("Not valid matrix structure!");
    }

    switch (this->system_.matrix_struct())
    {
    case General:
    case Diagonal:
    case Tridiagonal:
    case FullHessenberg:
    case MixedHessenberg:
        Bu = [this, n, m](T *input, int i)
        {
            if (T(n) / T(m) < 10 * T(log10(double(n) / double(m))))
            {
                XGEMV(CblasRowMajor, CblasNoTrans, n, m, one, this->system_.B(), m, input + i * m, 1, one, x1, 1);
            }
            else
            {
                XGEMV(CblasColMajor, CblasNoTrans, n, m, one, this->system_.B(), n, input + i * m, 1, one, x1, 1);
            }
        };
        break;

    case Triangular:
        Bu = [this, n, m](T *input, int i)
        {
            if (T(n) / T(m) < 10 * T(log10(double(n) / double(m))))
            {
                XGEMV(CblasRowMajor, CblasNoTrans, n, m, one, this->system_.B(), m, input + i * m, 1, one, x, 1); // x = x + Bu
            }
            else
            {
                XGEMV(CblasColMajor, CblasNoTrans, n, m, one, this->system_.B(), n, input + i * m, 1, one, x, 1); // x = x + Bu
            }
        };
        break;

    default:
        throw std::invalid_argument("Not valid matrix structure!");
    }
}

template <typename T>
XGEMVSolverV2<T>::~XGEMVSolverV2()
{
    free(x);
    switch (this->system_.matrix_struct())
    {
    case General:
    case Diagonal:
    case Tridiagonal:
    case FullHessenberg:
    case MixedHessenberg:
        free(x1);
        break;

    case Triangular:
        break;

    default:
        free(x1);
        break;
    }
}

template <typename T>
void XGEMVSolverV2<T>::process(T *input, T *output)
{
    int n = this->system_.shape().n;
    int m = this->system_.shape().m;
    int p = this->system_.shape().p;
    T zero = 0;
    T one = 1;

    for (int i = 0; i < this->buffer_size_; i++)
    {
        XGEMV(CblasColMajor, CblasNoTrans, p, m, one, this->system_.D(), p, input + i * m, 1, zero, output + i * p, 1); // y = Du
        XGEMV(CblasColMajor, CblasNoTrans, p, n, one, this->system_.C(), p, x, 1, one, output + i * p, 1);              // y = y + Cx
        Ax();
        Bu(input, i);

        if (this->system_.matrix_struct() != Triangular)
        {
            std::swap(x, x1);
        }
    }
    switch (this->system_.matrix_struct())
    {
    case General:
    case Diagonal:
    case Tridiagonal:
    case FullHessenberg:
    case MixedHessenberg:
        std::swap(x, x1);
        break;

    case Triangular:
        break;

    default:
        throw std::invalid_argument("Not valid matrix structure!");
        break;
    }
}

/* CBLAS_XGEMM-based solver */
template <typename T>
XGEMMSolver<T>::XGEMMSolver(StateSpaceSystem<T> &system) : Solver<T>(system)
{
    int n = this->system_.shape().n;

    X = (T *)calloc(n * (this->buffer_size_ + 1), sizeof(T));
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
    XGEMM(CblasColMajor, CblasNoTrans, CblasNoTrans, n, this->buffer_size_, m, one, this->system_.B(), n, input, m, zero, X + n, n); // Error because not yet row major

    // x1 = Ax+BU
    for (int i = 1; i < this->buffer_size_; i++)
    {
        switch (this->system_.matrix_struct())
        {
        case General:
        case Triangular:
            XGEMV(CblasColMajor, CblasNoTrans, n, n, one, this->system_.A(), n, X + n * (i - 1), 1, one, X + n * i, 1);
            break;

        case Diagonal:
            XGBMV(CblasColMajor, CblasNoTrans, n, n, 0, 0, one, this->system_.A(), 1, X + n * (i - 1), 1, one, X + n * i, 1);
            break;

        case Tridiagonal:
            XGBMV(CblasColMajor, CblasNoTrans, n, n, 1, 1, one, this->system_.A(), 3, X + n * (i - 1), 1, one, X + n * i, 1);
            break;

        case FullHessenberg:
            XGBMV(CblasColMajor, CblasNoTrans, n, n, 1, n - 1, one, this->system_.A(), n + 1, X + n * (i - 1), 1, one, X + n * i, 1);
            break;

        case MixedHessenberg:
            throw std::invalid_argument("No process method for Mixed Hessenberg!");
            break;

        default:
            break;
        }
    }

    // Y = CX+DU
    XGEMM(CblasColMajor, CblasNoTrans, CblasNoTrans, p, this->buffer_size_, n, one, this->system_.C(), p, X, n, zero, output, p);
    XGEMM(CblasColMajor, CblasNoTrans, CblasNoTrans, p, this->buffer_size_, m, one, this->system_.D(), p, input, m, one, output, p);

    for (int j = 0; j < n; j++)
    {
        std::swap(X[j], X[j + this->buffer_size_]);
    }
}

template class Solver<double>;
template class Solver<float>;

template class NativeSolver<double>;
template class NativeSolver<float>;

template class XGEMVSolver<double>;
template class XGEMVSolver<float>;

template class XGEMVSolverV1<double>;
template class XGEMVSolverV1<float>;

template class XGEMVSolverV2<double>;
template class XGEMVSolverV2<float>;

template class XGEMMSolver<double>;
template class XGEMMSolver<float>;
