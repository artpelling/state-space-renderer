#include "../src/state_space_system.h"
#include "../src/solver.h"
#include "../src/utils.h"
#include "../src/renderer.h"

int main(int argc, char const *argv[])
{
    const char *filename = argv[1];
    using T = double;
    using sol = XGEMVSolver<T>;
    MatrixData<T> matdata = load_matrices_from_hdf5<T>(filename);
    auto n = matdata.n, m = matdata.m, p = matdata.p;

    std::cout << "n: " << n << std::endl;
    std::cout << "m: " << m << std::endl;
    std::cout << "p: " << p << std::endl;
    MatrixStructure matstruct = string_to_matstruct(argv[2]);

    StateSpaceSystem<T> system(matdata.A, matdata.B, matdata.C, matdata.D, n, m, p, matstruct);
    sol solver(system);
    JackRenderer<T> renderer(solver, m, p);

    // Atomic flag to indicate when rendering is done
    renderer.render();
    return 0;
}
