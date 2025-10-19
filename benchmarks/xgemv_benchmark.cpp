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

    std::vector<T> input(m * buffer_size);
    std::vector<T> output(p * buffer_size);

    StateSpaceSystem<T> system(n, m, p, matstruct);
    Solver solver(system);
    solver.set_buffer_size(buffer_size);

    for (auto _ : state)
    {
        solver.process(input.data(), output.data());
    }
}
// BENCHMARK(BM_Solver<NativeSolver<float>>)->ArgsProduct({{10, 100, 1000}, {2}, {5}, {128}, {0, 1, 2, 3, 4, 5}}); // All combinations of set up
// BENCHMARK(BM_Solver<NativeSolver<double>>)->ArgsProduct({{10, 100, 1000}, {2}, {5}, {128}, {0, 1, 2, 3, 4, 5}});
BENCHMARK(BM_Solver<XGEMVSolver<float>>)->ArgsProduct({{10, 50, 100, 200, 500, 1000, 5000, 10000}, {100}, {100, 1000}, {1024}, {0}});
BENCHMARK(BM_Solver<XGEMVSolver<double>>)->ArgsProduct({{10, 50, 100, 200, 500, 1000, 5000, 10000}, {100}, {100, 1000}, {1024}, {0}});
BENCHMARK(BM_Solver<XGEMVSolverV1<float>>)->ArgsProduct({{10, 50, 100, 200, 500, 1000, 5000, 10000}, {100}, {100, 1000}, {1024}, {0}});
BENCHMARK(BM_Solver<XGEMVSolverV1<double>>)->ArgsProduct({{10, 50, 100, 200, 500, 1000, 5000, 10000}, {100}, {100, 1000}, {1024}, {0}});
BENCHMARK(BM_Solver<XGEMVSolverV2<float>>)->ArgsProduct({{10, 50, 100, 200, 500, 1000, 5000, 10000}, {100}, {100, 1000}, {1024}, {0}});
BENCHMARK(BM_Solver<XGEMVSolverV2<double>>)->ArgsProduct({{10, 50, 100, 200, 500, 1000, 5000, 10000}, {100}, {100, 1000}, {1024}, {0}});
//  BENCHMARK(BM_Solver<XGEMMSolver<float>>)->ArgsProduct({{10, 100, 1000}, {2}, {5}, {128}, {0, 1, 2, 3, 4}});
//  BENCHMARK(BM_Solver<XGEMMSolver<double>>)->ArgsProduct({{10, 100, 1000}, {2}, {5}, {128}, {0, 1, 2, 3, 4}});

BENCHMARK_MAIN();
