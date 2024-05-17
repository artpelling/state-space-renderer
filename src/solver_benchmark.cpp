#include <benchmark/benchmark.h>
#include <string>
#include <limits>
#include <cassert>

#include "../cnpy/cnpy.h"
#include "state_space_system.h"
#include "solver.h"
#include "utils.h"

// Single precision native solver
static void BM_SinglePNativeSolver(benchmark::State &state)
{
    int n = state.range(0);
    int p = state.range(1);
    int m = state.range(2);
    int dataframes = state.range(3);
    std::string inputfile = "n" + std::to_string(n) + "p" + std::to_string(p) + "m" + std::to_string(m) + "d" + std::to_string(dataframes) + "single.npz";
    std::string path = "systems/benchmark/" + inputfile;

    cnpy::NpyArray A_npy = cnpy::npz_load(path, "A");
    cnpy::NpyArray B_npy = cnpy::npz_load(path, "B");
    cnpy::NpyArray C_npy = cnpy::npz_load(path, "C");
    cnpy::NpyArray D_npy = cnpy::npz_load(path, "D");
    cnpy::NpyArray input = cnpy::npz_load(path, "input");
    cnpy::NpyArray true_output = cnpy::npz_load(path, "output");

    float err;
    float *output, *stout;

    output = (float *)calloc(p * dataframes, sizeof(float)); // allocate
    stout = true_output.data<float>();

    StateSpaceSystem<float> system(A_npy.data<float>(), B_npy.data<float>(), C_npy.data<float>(), D_npy.data<float>(), A_npy.shape[0], B_npy.shape[1], C_npy.shape[0]);
    NativeSolver<float> snat_solver(system);

    for (auto _ : state)
    {
        snat_solver.process(input.data<float>(), output, dataframes);
        // assert(l2err(output, stout) < std::numeric_limits<float>::epsilon() / float(2));
    }
}

// Double precision native solver
static void BM_DoublePNativeSolver(benchmark::State &state)
{
    int n = state.range(0);
    int p = state.range(1);
    int m = state.range(2);
    int dataframes = state.range(3);
    std::string inputfile = "n" + std::to_string(n) + "p" + std::to_string(p) + "m" + std::to_string(m) + "d" + std::to_string(dataframes) + "double.npz";
    std::string path = "systems/benchmark/" + inputfile;

    cnpy::NpyArray A_npy = cnpy::npz_load(path, "A");
    cnpy::NpyArray B_npy = cnpy::npz_load(path, "B");
    cnpy::NpyArray C_npy = cnpy::npz_load(path, "C");
    cnpy::NpyArray D_npy = cnpy::npz_load(path, "D");
    cnpy::NpyArray input = cnpy::npz_load(path, "input");
    cnpy::NpyArray true_output = cnpy::npz_load(path, "output");

    double err;
    double *output, *stout;

    output = (double *)calloc(p * dataframes, sizeof(double)); // allocate
    stout = true_output.data<double>();

    StateSpaceSystem<double> system(A_npy.data<double>(), B_npy.data<double>(), C_npy.data<double>(), D_npy.data<double>(), A_npy.shape[0], B_npy.shape[1], C_npy.shape[0]);
    NativeSolver<double> dnat_solver(system);

    for (auto _ : state)
    {
        dnat_solver.process(input.data<double>(), output, dataframes);
        // assert(l2err(output, stout) < std::numeric_limits<double>::epsilon() / double(2));
    }
}

// Single precision BLAS GEMV Solver
static void BM_FGEMVSolver(benchmark::State &state)
{
    int n = state.range(0);
    int p = state.range(1);
    int m = state.range(2);
    int dataframes = state.range(3);
    std::string inputfile = "n" + std::to_string(n) + "p" + std::to_string(p) + "m" + std::to_string(m) + "d" + std::to_string(dataframes) + "single.npz";
    std::string path = "systems/benchmark/" + inputfile;

    cnpy::NpyArray A_npy = cnpy::npz_load(path, "A");
    cnpy::NpyArray B_npy = cnpy::npz_load(path, "B");
    cnpy::NpyArray C_npy = cnpy::npz_load(path, "C");
    cnpy::NpyArray D_npy = cnpy::npz_load(path, "D");
    cnpy::NpyArray input = cnpy::npz_load(path, "input");
    cnpy::NpyArray true_output = cnpy::npz_load(path, "output");

    float err;
    float *output, *stout;

    output = (float *)calloc(p * dataframes, sizeof(float)); // allocate
    stout = true_output.data<float>();

    StateSpaceSystem<float> system(A_npy.data<float>(), B_npy.data<float>(), C_npy.data<float>(), D_npy.data<float>(), A_npy.shape[0], B_npy.shape[1], C_npy.shape[0]);
    XGEMVSolver<float> sgemv_solver(system);

    for (auto _ : state)
    {
        sgemv_solver.process(input.data<float>(), output, dataframes);
        // assert(l2err(output, stout) < std::numeric_limits<float>::epsilon() / float(2));
    }
}

// Double precision native solver
static void BM_DGEMVSolver(benchmark::State &state)
{
    int n = state.range(0);
    int p = state.range(1);
    int m = state.range(2);
    int dataframes = state.range(3);
    std::string inputfile = "n" + std::to_string(n) + "p" + std::to_string(p) + "m" + std::to_string(m) + "d" + std::to_string(dataframes) + "double.npz";
    std::string path = "systems/benchmark/" + inputfile;

    cnpy::NpyArray A_npy = cnpy::npz_load(path, "A");
    cnpy::NpyArray B_npy = cnpy::npz_load(path, "B");
    cnpy::NpyArray C_npy = cnpy::npz_load(path, "C");
    cnpy::NpyArray D_npy = cnpy::npz_load(path, "D");
    cnpy::NpyArray input = cnpy::npz_load(path, "input");
    cnpy::NpyArray true_output = cnpy::npz_load(path, "output");

    double err;
    double *output, *stout;

    output = (double *)calloc(p * dataframes, sizeof(double)); // allocate
    stout = true_output.data<double>();

    StateSpaceSystem<double> system(A_npy.data<double>(), B_npy.data<double>(), C_npy.data<double>(), D_npy.data<double>(), A_npy.shape[0], B_npy.shape[1], C_npy.shape[0]);
    XGEMVSolver<double> dgemv_solver(system);

    for (auto _ : state)
    {
        dgemv_solver.process(input.data<double>(), output, dataframes);
        // assert(l2err(output, stout) < std::numeric_limits<double>::epsilon() / double(2));
    }
}

BENCHMARK(BM_SinglePNativeSolver)->Args({10, 2, 5, 16})->Args({100, 2, 5, 16})->Args({1000, 2, 5, 16});//->Args({5000, 2, 5, 16});
BENCHMARK(BM_DoublePNativeSolver)->Args({10, 2, 5, 16})->Args({100, 2, 5, 16})->Args({1000, 2, 5, 16});//->Args({5000, 2, 5, 16});
BENCHMARK(BM_FGEMVSolver)->Args({10, 2, 5, 16})->Args({100, 2, 5, 16})->Args({1000, 2, 5, 16});//->Args({5000, 2, 5, 16});
BENCHMARK(BM_DGEMVSolver)->Args({10, 2, 5, 16})->Args({100, 2, 5, 16})->Args({1000, 2, 5, 16});//->Args({5000, 2, 5, 16});

BENCHMARK_MAIN();
