#include "../cnpy/cnpy.h"
#include "../src/state_space_system.h"
#include "../src/solver.h"
#include "../src/utils.h"
#include "../src/renderer.h"

int main(int argc, char const *argv[])
{
    std::string filename = argv[1];
    cnpy::NpyArray A_npy = cnpy::npz_load(filename, "A");
    cnpy::NpyArray B_npy = cnpy::npz_load(filename, "B");
    cnpy::NpyArray C_npy = cnpy::npz_load(filename, "C");
    cnpy::NpyArray D_npy = cnpy::npz_load(filename, "D");
    cnpy::NpyArray input = cnpy::npz_load(filename, "input");

    auto buffer_size = input.shape[1];
    auto n = A_npy.shape[0], m = B_npy.shape[1], p = C_npy.shape[0];
    using T = double;
    using sol = XGEMVSolver<T>;

    std::cout << "buffer_size: " << buffer_size << std::endl;
    std::cout << "n: " << n << std::endl;
    std::cout << "m: " << m << std::endl;
    std::cout << "p: " << p << std::endl;

    StateSpaceSystem<T> system(A_npy.data<T>(), B_npy.data<T>(), C_npy.data<T>(), D_npy.data<T>(), A_npy.shape[0], B_npy.shape[1], C_npy.shape[0]);
    sol solver(system);
    JackRenderer<T> renderer(solver, m, p);

    // Atomic flag to indicate when rendering is done
    renderer.render();
    return 0;
}
