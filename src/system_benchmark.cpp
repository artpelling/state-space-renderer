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

template <typename T>
void BM_CnpyInitialization(benchmark::State &state)
{
    int n = state.range(0);
    int p = state.range(1);
    int m = state.range(2);
    int dataframes = state.range(3);

    // Type evaluation
    std::string type_string;
    if (std ::integral_constant<bool, std::is_same<float, typename std::remove_cv<T>::type>::value>::value)
    {
        type_string = "single";
    }
    else if (std ::integral_constant<bool, std::is_same<double, typename std::remove_cv<T>::type>::value>::value)
    {
        type_string = "double";
    }

    std::string inputfile = "n" + std::to_string(n) + "p" + std::to_string(p) + "m" + std::to_string(m) + "d" + std::to_string(dataframes) + type_string + ".npz";
    std::string path = "systems/benchmark/" + inputfile;

    cnpy::NpyArray A_npy = cnpy::npz_load(path, "A");
    cnpy::NpyArray B_npy = cnpy::npz_load(path, "B");
    cnpy::NpyArray C_npy = cnpy::npz_load(path, "C");
    cnpy::NpyArray D_npy = cnpy::npz_load(path, "D");

    for (auto _ : state)
    {
        NoProcess<T> nonempty_state(A_npy.data<T>(), B_npy.data<T>(), C_npy.data<T>(), D_npy.data<T>(), A_npy.shape[0], B_npy.shape[1], C_npy.shape[0]);
    }
}

BENCHMARK(BM_CnpyInitialization<float>)->ArgsProduct({{10, 100, 1000}, {2}, {5}, {128}});
BENCHMARK(BM_CnpyInitialization<double>)->ArgsProduct({{10, 100, 1000}, {2}, {5}, {128}});

BENCHMARK_MAIN();
