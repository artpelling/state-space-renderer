window.BENCHMARK_DATA = {
  "lastUpdate": 1716306855272,
  "repoUrl": "https://github.com/artpelling/state-space-renderer",
  "entries": {
    "C++ Benchmark": [
      {
        "commit": {
          "author": {
            "email": "a.pelling@tu-berlin.de",
            "name": "Art Pelling",
            "username": "artpelling"
          },
          "committer": {
            "email": "a.pelling@tu-berlin.de",
            "name": "Art Pelling",
            "username": "artpelling"
          },
          "distinct": true,
          "id": "71dd62cbc0409fb979d5e03e7cd44334795dab74",
          "message": "change dirname",
          "timestamp": "2024-05-21T17:52:28+02:00",
          "tree_id": "a7932ff2819737dc100b98e070a3964d27b3b669",
          "url": "https://github.com/artpelling/state-space-renderer/commit/71dd62cbc0409fb979d5e03e7cd44334795dab74"
        },
        "date": 1716306854789,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_SinglePNativeSolver/10/2/5/128",
            "value": 62852.875409835826,
            "unit": "ns/iter",
            "extra": "iterations: 11285\ncpu: 62849.794771821 ns\nthreads: 1"
          },
          {
            "name": "BM_SinglePNativeSolver/100/2/5/128",
            "value": 4809352.602739663,
            "unit": "ns/iter",
            "extra": "iterations: 146\ncpu: 4809116.979452056 ns\nthreads: 1"
          },
          {
            "name": "BM_SinglePNativeSolver/1000/2/5/128",
            "value": 476537508.99999636,
            "unit": "ns/iter",
            "extra": "iterations: 2\ncpu: 476524836.99999994 ns\nthreads: 1"
          },
          {
            "name": "BM_DoublePNativeSolver/10/2/5/128",
            "value": 61607.45534690963,
            "unit": "ns/iter",
            "extra": "iterations: 11343\ncpu: 61607.17535043636 ns\nthreads: 1"
          },
          {
            "name": "BM_DoublePNativeSolver/100/2/5/128",
            "value": 4806210.321917806,
            "unit": "ns/iter",
            "extra": "iterations: 146\ncpu: 4806132.363013694 ns\nthreads: 1"
          },
          {
            "name": "BM_DoublePNativeSolver/1000/2/5/128",
            "value": 478491579.00000703,
            "unit": "ns/iter",
            "extra": "iterations: 2\ncpu: 478479912.00000006 ns\nthreads: 1"
          },
          {
            "name": "BM_FGEMVSolver/10/2/5/128",
            "value": 21621.87894282872,
            "unit": "ns/iter",
            "extra": "iterations: 32464\ncpu: 21621.260226712675 ns\nthreads: 1"
          },
          {
            "name": "BM_FGEMVSolver/100/2/5/128",
            "value": 872946.3892365353,
            "unit": "ns/iter",
            "extra": "iterations: 799\ncpu: 872929.6007509397 ns\nthreads: 1"
          },
          {
            "name": "BM_FGEMVSolver/1000/2/5/128",
            "value": 115798720.00000079,
            "unit": "ns/iter",
            "extra": "iterations: 6\ncpu: 115796329.99999993 ns\nthreads: 1"
          },
          {
            "name": "BM_DGEMVSolver/10/2/5/128",
            "value": 21279.004895062095,
            "unit": "ns/iter",
            "extra": "iterations: 32686\ncpu: 21278.90635134312 ns\nthreads: 1"
          },
          {
            "name": "BM_DGEMVSolver/100/2/5/128",
            "value": 868103.0420792021,
            "unit": "ns/iter",
            "extra": "iterations: 808\ncpu: 868080.6967821791 ns\nthreads: 1"
          },
          {
            "name": "BM_DGEMVSolver/1000/2/5/128",
            "value": 115974390.333335,
            "unit": "ns/iter",
            "extra": "iterations: 6\ncpu: 115972739.99999984 ns\nthreads: 1"
          }
        ]
      }
    ]
  }
}