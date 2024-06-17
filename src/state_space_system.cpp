#include <cstring>
#include <cassert>
#include <stdlib.h>
#include "utils.h"
#include "state_space_system.h"

/*Base State System*/


template <typename T>
StateSpaceSystem<T>::StateSpaceSystem()
{
}

template <typename T>
StateSpaceSystem<T>::StateSpaceSystem(int n, int m, int p)
{
    this->n_ = n;
    this->m_ = m;
    this->p_ = p;

    this->A_ = (T *)calloc(this->n_ * this->n_, sizeof(T));
    this->B_ = (T *)calloc(this->n_ * this->m_, sizeof(T));
    this->C_ = (T *)calloc(this->p_ * this->n_, sizeof(T));
    this->D_ = (T *)calloc(this->p_ * this->m_, sizeof(T));
}

template <typename T>
StateSpaceSystem<T>::StateSpaceSystem(T *A, T *B, T *C, T *D, int n, int m, int p)
{
    this->n_ = n;
    this->m_ = m;
    this->p_ = p;

    this->A_ = (T *)aligned_alloc(64, this->n_ * this->n_ * sizeof(T));
    this->B_ = (T *)aligned_alloc(64, this->n_ * this->m_ * sizeof(T));
    this->C_ = (T *)aligned_alloc(64, this->p_ * this->n_ * sizeof(T));
    this->D_ = (T *)aligned_alloc(64, this->p_ * this->m_ * sizeof(T));

    memcpy(this->A_, A, this->n_ * this->n_ * sizeof(T));
    memcpy(this->B_, B, this->n_ * this->m_ * sizeof(T));
    memcpy(this->C_, C, this->p_ * this->n_ * sizeof(T));
    memcpy(this->D_, D, this->p_ * this->m_ * sizeof(T));
}

template <typename T>
StateSpaceSystem<T>::~StateSpaceSystem()
{
    free(this->A_);
    free(this->B_);
    free(this->C_);
    free(this->D_);
}

template <typename T>
void StateSpaceSystem<T>::info()
{
    std::cout << UNDERLINE << "State system: n=" << n_ << " m=" << m_ << " p=" << p_ << CLOSEUNDERLINE << std::endl;
    std::cout << "A:" << std::endl;
    for (size_t i = 0; i < n_; i++)
    {
        for (size_t j = 0; j < n_; j++)
        {
            std::cout << " " << A(i, j) << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "B:" << std::endl;
    for (size_t i = 0; i < n_; i++)
    {
        for (size_t j = 0; j < m_; j++)
        {
            std::cout << " " << B(i, j) << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "C:" << std::endl;
    for (size_t i = 0; i < p_; i++)
    {
        for (size_t j = 0; j < n_; j++)
        {
            std::cout << " " << C(i, j) << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "D:" << std::endl;
    for (size_t i = 0; i < p_; i++)
    {
        for (size_t j = 0; j < m_; j++)
        {
            std::cout << " " << D(i, j) << " ";
        }
        std::cout << std::endl;
    }
}

template <typename T>
struct system_shape StateSpaceSystem<T>::shape()
{
    struct system_shape shape;

    shape.n = n_;
    shape.m = m_;
    shape.p = p_;

    return shape;
}

template <typename T>
T *StateSpaceSystem<T>::A()
{
    return A_;
}

template <typename T>
T *StateSpaceSystem<T>::B()
{
    return B_;
}

template <typename T>
T *StateSpaceSystem<T>::C()
{
    return C_;
}

template <typename T>
T *StateSpaceSystem<T>::D()
{
    return D_;
}

template <typename T>
T StateSpaceSystem<T>::A(int i, int j)
{
    return A_[i + n_ * j];
}

template <typename T>
T StateSpaceSystem<T>::B(int i, int j)
{
    return B_[i + n_ * j];
}

template <typename T>
T StateSpaceSystem<T>::C(int i, int j)
{
    return C_[i + p_ * j];
}

template <typename T>
T StateSpaceSystem<T>::D(int i, int j)
{
    return D_[i + p_ * j];
}

// List available typenames here :

template class StateSpaceSystem<float>;
template class StateSpaceSystem<double>;
