#include "../cnpy/cnpy.h"
#include "../src/state_space_system.h"
#include "../src/utils.h"

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
    std::string filename = argv[1];
    cnpy::NpyArray A_npy = cnpy::npz_load(filename, "A");
    cnpy::NpyArray B_npy = cnpy::npz_load(filename, "B");
    cnpy::NpyArray C_npy = cnpy::npz_load(filename, "C");
    cnpy::NpyArray D_npy = cnpy::npz_load(filename, "D");

    MatrixStructure matstruct = string_to_matstruct(argv[2]);

    auto n = A_npy.shape[0], m = B_npy.shape[1], p = C_npy.shape[0];

    /* Default state space */

    std::cout << "- STL double zero states - " << std::endl;
    NoProcess<double> zero_state_double(2, 2, 1, General);
    zero_state_double.info();

    std::cout << "- STL float zero states - " << std::endl;
    NoProcess<float> zero_state_float(4, 4, 3, General);
    zero_state_float.info();

    std::cout << "- STL double Cnpy import - " << std::endl;
    NoProcess<double> nonempty_state_double(A_npy.data<double>(), B_npy.data<double>(), C_npy.data<double>(), D_npy.data<double>(), B_npy.shape[0], B_npy.shape[1], C_npy.shape[0], matstruct);
    nonempty_state_double.info();

    std::cout << "- STL float Cnpy import" << std::endl;
    // Note that you have to be careful when casting datatypes using cnpy, since float in python is internally casted as double. As you can see here, the value does not coincide correctly.
    NoProcess<float> nonempty_state_float(A_npy.data<float>(), B_npy.data<float>(), C_npy.data<float>(), D_npy.data<float>(), B_npy.shape[0], B_npy.shape[1], C_npy.shape[0], matstruct);
    nonempty_state_float.info();
}
