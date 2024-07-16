#!/usr/bin/nv python3

from itertools import product
import numpy as np
from pathlib import Path
from utils import random_stable_system, save_system

precisions = ("single", "double")
structures = (
    "triangular",
    "general",
    "diagonal",
    "tridiagonal",
    "fullhessenberg",
    "mixedhessenberg",
)

if __name__ == "__main__":
    import argparse

    np.random.seed(0)

    parser = argparse.ArgumentParser(
        description="Generate a random system with a test input and calculate a reference output."
    )
    parser.add_argument(
        "--shape",
        metavar="shape",
        type=int,
        nargs=3,
        default=[10, 2, 5],
        help="State dimension, number of outputs and number of inputs of the system.",
    )

    parser.add_argument(
        "--input_length",
        metavar="input_length",
        type=int,
        default=16,
        help="Length of the input signal.",
    )

    args = parser.parse_args()

    path = Path("systems")

    n, p, m = args.shape
    matrices = random_stable_system(n, p, m)
    for precision, structure in product(precisions, structures):
        filename = path / f"n{n}p{p}m{m}d{args.input_length}{precision}{structure}"
        if not filename.with_suffix(".npz").exists():
            save_system(
                filename,
                *matrices,
                structure=structure,
                precision=precision,
                real=True,
                test_input=True,
                test_input_length=args.input_length,
            )

        d = np.load(filename.with_suffix(".npz"))
        A, B, C, D, input, output = (
            d["A"],
            d["B"],
            d["C"],
            d["D"],
            d["input"],
            d["output"],
        )
        print(f"A: {A.shape} - {A.dtype}")
        print(A)
        print(f"B: {B.shape} - {B.dtype}")
        print(B)
        print(f"C: {C.shape} - {C.dtype}")
        print(C)
        print(f"D: {D.shape} - {D.dtype}")
        print(D)
        print(f"input: {input.shape} - {input.dtype}")
        print(input)
        print(f"output: {output.shape} - {output.dtype}")
        print(output)
