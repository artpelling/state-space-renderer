#include "../src/state_space_system.h"
#include "../src/solver.h"
#include "../src/utils.h"
#include "../src/renderer.h"

#include <signal.h>
#include <unistd.h>

volatile sig_atomic_t keep_running = 1;

void handle_sigint(int)
{
    keep_running = 0;
}

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
    PipewireRenderer<T> renderer(solver, m, p);

    // Set up SIGINT handler
    signal(SIGINT, handle_sigint);

    // Atomic flag to indicate when rendering is done
    renderer.render();

    std::cout << "Press Ctrl+C to stop..." << std::endl;

    while (keep_running)
    {
        pause(); // sleep until a signal is received
    }

    std::cout << "Exiting..." << std::endl;

    return 0;
}
