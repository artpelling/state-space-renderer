window.BENCHMARK_DATA = {
  "lastUpdate": 1716305795658,
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
          "id": "890bd73358cd8da6d4975d4e0cbe6b6d09588815",
          "message": "newline",
          "timestamp": "2024-05-21T17:34:55+02:00",
          "tree_id": "730af4d7fc2f1dee14bbea75a08cfd997c141784",
          "url": "https://github.com/artpelling/state-space-renderer/commit/890bd73358cd8da6d4975d4e0cbe6b6d09588815"
        },
        "date": 1716305795327,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_SinglePNativeSolver/10/2/5/128",
            "value": 62144.48236662954,
            "unit": "ns/iter",
            "extra": "iterations: 11172\ncpu: 62141.89384174722 ns\nthreads: 1"
          },
          {
            "name": "BM_SinglePNativeSolver/100/2/5/128",
            "value": 4806127.013698767,
            "unit": "ns/iter",
            "extra": "iterations: 146\ncpu: 4805855.500000001 ns\nthreads: 1"
          },
          {
            "name": "BM_SinglePNativeSolver/1000/2/5/128",
            "value": 481723039.000002,
            "unit": "ns/iter",
            "extra": "iterations: 2\ncpu: 481695613.99999994 ns\nthreads: 1"
          },
          {
            "name": "BM_DoublePNativeSolver/10/2/5/128",
            "value": 61812.00114557467,
            "unit": "ns/iter",
            "extra": "iterations: 11348\ncpu: 61807.35257314066 ns\nthreads: 1"
          },
          {
            "name": "BM_DoublePNativeSolver/100/2/5/128",
            "value": 4805062.801369864,
            "unit": "ns/iter",
            "extra": "iterations: 146\ncpu: 4805042.671232873 ns\nthreads: 1"
          },
          {
            "name": "BM_DoublePNativeSolver/1000/2/5/128",
            "value": 478177198.4999921,
            "unit": "ns/iter",
            "extra": "iterations: 2\ncpu: 478146940.5000003 ns\nthreads: 1"
          },
          {
            "name": "BM_FGEMVSolver/10/2/5/128",
            "value": 21560.574292235313,
            "unit": "ns/iter",
            "extra": "iterations: 32426\ncpu: 21559.351076296804 ns\nthreads: 1"
          },
          {
            "name": "BM_FGEMVSolver/100/2/5/128",
            "value": 872408.1537500083,
            "unit": "ns/iter",
            "extra": "iterations: 800\ncpu: 872372.0087500009 ns\nthreads: 1"
          },
          {
            "name": "BM_FGEMVSolver/1000/2/5/128",
            "value": 117070136.33333455,
            "unit": "ns/iter",
            "extra": "iterations: 6\ncpu: 117069777.49999996 ns\nthreads: 1"
          },
          {
            "name": "BM_DGEMVSolver/10/2/5/128",
            "value": 21192.80573470161,
            "unit": "ns/iter",
            "extra": "iterations: 33027\ncpu: 21191.13764495712 ns\nthreads: 1"
          },
          {
            "name": "BM_DGEMVSolver/100/2/5/128",
            "value": 864594.3159851803,
            "unit": "ns/iter",
            "extra": "iterations: 807\ncpu: 864555.2812887226 ns\nthreads: 1"
          },
          {
            "name": "BM_DGEMVSolver/1000/2/5/128",
            "value": 115874652.66667134,
            "unit": "ns/iter",
            "extra": "iterations: 6\ncpu: 115864634.49999999 ns\nthreads: 1"
          }
        ]
      }
    ]
  }
}