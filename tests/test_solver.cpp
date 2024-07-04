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
    auto dataframes = input.shape[1];
    auto n = A_npy.shape[0], m = B_npy.shape[1], p = C_npy.shape[0];

    MatrixStructure matstruct = string_to_matstruct(argv[2]);

    auto conversion_function = [matstruct, n](auto *val)
    {
        switch (matstruct)
        {
        case General:
        case Triangular:
            return val;
            break;

        case Diagonal:
            return general_to_diagonal(val, n);
            break;

        case Tridiagonal:
            return general_to_tridiagonal(val, n);
            break;

        case FullHessenberg:
            return general_to_hessenberg(val, n, true);
            break;

        case MixedHessenberg:
            return general_to_hessenberg(val, n, false);
            break;

        default:
            std::invalid_argument("Invalid structure!");
            break;
        }
    };
    std::shared_ptr<double> A(conversion_function(A_npy.data<double>()));

    std::cout << "dataframes: " << dataframes << std::endl;
    std::cout << "n: " << n << std::endl;
    std::cout << "m: " << m << std::endl;
    std::cout << "p: " << p << std::endl;

    std::cout << "Type: double" << std::endl;

    double err;
    double *output, *dtout;

    output = (double *)calloc(p * dataframes, sizeof(double)); // allocate
    dtout = true_output.data<double>();
    StateSpaceSystem<double> system(A.get(), B_npy.data<double>(), C_npy.data<double>(), D_npy.data<double>(), A_npy.shape[0], B_npy.shape[1], C_npy.shape[0], matstruct);
    NativeSolver<double> dnat_solver(system, dataframes);
    XGEMVSolver<double> dgemv_solver(system, dataframes);
    XGEMMSolver<double> dgemm_solver(system, dataframes);

    dnat_solver.process(input.data<double>(), output);
    std::cout << "True out:" << std::endl;
    print_data(true_output);
    std::cout << "Calculated out:" << std::endl;
    print_data(output, p, dataframes);
    std::cout << "Native Solver - " << l2err(output, dtout) << std::endl;

    dgemv_solver.process(input.data<double>(), output);
    std::cout << "True out:" << std::endl;
    print_data(true_output);
    std::cout << "Calculated out:" << std::endl;
    print_data(output, p, dataframes);
    std::cout << "DGEMV Solver - " << l2err(output, dtout) << std::endl;

    dgemm_solver.process(input.data<double>(), output);
    std::cout << "True out:" << std::endl;
    print_data(true_output);
    std::cout << "Calculated out:" << std::endl;
    print_data(output, p, dataframes);
    std::cout << "DGEMM Solver - " << l2err(output, dtout) << std::endl;

    /* dgemv_solver2.process(input.data<double>(), output, dataframes);
    std::cout << "True out:" << std::endl;
    print_data(true_output);
    std::cout << "Calculated out:" << std::endl;
    print_data(output, p, dataframes);
    std::cout << "DGEMV Solver - " << l2err(output, dtout) << std::endl; */

    free(output);
    // free(A);
}
