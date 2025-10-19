#include <string>
#include <limits>
#include <cassert>
#include <type_traits>

#include <benchmark/benchmark.h>
#include "../src/state_space_system.h"
#include "../src/solver.h"
#include "../src/utils.h"

MatrixStructure StructureIdx(int m)
{
    return static_cast<MatrixStructure>(m);
}

// Solver single data benchmark
template <class Solver>
void BM_Solver(benchmark::State &state)
{
    int n = state.range(0);
    int p = state.range(1);
    int m = state.range(2);
    int buffer_size = state.range(3);

    MatrixStructure matstruct = StructureIdx(state.range(4));

    using T = typename Solver::value_type;

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

    std::string inputfile = "n" + std::to_string(n) + "p" + std::to_string(p) + "m" + std::to_string(m) + "d" + std::to_string(buffer_size) + type_string + struct_string + ".h5";
    std::string path = "systems/benchmark/" + inputfile;

    MatrixData<T> matdata = load_matrices_from_hdf5<T>(path.c_str());
    TestData<T> testdata = load_test_from_hdf5<T>(path.c_str());

    std::vector<T> output(p * buffer_size);

    StateSpaceSystem<T> system(matdata.A, matdata.B, matdata.C, matdata.D, n, m, p, matstruct);
    Solver solver(system);
    solver.set_buffer_size(buffer_size);

    for (auto _ : state)
    {
        solver.process(testdata.u, testdata.y);
    }
}
// BENCHMARK(BM_Solver<NativeSolver<float>>)->ArgsProduct({{10, 100, 1000}, {2}, {5}, {128}, {0, 1, 2, 3, 4, 5}}); // All combinations of set up
// BENCHMARK(BM_Solver<NativeSolver<double>>)->ArgsProduct({{10, 100, 1000}, {2}, {5}, {128}, {0, 1, 2, 3, 4, 5}});
BENCHMARK(BM_Solver<XGEMVSolver<float>>)->ArgsProduct({{10, 100, 1000}, {2}, {5}, {128}, {0, 1}});
BENCHMARK(BM_Solver<XGEMVSolver<double>>)->ArgsProduct({{10, 100, 1000}, {2}, {5}, {128}, {0, 1}});
// BENCHMARK(BM_Solver<XGEMMSolver<float>>)->ArgsProduct({{10, 100, 1000}, {2}, {5}, {128}, {0, 1, 2, 3, 4}});
// BENCHMARK(BM_Solver<XGEMMSolver<double>>)->ArgsProduct({{10, 100, 1000}, {2}, {5}, {128}, {0, 1, 2, 3, 4}});

BENCHMARK_MAIN();
