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

    NoProcess(int n, int m, int p) : StateSpaceSystem<T>(n, m, p)
    {
    }

    NoProcess(T *A, T *B, T *C, T *D, int n, int m, int p, MatrixStructure General) : StateSpaceSystem<T>(A, B, C, D, n, m, p, General)
    {
    }
};

MatrixStructure stringToMatStruct(const std::string &matstruct_str)
{
    if (matstruct_str == "General")
    {
        return General;
    }
    else if (matstruct_str == "Triangular")
    {
        return Triangular;
    }
    else if (matstruct_str == "Diagonal")
    {
        return Diagonal;
    }
    else if (matstruct_str == "Tridiagonal")
    {
        return Tridiagonal;
    }
    else if (matstruct_str == "MixedHessenberg")
    {
        return MixedHessenberg;
    }
    else if (matstruct_str == "FullHessenberg")
    {
        return FullHessenberg;
    }
    else
    {
        throw std::invalid_argument("Not valid matrix structure!");
    }
}

int main(int argc, char const *argv[])
{
    std::string filename = argv[1];
    cnpy::NpyArray A_npy = cnpy::npz_load(filename, "A");
    cnpy::NpyArray B_npy = cnpy::npz_load(filename, "B");
    cnpy::NpyArray C_npy = cnpy::npz_load(filename, "C");
    cnpy::NpyArray D_npy = cnpy::npz_load(filename, "D");

    MatrixStructure matstruct = stringToMatStruct(argv[2]);

    auto n = A_npy.shape[0], m = B_npy.shape[1], p = C_npy.shape[0];

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

    /* Default state space */

    std::cout << "- STL double zero states - " << std::endl;
    NoProcess<double> zero_state_double(2, 2, 1);
    zero_state_double.info();

    // std::cout << "- STL float zero states - " << std::endl;
    //  NoProcess<float> zero_state_float(4, 4, 3);
    //  zero_state_float.info();

    std::cout << "- STL double Cnpy import - " << std::endl;
    NoProcess<double> nonempty_state_double(conversion_function(A_npy.data<double>()), B_npy.data<double>(), C_npy.data<double>(), D_npy.data<double>(), B_npy.shape[0], B_npy.shape[1], C_npy.shape[0], matstruct);
    nonempty_state_double.info();

    // std::cout << "- STL float Cnpy import - *Bug in cnpy* " << std::endl;
    // Note that you have to be careful when casting datatypes using cnpy, since float in python is internally casted as double. As you can see here, the value does not coincide correctly.
    // NoProcess<float> nonempty_state_float(conversion_function(A_npy.data<float>()), B_npy.data<float>(), C_npy.data<float>(), D_npy.data<float>(), B_npy.shape[0], B_npy.shape[1], C_npy.shape[0], matstruct);
    // nonempty_state_float.info();
}
