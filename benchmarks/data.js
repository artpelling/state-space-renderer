window.BENCHMARK_DATA = {
  "lastUpdate": 1716308496616,
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
          "id": "0fd2744936add56afaa0b3305bf78e68bd0de324",
          "message": "use ms",
          "timestamp": "2024-05-21T18:13:11+02:00",
          "tree_id": "d9544faea1c811a7792246f1c0d80d7f72a89812",
          "url": "https://github.com/artpelling/state-space-renderer/commit/0fd2744936add56afaa0b3305bf78e68bd0de324"
        },
        "date": 1716308085592,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_SinglePNativeSolver/10/2/5/128",
            "value": 0.062480898315603606,
            "unit": "ms/iter",
            "extra": "iterations: 11280\ncpu: 0.0624776620567376 ms\nthreads: 1"
          },
          {
            "name": "BM_SinglePNativeSolver/100/2/5/128",
            "value": 4.805283657534247,
            "unit": "ms/iter",
            "extra": "iterations: 146\ncpu: 4.80532223287671 ms\nthreads: 1"
          },
          {
            "name": "BM_SinglePNativeSolver/1000/2/5/128",
            "value": 476.3150219999943,
            "unit": "ms/iter",
            "extra": "iterations: 2\ncpu: 476.29552250000006 ms\nthreads: 1"
          },
          {
            "name": "BM_DoublePNativeSolver/10/2/5/128",
            "value": 0.06170563915260102,
            "unit": "ms/iter",
            "extra": "iterations: 11376\ncpu: 0.06170499147327711 ms\nthreads: 1"
          },
          {
            "name": "BM_DoublePNativeSolver/100/2/5/128",
            "value": 4.812099465753313,
            "unit": "ms/iter",
            "extra": "iterations: 146\ncpu: 4.811855109589042 ms\nthreads: 1"
          },
          {
            "name": "BM_DoublePNativeSolver/1000/2/5/128",
            "value": 476.70661399999403,
            "unit": "ms/iter",
            "extra": "iterations: 2\ncpu: 476.6864570000005 ms\nthreads: 1"
          },
          {
            "name": "BM_FGEMVSolver/10/2/5/128",
            "value": 0.0215477277409403,
            "unit": "ms/iter",
            "extra": "iterations: 32425\ncpu: 0.021546735543562064 ms\nthreads: 1"
          },
          {
            "name": "BM_FGEMVSolver/100/2/5/128",
            "value": 0.9248543665835487,
            "unit": "ms/iter",
            "extra": "iterations: 802\ncpu: 0.9247746783042405 ms\nthreads: 1"
          },
          {
            "name": "BM_FGEMVSolver/1000/2/5/128",
            "value": 115.82745550000102,
            "unit": "ms/iter",
            "extra": "iterations: 6\ncpu: 115.8250573333334 ms\nthreads: 1"
          },
          {
            "name": "BM_DGEMVSolver/10/2/5/128",
            "value": 0.021201775242821096,
            "unit": "ms/iter",
            "extra": "iterations: 33049\ncpu: 0.021200915307573606 ms\nthreads: 1"
          },
          {
            "name": "BM_DGEMVSolver/100/2/5/128",
            "value": 0.867105506815361,
            "unit": "ms/iter",
            "extra": "iterations: 807\ncpu: 0.8670436369268903 ms\nthreads: 1"
          },
          {
            "name": "BM_DGEMVSolver/1000/2/5/128",
            "value": 115.83497499999851,
            "unit": "ms/iter",
            "extra": "iterations: 6\ncpu: 115.83398733333331 ms\nthreads: 1"
          }
        ]
      },
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
          "id": "b05082a4ed4cf145924edda26f2c54399ec24434",
          "message": "remove assertions",
          "timestamp": "2024-05-21T18:19:57+02:00",
          "tree_id": "00efbdc665c467a997124840a5bbb0dc53c6c31d",
          "url": "https://github.com/artpelling/state-space-renderer/commit/b05082a4ed4cf145924edda26f2c54399ec24434"
        },
        "date": 1716308496138,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_SinglePNativeSolver/10/2/5/128",
            "value": 0.06506348779360065,
            "unit": "ms/iter",
            "extra": "iterations: 10814\ncpu: 0.06506158562973924 ms\nthreads: 1"
          },
          {
            "name": "BM_SinglePNativeSolver/100/2/5/128",
            "value": 4.895628993006999,
            "unit": "ms/iter",
            "extra": "iterations: 143\ncpu: 4.89543204195804 ms\nthreads: 1"
          },
          {
            "name": "BM_SinglePNativeSolver/1000/2/5/128",
            "value": 477.3753614999947,
            "unit": "ms/iter",
            "extra": "iterations: 2\ncpu: 477.35232950000017 ms\nthreads: 1"
          },
          {
            "name": "BM_DoublePNativeSolver/10/2/5/128",
            "value": 0.06475281712278161,
            "unit": "ms/iter",
            "extra": "iterations: 10816\ncpu: 0.06474813202662721 ms\nthreads: 1"
          },
          {
            "name": "BM_DoublePNativeSolver/100/2/5/128",
            "value": 4.899746468531369,
            "unit": "ms/iter",
            "extra": "iterations: 143\ncpu: 4.899484937062937 ms\nthreads: 1"
          },
          {
            "name": "BM_DoublePNativeSolver/1000/2/5/128",
            "value": 478.659605499999,
            "unit": "ms/iter",
            "extra": "iterations: 2\ncpu: 478.6370454999998 ms\nthreads: 1"
          },
          {
            "name": "BM_FGEMVSolver/10/2/5/128",
            "value": 0.021728236033129993,
            "unit": "ms/iter",
            "extra": "iterations: 32237\ncpu: 0.021726698483109458 ms\nthreads: 1"
          },
          {
            "name": "BM_FGEMVSolver/100/2/5/128",
            "value": 0.8722000049875077,
            "unit": "ms/iter",
            "extra": "iterations: 802\ncpu: 0.8721883004987535 ms\nthreads: 1"
          },
          {
            "name": "BM_FGEMVSolver/1000/2/5/128",
            "value": 115.87384400000171,
            "unit": "ms/iter",
            "extra": "iterations: 6\ncpu: 115.86307900000007 ms\nthreads: 1"
          },
          {
            "name": "BM_DGEMVSolver/10/2/5/128",
            "value": 0.02114354684662815,
            "unit": "ms/iter",
            "extra": "iterations: 33044\ncpu: 0.021141768066820013 ms\nthreads: 1"
          },
          {
            "name": "BM_DGEMVSolver/100/2/5/128",
            "value": 0.8917723762375941,
            "unit": "ms/iter",
            "extra": "iterations: 808\ncpu: 0.8917305495049513 ms\nthreads: 1"
          },
          {
            "name": "BM_DGEMVSolver/1000/2/5/128",
            "value": 115.8330660000028,
            "unit": "ms/iter",
            "extra": "iterations: 6\ncpu: 115.83120699999996 ms\nthreads: 1"
          }
        ]
      }
    ]
  }
}