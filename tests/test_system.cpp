#include "../src/state_space_system.h"
#include "../src/utils.h"
#include "hdf5.h"

template <typename T>
class NoProcess : public StateSpaceSystem<T>
{
public:
    NoProcess() : StateSpaceSystem<T>()
    {
    }

    NoProcess(int n, int m, int p, MatrixStructure matstruct) : StateSpaceSystem<T>(n, m, p, matstruct)
    {
    }

    NoProcess(T *A, T *B, T *C, T *D, int n, int m, int p, MatrixStructure matstruct) : StateSpaceSystem<T>(A, B, C, D, n, m, p, matstruct)
    {
    }
};

int main(int argc, char const *argv[])
{
    const char *filename = argv[1];
    MatrixData<double> matdata1 = load_matrices_from_hdf5<double>(filename);
    MatrixData<float> matdata2 = load_matrices_from_hdf5<float>(filename);

    // /* Default state space */

    std::cout << "- STL double zero states - " << std::endl;
    NoProcess<double> zero_state_double(2, 2, 1, General);
    zero_state_double.info();

    std::cout << "- STL float zero states - " << std::endl;
    NoProcess<float> zero_state_float(4, 4, 3, General);
    zero_state_float.info();

    std::cout << "- STL double HDF5 import - " << std::endl;
    NoProcess<double> nonempty_state_double(matdata1.A, matdata1.B, matdata1.C, matdata1.D, matdata1.n, matdata1.m, matdata1.p, matdata1.matstruct);
    nonempty_state_double.info();

    free(matdata1.A);
    free(matdata1.B);
    free(matdata1.C);
    free(matdata1.D);

    std::cout << "- STL float Cnpy import" << std::endl;
    NoProcess<float> nonempty_state_float(matdata2.A, matdata2.B, matdata2.C, matdata2.D, matdata2.n, matdata2.m, matdata2.p, matdata2.matstruct);
    nonempty_state_float.info();

    free(matdata2.A);
    free(matdata2.B);
    free(matdata2.C);
    free(matdata2.D);
}
