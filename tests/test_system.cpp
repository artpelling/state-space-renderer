#include "../cnpy/cnpy.h"
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

struct dataset_info
{
    hid_t id;
    hsize_t shape[2];
};

dataset_info GetMatrixDatasetInfo(hid_t file_id, const char *matrix_char)
{
    hid_t dataspace_id;
    dataset_info dinfo;

    dinfo.id = H5Dopen2(file_id, matrix_char, H5P_DEFAULT);
    dataspace_id = H5Dget_space(dinfo.id);

    int rank = H5Sget_simple_extent_ndims(dataspace_id);
    H5Sget_simple_extent_dims(dataspace_id, dinfo.shape, NULL);

    return dinfo;
}

int main(int argc, char const *argv[])
{
    const char *filename = argv[1];
    MatrixStructure matstruct = string_to_matstruct(argv[2]);
    hid_t file_id;
    dataset_info A, B, C, D;

    herr_t status;

    file_id = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT);

    A = GetMatrixDatasetInfo(file_id, "A");
    B = GetMatrixDatasetInfo(file_id, "B");
    C = GetMatrixDatasetInfo(file_id, "C");
    D = GetMatrixDatasetInfo(file_id, "D");

    auto n = A.shape[0], m = B.shape[1], p = C.shape[0];

    double *A_data = (double *)malloc(A.shape[0] * A.shape[1] * sizeof(double));
    double *B_data = (double *)malloc(B.shape[0] * B.shape[1] * sizeof(double));
    double *C_data = (double *)malloc(C.shape[0] * C.shape[1] * sizeof(double));
    double *D_data = (double *)malloc(D.shape[0] * D.shape[1] * sizeof(double));

    status = H5Dread(A.id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, A_data);
    status = H5Dread(B.id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, B_data);
    status = H5Dread(C.id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, C_data);
    status = H5Dread(D.id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, D_data);

    // /* Default state space */

    // std::cout << "- STL double zero states - " << std::endl;
    // NoProcess<double> zero_state_double(2, 2, 1, General);
    // zero_state_double.info();

    // std::cout << "- STL float zero states - " << std::endl;
    // NoProcess<float> zero_state_float(4, 4, 3, General);
    // zero_state_float.info();

    std::cout << "- STL double Cnpy import - " << std::endl;
    NoProcess<double> nonempty_state_double(A_data, B_data, C_data, D_data, B.shape[0], B.shape[1], C.shape[0], matstruct);
    nonempty_state_double.info();

    free(A_data);
    free(B_data);
    free(C_data);
    free(D_data);
    H5Fclose(file_id);

    // std::cout << "- STL float Cnpy import" << std::endl;
    // // Note that you have to be careful when casting datatypes using cnpy, since float in python is internally casted as double. As you can see here, the value does not coincide correctly.
    // NoProcess<float> nonempty_state_float(A_npy.data<float>(), B_npy.data<float>(), C_npy.data<float>(), D_npy.data<float>(), B_npy.shape[0], B_npy.shape[1], C_npy.shape[0], matstruct);
    // nonempty_state_float.info();
}
