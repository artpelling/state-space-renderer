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

    auto dataframes = input.shape[1];
    auto n = A_npy.shape[0], m = B_npy.shape[1], p = C_npy.shape[0];
    using T = double;
    using sol = XGEMVSolver<T>;

    std::cout << "dataframes: " << dataframes << std::endl;
    std::cout << "n: " << n << std::endl;
    std::cout << "m: " << m << std::endl;
    std::cout << "p: " << p << std::endl;

    StateSpaceSystem<T> system(A_npy.data<T>(), B_npy.data<T>(), C_npy.data<T>(), D_npy.data<T>(), A_npy.shape[0], B_npy.shape[1], C_npy.shape[0]);
    sol solver(system, dataframes);
    RandomRenderer<T> rng_renderer(solver, 30);

    // Atomic flag to indicate when rendering is done
    std::atomic<bool> rendering_done{false};

    // Register the callback function
    rng_renderer.register_output_callback([&](T *output)
                                          {
        print_data(output, p, dataframes);
        free(output); });

    // Start the rendering process in a separate thread
    std::thread render_thread([&]()
                              {
        rng_renderer.render();
        rendering_done.store(true); });

    // Ensure the rendering thread has finished
    render_thread.join();

    return 0;
}