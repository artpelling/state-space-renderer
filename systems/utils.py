#!/usr/bin/nv python3

import numpy as np
import scipy.linalg as spla
from pathlib import Path
import h5py as h5


def random_stable_system(n, p, m):
    # create random stable A
    lam = 1.8 * np.random.rand(n) - 0.9
    V = np.random.randn(n, n)
    A = np.linalg.inv(V) @ np.diag(lam) @ V
    B = np.random.randn(n, m)
    C = np.random.randn(p, n)
    D = np.random.randn(p, m)
    return A, B, C, D


def save_system(
    filename,
    A,
    B,
    C,
    D=None,
    structure="general",
    real=True,
    precision="double",
    test_input=False,
    test_input_length=1,
    format="h5",
):
    assert structure in (
        "triangular",
        "general",
        "diagonal",
        "tridiagonal",
        "fullhessenberg",
        "mixedhessenberg",
    )
    assert precision in ("single", "double")
    dtype = np.float64
    if precision == "single":
        dtype = np.float32 if real else np.complex64
    elif precision == "double":
        dtype = np.float64 if real else np.complex128
    assert all([mat.ndim == 2 for mat in (A, B, C)])
    n, p, m = len(A), C.shape[0], B.shape[1]
    assert A.shape == (n, n)
    assert B.shape == (n, m)
    assert C.shape == (p, n)
    D = np.zeros((C.shape[0], B.shape[1]), dtype=dtype) if D is None else D
    assert D.shape == (p, m)
    if structure != "general":
        if structure == "triangular":
            print("Applying Schur transformation ...")
            Ap, T = spla.schur(A, output="real" if real else "complex")
            # System storage, normal
            Ab = Ap - np.diag(np.diag(Ap, k=-1), k=-1)
            #####
            A = Ap
            A = A - np.diag(np.diag(A, k=-1), k=-1)
        elif structure == "diagonal":
            print("Applying diagonal transformation ...")
            lam, T = spla.eig(A)
            # System storage, banded
            Ab = lam
            #####
            A = np.diag(lam)
        elif structure == "tridiagonal":
            print("Applying diagonal transformation ...")
            lam, T = spla.eig(A)
            lam, T = spla.cdf2rdf(lam, T)
            # System storage, banded
            Ab = np.zeros([3, n], dtype=dtype)
            Ab[0, 1:] = np.diag(lam, k=1)
            Ab[1] = np.diag(lam, k=0)
            Ab[2, :-1] = np.diag(lam, k=-1)
            ######
            A = lam
        elif structure == "fullhessenberg":
            print("Applying Schur transformation ...")
            Ap, T = spla.schur(A, output="real" if real else "complex")
            # System storage, banded
            Ab = np.zeros([n + 1, n], dtype=dtype)
            for i in range(n):
                Ab[i, n - 1 - i :] = np.diag(Ap, k=n - 1 - i)
            Ab[n, :-1] = np.diag(Ap, k=-1)
            #######
            A = Ap
        elif structure == "mixedhessenberg":
            print("Applying Schur transformation ...")
            Ap, T = spla.schur(A, output="real" if real else "complex")
            # System storage, normal + banded
            A1 = Ap
            A2 = np.zeros([2, n], dtype=dtype)
            A2[1, :-1] = np.diag(Ap, k=-1)
            print(A1)
            print(A2)
            Ab = np.append(A1.flatten("F"), A2.flatten("F"))
            print(Ab)
            #######
            A = Ap
        print(f"Condition number of transformation: {np.linalg.cond(T)}")
        B, *_, svs = spla.lstsq(T, B)
        print(f"Singular values: {svs}")
        C = C @ T
    else:
        Ab = A
    if format == "npz":
        if test_input:
            u = np.random.randn(m, test_input_length)
            # u = np.zeros((m, test_input_length))
            # u[0, 0] = 1
            # u[0, 1] = 1
            x = np.zeros(n)

            y = np.zeros((p, test_input_length))

            for i in np.arange(test_input_length):
                y[:, i] = C @ x + D @ u[:, i]
                x = B @ u[:, i] + A @ x

            np.savez(
                Path(filename).with_suffix(".npz"),
                A=np.asfortranarray(Ab, dtype=dtype),
                B=(
                    np.ascontiguousarray(B, dtype=dtype)
                    if n / m < 1
                    else np.asfortranarray(B, dtype=dtype)
                ),
                C=np.asfortranarray(C, dtype=dtype),
                D=np.asfortranarray(D, dtype=dtype),
                input=np.asfortranarray(u, dtype=dtype),
                output=np.asfortranarray(y, dtype=dtype),
            )
        else:
            np.savez(
                Path(filename).with_suffix(".npz"),
                A=np.asfortranarray(Ab, dtype=dtype),
                B=(
                    np.ascontiguousarray(B, dtype=dtype)
                    if n / m < 1
                    else np.asfortranarray(B, dtype=dtype)
                ),
                C=np.asfortranarray(C, dtype=dtype),
                D=np.asfortranarray(D, dtype=dtype),
            )
    elif format == "h5":
        hf = h5.File(Path(filename).with_suffix(".h5"), "w")
        hf.create_dataset("A", data=Ab)
        hf.create_dataset("B", data=B)
        hf.create_dataset("C", data=C)
        hf.create_dataset("D", data=D)

        if test_input:
            u = np.random.randn(m, test_input_length)
            # u = np.zeros((m, test_input_length))
            # u[0, 0] = 1
            # u[0, 1] = 1
            x = np.zeros(n)

            y = np.zeros((p, test_input_length))

            for i in np.arange(test_input_length):
                y[:, i] = C @ x + D @ u[:, i]
                x = B @ u[:, i] + A @ x
            hf.create_dataset("u", data=u)
            hf.create_dataset("y", data=y)
        hf.close()
