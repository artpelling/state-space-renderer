#include <benchmark/benchmark.h>
#include <string>

#include "../cnpy/cnpy.h"
#include "state_space_system.h"

template <typename T>
class NoProcess : public StateSpaceSystem<T>
{
public:
    NoProcess() : StateSpaceSystem<T>()
    {
    }

    NoProcess(int n, int m, int p) : StateSpaceSystem<T>(n, m, p)
    {
    }

    NoProcess(T *A, T *B, T *C, T *D, int n, int m, int p) : StateSpaceSystem<T>(A, B, C, D, n, m, p)
    {
    }
};

// Single precision initialization
static void BM_CnpyStateSinglePrecision(benchmark::State &state)
{
    int n = state.range(0);
    int p = state.range(1);
    int m = state.range(2);
    int dataframes = state.range(3);
    std::string input = "n" + std::to_string(n) + "p" + std::to_string(p) + "m" + std::to_string(m) + "d" + std::to_string(dataframes) + "single.npz";
    std::string path = "systems/benchmark/" + input;

    cnpy::NpyArray A_npy = cnpy::npz_load(path, "A");
    cnpy::NpyArray B_npy = cnpy::npz_load(path, "B");
    cnpy::NpyArray C_npy = cnpy::npz_load(path, "C");
    cnpy::NpyArray D_npy = cnpy::npz_load(path, "D");

    for (auto _ : state)
    {
        NoProcess<float> nonempty_state(A_npy.data<float>(), B_npy.data<float>(), C_npy.data<float>(), D_npy.data<float>(), A_npy.shape[0], B_npy.shape[1], C_npy.shape[0]);
    }
}

BENCHMARK(BM_CnpyStateSinglePrecision)->Args({10, 2, 5, 128})->Args({100, 2, 5, 128})->Args({1000, 2, 5, 128});//->Args({5000, 2, 5, 128});

// Double precision initialization
static void BM_CnpyStateDoublePrecision(benchmark::State &state)
{
    int n = state.range(0);
    int p = state.range(1);
    int m = state.range(2);
    int dataframes = state.range(3);
    std::string input = "n" + std::to_string(n) + "p" + std::to_string(p) + "m" + std::to_string(m) + "d" + std::to_string(dataframes) + "double.npz";
    std::string path = "systems/benchmark/" + input;

    cnpy::NpyArray A_npy = cnpy::npz_load(path, "A");
    cnpy::NpyArray B_npy = cnpy::npz_load(path, "B");
    cnpy::NpyArray C_npy = cnpy::npz_load(path, "C");
    cnpy::NpyArray D_npy = cnpy::npz_load(path, "D");

    for (auto _ : state)
    {
        NoProcess<double> nonempty_state(A_npy.data<double>(), B_npy.data<double>(), C_npy.data<double>(), D_npy.data<double>(), A_npy.shape[0], B_npy.shape[1], C_npy.shape[0]);
    }
}

BENCHMARK(BM_CnpyStateDoublePrecision)->Args({10, 2, 5, 128})->Args({100, 2, 5, 128})->Args({1000, 2, 5, 128});//->Args({5000, 2, 5, 128});

BENCHMARK_MAIN();
