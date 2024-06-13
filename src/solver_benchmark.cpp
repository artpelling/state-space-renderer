#include <string>
#include <limits>
#include <cassert>
#include <type_traits>

#include <benchmark/benchmark.h>
#include "../cnpy/cnpy.h"
#include "state_space_system.h"
#include "solver.h"
#include "utils.h"

// Solver single data benchmark
template <class Solver>
void BM_Solver(benchmark::State &state)
{
    int n = state.range(0);
    int p = state.range(1);
    int m = state.range(2);
    int dataframes = state.range(3);

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

    std::string inputfile = "n" + std::to_string(n) + "p" + std::to_string(p) + "m" + std::to_string(m) + "d" + std::to_string(dataframes) + type_string + ".npz";
    std::string path = "systems/benchmark/" + inputfile;

    cnpy::NpyArray A_npy = cnpy::npz_load(path, "A");
    cnpy::NpyArray B_npy = cnpy::npz_load(path, "B");
    cnpy::NpyArray C_npy = cnpy::npz_load(path, "C");
    cnpy::NpyArray D_npy = cnpy::npz_load(path, "D");
    cnpy::NpyArray input = cnpy::npz_load(path, "input");
    cnpy::NpyArray true_output = cnpy::npz_load(path, "output");

    T *output;
    output = (T *)calloc(p * dataframes, sizeof(T));

    StateSpaceSystem<T> system(A_npy.data<T>(), B_npy.data<T>(), C_npy.data<T>(), D_npy.data<T>(), A_npy.shape[0], B_npy.shape[1], C_npy.shape[0]);
    Solver solver(system, dataframes);

    for (auto _ : state)
    {
        solver.process(input.data<T>(), output);
    }
}

#include <queue>
#include <random>

// Solver data stream benchmark
template <class Solver>
void BM_Datastream(benchmark::State &state)
{

    int n = state.range(0);
    int p = state.range(1);
    int m = state.range(2);
    int dataframes = state.range(3);

    int dataframes_n = state.range(4);

    using T = typename Solver::value_type;
    std::default_random_engine gen;
    std::uniform_real_distribution<T> dist(0.0, 100.0);

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

    std::queue<T *> input_buff;
    T *input_arr;
    T *output;

    // Allocate buffer
    for (size_t i = 0; i < dataframes_n; i++)
    {
        input_arr = (T *)malloc(m * dataframes * sizeof(T));
        for (size_t j = 0; j < dataframes; j++)
        {
            for (size_t k = 0; k < m; k++)
            {
                input_arr[k * dataframes + j] = dist(gen);
            }
        }
        input_buff.push(std::move(input_arr));
    }

    StateSpaceSystem<T> system(A_npy.data<T>(), B_npy.data<T>(), C_npy.data<T>(), D_npy.data<T>(), A_npy.shape[0], B_npy.shape[1], C_npy.shape[0]);
    Solver solver(system, dataframes);

    for (auto _ : state)
    {
        for (size_t i = 0; i < dataframes_n; i++)
        {
            output = (T *)calloc(p * dataframes, sizeof(T));
            solver.process(input_buff.front(), output);
            input_buff.pop();
            free(output);
        }
    }
}

BENCHMARK(BM_Solver<NativeSolver<float>>)->ArgsProduct({{10, 100, 1000}, {2}, {5}, {128}}); // All combinations of set up
//   BENCHMARK(BM_Solver<NativeSolver<float>>)->RangeMultiplier(10)->Ranges({{10, 1000}, {2, 2}, {5, 5}, {128, 128}}); // Ranges of setup
BENCHMARK(BM_Solver<NativeSolver<double>>)->ArgsProduct({{10, 100, 1000}, {2}, {5}, {128}});
BENCHMARK(BM_Solver<XGEMVSolver<float>>)->ArgsProduct({{10, 100, 1000}, {2}, {5}, {128}});
BENCHMARK(BM_Solver<XGEMVSolver<double>>)->ArgsProduct({{10, 100, 1000}, {2}, {5}, {128}});
BENCHMARK(BM_Solver<XGEMMSolver<float>>)->ArgsProduct({{10, 100, 1000}, {2}, {5}, {128}});
BENCHMARK(BM_Solver<XGEMMSolver<double>>)->ArgsProduct({{10, 100, 1000}, {2}, {5}, {128}});

// BENCHMARK(BM_Datastream<NativeSolver<float>>)->ArgsProduct({{10}, {2}, {5}, {128}, {100}});

BENCHMARK_MAIN();
