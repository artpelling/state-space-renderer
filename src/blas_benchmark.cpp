#include <benchmark/benchmark.h>
#include <cblas.h>
#include "utils.h"

CBLAS_ORDER OrderIdx(int m)
{
    return static_cast<CBLAS_ORDER>(m);
}

CBLAS_TRANSPOSE TransIdx(int m)
{
    return static_cast<CBLAS_TRANSPOSE>(m);
}

template <typename T>
void General(benchmark::State &state)
{
    int p = state.range(0);
    int m = state.range(1);
    int buffer_size = state.range(2);
    CBLAS_ORDER cblas_order = OrderIdx(state.range(3));
    CBLAS_TRANSPOSE cblas_trans = TransIdx(state.range(4));

    T *A;
    A = (T *)calloc(p * m, sizeof(T));
    T one = 1;

    int lda;
    switch (cblas_order)
    {
    case CblasColMajor:
        lda = p;
        break;

    case CblasRowMajor:
        lda = m;
        break;

    default:
        break;
    }

    T *input, *x;
    int input_length, x_length;
    switch (cblas_trans)
    {
    case CblasNoTrans:
        input_length = m;
        x_length = p;
        break;

    case CblasTrans:
        input_length = p;
        x_length = m;
        break;

    default:
        break;
    }

    input = (T *)calloc(input_length * buffer_size, sizeof(T));
    x = (T *)calloc(x_length, sizeof(T));

    for (auto _ : state)
    {
        for (int i = 0; i < buffer_size; i++)
        {
            XGEMV(cblas_order, cblas_trans, p, m, one, A, lda, input + i * input_length, 1, one, x, 1);
        }
    }
}

template <typename T>
void General(benchmark::State &state)
{
    int p = state.range(0);
    int m = state.range(1);
    int buffer_size = state.range(2);
    CBLAS_ORDER cblas_order = OrderIdx(state.range(3));
    CBLAS_TRANSPOSE cblas_trans = TransIdx(state.range(4));

    T *A;
    A = (T *)calloc(p * m, sizeof(T));
    T one = 1;

    int lda;
    switch (cblas_order)
    {
    case CblasColMajor:
        lda = p;
        break;

    case CblasRowMajor:
        lda = m;
        break;

    default:
        break;
    }

    T *input, *x;
    int input_length, x_length;
    switch (cblas_trans)
    {
    case CblasNoTrans:
        input_length = m;
        x_length = p;
        break;

    case CblasTrans:
        input_length = p;
        x_length = m;
        break;

    default:
        break;
    }

    input = (T *)calloc(input_length * buffer_size, sizeof(T));
    x = (T *)calloc(x_length, sizeof(T));

    for (auto _ : state)
    {
        for (int i = 0; i < buffer_size; i++)
        {
            XGEMV(cblas_order, cblas_trans, p, m, one, A, lda, input + i * input_length, 1, one, x, 1);
        }
    }
}

// GUIDE :
//  101 -> CblasColMajor, 102 -> CblasRowMajor
//  111 -> CblasNoTrans, 112 -> CblasTrans
BENCHMARK(General<float>)->ArgsProduct({{10, 1000}, {10, 1000}, {1}, {101, 102}, {111, 112}});
// BENCHMARK Results
// General<float>/1000/10/1/101/111          2179 ns
// General<float>/10/1000/1/101/111           778 ns
// Tall skinny matrices is slower in Column Major and NoTranspose than short fat matrices
// General<float>/1000/10/1/102/111           600 ns          600 ns      1508177
// General<float>/10/1000/1/102/111          7405 ns         7404 ns        98432
// Short fat matrices is even slower in Row Major and NoTranspose than tall skinny matrices
// General<float>/1000/10/1/101/112          8192 ns         8190 ns        79821
// General<float>/10/1000/1/101/112           661 ns          661 ns      1277924
// Transposed tall skinny matrices (short fat) is slower in Column Major than transposed short fat matrices

// BENCHMARK(General<double>)->ArgsProduct({{10, 1000}, {10, 1000}, {1}, {101, 102}, {111, 112}});

BENCHMARK_MAIN();