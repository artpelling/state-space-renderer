#!/usr/bin/env python3

from itertools import product
from pathlib import Path
from utils import random_stable_system, save_system


ns = (10, 100, 1000)
ps = (2,)
ms = (5,)
precisions = ('single', 'double')
input_lengths = (128,)


if __name__ == '__main__':
    print('Creating benchmark systems...')
    path = Path('systems/benchmark')
    for n, p, m, precision, input_length in product(ns, ps, ms, precisions, input_lengths):
        print(n, p, m, precision, input_length)
        filename = path / f"n{n}p{p}m{m}d{input_length}{precision}"
        if not filename.with_suffix('.npz').exists():
            matrices = random_stable_system(n, p, m)
            save_system(
                filename,
                *matrices,
                structure="dense",
                precision=precision,
                real=True,
                test_input=True,
                test_input_length=input_length,
            )
