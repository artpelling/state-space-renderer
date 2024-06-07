#!/usr/bin/nv python3

import numpy as np
import scipy.linalg as spla
from pathlib import Path


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
    structure="dense",
    real=True,
    precision="double",
    test_input=False,
    test_input_length=1,
):
    assert structure in ("dense", "diagonal", "schur")
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
    if structure != "dense":
        if structure == "diagonal":
            print("Applying diagonal transformation ...")
            lam, T = spla.eig(A)
            if real:
                lam, T = spla.cdf2rdf(lam, T)
                A = np.zeros([3, n], dtype=dtype)
                A[0, 1:] = np.diag(lam, k=1)
                A[1] = np.diag(lam, k=0)
                A[2, :-1] = np.diag(lam, k=-1)
            else:
                A = lam
        elif structure == "schur":
            print("Applying Schur transformation ...")
            A, T = spla.schur(A, output="real" if real else "complex")
        print(f"Condition number of transformation: {np.linalg.cond(T)}")
        B, *_, svs = spla.lstsq(T, B)
        print(f"Singular values: {svs}")
        C = C @ T
    if test_input:
        u = np.random.randn(m, test_input_length)
        u = np.zeros((m, test_input_length))
        u[0, 0] = 1
        # u[0, 1] = 1
        x = np.zeros(n)

        y = np.zeros((p, test_input_length))

        for i in np.arange(test_input_length):
            y[:, i] = C @ x + D @ u[:, i]
            x = B @ u[:, i] + A @ x
        np.savez(
            Path(filename).with_suffix(".npz"),
            A=np.ascontiguousarray(A, dtype=dtype),
            B=np.ascontiguousarray(B, dtype=dtype),
            C=np.ascontiguousarray(C, dtype=dtype),
            D=np.ascontiguousarray(D, dtype=dtype),
            input=np.ascontiguousarray(u, dtype=dtype),
            output=np.ascontiguousarray(y, dtype=dtype),
        )
    else:
        np.savez(
            Path(filename).with_suffix(".npz"),
            A=np.ascontiguousarray(A, dtype=dtype),
            B=np.ascontiguousarray(B, dtype=dtype),
            C=np.ascontiguousarray(C, dtype=dtype),
            D=np.ascontiguousarray(D, dtype=dtype),
        )
