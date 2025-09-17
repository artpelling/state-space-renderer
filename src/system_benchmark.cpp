#include <benchmark/benchmark.h>
#include <string>

#include "../cnpy/cnpy.h"
#include "state_space_system.h"

MatrixStructure StructureIdx(int m)
{
    return static_cast<MatrixStructure>(m);
}

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

    NoProcess(T *A, T *B, T *C, T *D, int n, int m, int p, MatrixStructure matstruct) : StateSpaceSystem<T>(A, B, C, D, n, m, p, matstruct)
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
    MatrixStructure matstruct = StructureIdx(state.range(4));

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

    // Structure evaluation
    std::string struct_string;
    switch (matstruct)
    {
    case General:
        struct_string = "general";
        break;

    case Triangular:
        struct_string = "triangular";
        break;

    case Diagonal:
        struct_string = "diagonal";
        break;

    case Tridiagonal:
        struct_string = "tridiagonal";
        break;

    case FullHessenberg:
        struct_string = "fullhessenberg";
        break;

    case MixedHessenberg:
        struct_string = "mixedhessenberg";
        break;

    default:
        struct_string = "general";
        break;
    }

    std::string inputfile = "n" + std::to_string(n) + "p" + std::to_string(p) + "m" + std::to_string(m) + "d" + std::to_string(dataframes) + type_string + struct_string + ".npz";
    std::string path = "systems/benchmark/" + inputfile;

    MatrixData<T> matdata = load_matrices_from_hdf5<T>(path.c_str());

    for (auto _ : state)
    {
        NoProcess<T> nonempty_state(matdata.A, matdata.B, matdata.C, matdata.D, n, m, p, matstruct);
    }
}

BENCHMARK(BM_CnpyInitialization<float>)->ArgsProduct({{10, 100, 1000}, {2}, {5}, {128}, {0, 1, 2, 3, 4, 5}});
BENCHMARK(BM_CnpyInitialization<double>)->ArgsProduct({{10, 100, 1000}, {2}, {5}, {128}, {0, 1, 2, 3, 4, 5}});

BENCHMARK_MAIN();
