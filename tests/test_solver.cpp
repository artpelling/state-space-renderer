#include "../src/state_space_system.h"
#include "../src/solver.h"
#include "../src/utils.h"

int main(int argc, char const *argv[])
{
    const char *filename = argv[1];
    MatrixData<double> matdata = load_matrices_from_hdf5<double>(filename);
    TestData<double> testdata = load_test_from_hdf5<double>(filename);

    auto buffer_size = testdata.buffer_size;
    auto n = matdata.n, m = matdata.m, p = matdata.p;

    std::cout << "buffer size: " << buffer_size << std::endl;

    std::cout << "n: " << n << std::endl;
    std::cout << "m: " << m << std::endl;
    std::cout << "p: " << p << std::endl;

    std::cout << "Type: double" << std::endl;

    double err;
    double *output, *dtout;

    output = (double *)calloc(p * buffer_size, sizeof(double)); // allocate
    dtout = testdata.y;
    StateSpaceSystem<double> system(matdata.A, matdata.B, matdata.C, matdata.D, n, m, p, matdata.matstruct);

    NativeSolver<double> dnat_solver(system);
    XGEMVSolver<double> dgemv_solver(system);
    XGEMMSolver<double> dgemm_solver(system);

    dnat_solver.set_buffer_size(buffer_size);
    dnat_solver.process(testdata.u, output);
    std::cout << "True out:" << std::endl;
    print_data(testdata.y, p, buffer_size);
    std::cout << "Calculated out:" << std::endl;
    print_data(output, p, buffer_size);
    std::cout << "Native Solver - " << l2err(output, dtout) << std::endl;

    dgemv_solver.set_buffer_size(buffer_size);
    dgemv_solver.process(testdata.u, output);
    std::cout << "True out:" << std::endl;
    print_data(testdata.y, p, buffer_size);
    std::cout << "Calculated out:" << std::endl;
    print_data(output, p, buffer_size);
    std::cout << "DGEMV Solver - " << l2err(output, dtout) << std::endl;

    dgemm_solver.set_buffer_size(buffer_size);
    dgemm_solver.process(testdata.u, output);
    std::cout << "True out:" << std::endl;
    print_data(testdata.y, p, buffer_size);
    std::cout << "Calculated out:" << std::endl;
    print_data(output, p, buffer_size);
    std::cout << "DGEMM Solver - " << l2err(output, dtout) << std::endl;

    /* dgemv_solver2.process(input.data<double>(), output, dataframes);
    std::cout << "True out:" << std::endl;
    print_data(true_output);
    std::cout << "Calculated out:" << std::endl;
    print_data(output, p, dataframes);
    std::cout << "DGEMV Solver - " << l2err(output, dtout) << std::endl; */

    free(output);
}
