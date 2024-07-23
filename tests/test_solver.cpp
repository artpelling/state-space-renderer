#include "../cnpy/cnpy.h"
#include "../src/state_space_system.h"
#include "../src/solver.h"
#include "../src/utils.h"

int main(int argc, char const *argv[])
{
    std::string filename = argv[1];
    cnpy::NpyArray A_npy = cnpy::npz_load(filename, "A");
    cnpy::NpyArray B_npy = cnpy::npz_load(filename, "B");
    cnpy::NpyArray C_npy = cnpy::npz_load(filename, "C");
    cnpy::NpyArray D_npy = cnpy::npz_load(filename, "D");
    cnpy::NpyArray input = cnpy::npz_load(filename, "input");
    cnpy::NpyArray true_output = cnpy::npz_load(filename, "output");
    auto buffer_size = input.shape[1];
    auto n = A_npy.shape[0], m = B_npy.shape[1], p = C_npy.shape[0];

    MatrixStructure matstruct = string_to_matstruct(argv[2]);

    std::cout << "buffer size: " << buffer_size << std::endl;
    std::cout << "n: " << n << std::endl;
    std::cout << "m: " << m << std::endl;
    std::cout << "p: " << p << std::endl;

    std::cout << "Type: double" << std::endl;

    double err;
    double *output, *dtout;

    output = (double *)calloc(p * buffer_size, sizeof(double)); // allocate
    dtout = true_output.data<double>();
    StateSpaceSystem<double> system(A_npy.data<double>(), B_npy.data<double>(), C_npy.data<double>(), D_npy.data<double>(), A_npy.shape[0], B_npy.shape[1], C_npy.shape[0], matstruct);
    NativeSolver<double> dnat_solver(system);
    XGEMVSolver<double> dgemv_solver(system);
    XGEMMSolver<double> dgemm_solver(system);

    dnat_solver.set_buffer_size(buffer_size);
    dnat_solver.process(input.data<double>(), output);
    std::cout << "True out:" << std::endl;
    print_data(true_output);
    std::cout << "Calculated out:" << std::endl;
    print_data(output, p, buffer_size);
    std::cout << "Native Solver - " << l2err(output, dtout) << std::endl;

    dgemv_solver.set_buffer_size(buffer_size);
    dgemv_solver.process(input.data<double>(), output);
    std::cout << "True out:" << std::endl;
    print_data(true_output);
    std::cout << "Calculated out:" << std::endl;
    print_data(output, p, buffer_size);
    std::cout << "DGEMV Solver - " << l2err(output, dtout) << std::endl;

    dgemm_solver.set_buffer_size(buffer_size);
    dgemm_solver.process(input.data<double>(), output);
    std::cout << "True out:" << std::endl;
    print_data(true_output);
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
