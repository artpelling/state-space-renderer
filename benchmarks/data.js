window.BENCHMARK_DATA = {
  "lastUpdate": 1716371710243,
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
            "value": 2048.6261153520268,
            "unit": "kHz",
            "extra": "iterations: 11280\ncpu: 0.0624776620567376 ms\nthreads: 1"
          },
          {
            "name": "BM_SinglePNativeSolver/100/2/5/128",
            "value": 26.637345289555938,
            "unit": "kHz",
            "extra": "iterations: 146\ncpu: 4.80532223287671 ms\nthreads: 1"
          },
          {
            "name": "BM_SinglePNativeSolver/1000/2/5/128",
            "value": 0.26872971476427954,
            "unit": "kHz",
            "extra": "iterations: 2\ncpu: 476.29552250000006 ms\nthreads: 1"
          },
          {
            "name": "BM_DoublePNativeSolver/10/2/5/128",
            "value": 2074.364705686782,
            "unit": "kHz",
            "extra": "iterations: 11376\ncpu: 0.06170499147327711 ms\nthreads: 1"
          },
          {
            "name": "BM_DoublePNativeSolver/100/2/5/128",
            "value": 26.599616427496716,
            "unit": "kHz",
            "extra": "iterations: 146\ncpu: 4.811855109589042 ms\nthreads: 1"
          },
          {
            "name": "BM_DoublePNativeSolver/1000/2/5/128",
            "value": 0.26850896597797486,
            "unit": "kHz",
            "extra": "iterations: 2\ncpu: 476.6864570000005 ms\nthreads: 1"
          },
          {
            "name": "BM_FGEMVSolver/10/2/5/128",
            "value": 5940.301526865975,
            "unit": "kHz",
            "extra": "iterations: 32425\ncpu: 0.021546735543562064 ms\nthreads: 1"
          },
          {
            "name": "BM_FGEMVSolver/100/2/5/128",
            "value": 138.40016831280954,
            "unit": "kHz",
            "extra": "iterations: 802\ncpu: 0.9247746783042405 ms\nthreads: 1"
          },
          {
            "name": "BM_FGEMVSolver/1000/2/5/128",
            "value": 1.105092047886685,
            "unit": "kHz",
            "extra": "iterations: 6\ncpu: 115.8250573333334 ms\nthreads: 1"
          },
          {
            "name": "BM_DGEMVSolver/10/2/5/128",
            "value": 6037.230304256748,
            "unit": "kHz",
            "extra": "iterations: 33049\ncpu: 0.021200915307573606 ms\nthreads: 1"
          },
          {
            "name": "BM_DGEMVSolver/100/2/5/128",
            "value": 147.6175609472354,
            "unit": "kHz",
            "extra": "iterations: 807\ncpu: 0.8670436369268903 ms\nthreads: 1"
          },
          {
            "name": "BM_DGEMVSolver/1000/2/5/128",
            "value": 1.10502031014382,
            "unit": "kHz",
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
            "value": 1967.3092288881185,
            "unit": "kHz",
            "extra": "iterations: 10814\ncpu: 0.06506158562973924 ms\nthreads: 1"
          },
          {
            "name": "BM_SinglePNativeSolver/100/2/5/128",
            "value": 26.14577211280459,
            "unit": "kHz",
            "extra": "iterations: 143\ncpu: 4.89543204195804 ms\nthreads: 1"
          },
          {
            "name": "BM_SinglePNativeSolver/1000/2/5/128",
            "value": 0.26813281606700895,
            "unit": "kHz",
            "extra": "iterations: 2\ncpu: 477.35232950000017 ms\nthreads: 1"
          },
          {
            "name": "BM_DoublePNativeSolver/10/2/5/128",
            "value": 1976.747973100409,
            "unit": "kHz",
            "extra": "iterations: 10816\ncpu: 0.06474813202662721 ms\nthreads: 1"
          },
          {
            "name": "BM_DoublePNativeSolver/100/2/5/128",
            "value": 26.12380065419308,
            "unit": "kHz",
            "extra": "iterations: 143\ncpu: 4.899484937062937 ms\nthreads: 1"
          },
          {
            "name": "BM_DoublePNativeSolver/1000/2/5/128",
            "value": 0.2674134155655219,
            "unit": "kHz",
            "extra": "iterations: 2\ncpu: 478.6370454999998 ms\nthreads: 1"
          },
          {
            "name": "BM_FGEMVSolver/10/2/5/128",
            "value": 5890.95220637482,
            "unit": "kHz",
            "extra": "iterations: 32237\ncpu: 0.021726698483109458 ms\nthreads: 1"
          },
          {
            "name": "BM_FGEMVSolver/100/2/5/128",
            "value": 146.75533050682947,
            "unit": "kHz",
            "extra": "iterations: 802\ncpu: 0.8721883004987535 ms\nthreads: 1"
          },
          {
            "name": "BM_FGEMVSolver/1000/2/5/128",
            "value": 1.1046496394820398,
            "unit": "kHz",
            "extra": "iterations: 6\ncpu: 115.86307900000007 ms\nthreads: 1"
          },
          {
            "name": "BM_DGEMVSolver/10/2/5/128",
            "value": 6053.856570446349,
            "unit": "kHz",
            "extra": "iterations: 33044\ncpu: 0.021141768066820013 ms\nthreads: 1"
          },
          {
            "name": "BM_DGEMVSolver/100/2/5/128",
            "value": 143.53438546732588,
            "unit": "kHz",
            "extra": "iterations: 808\ncpu: 0.8917305495049513 ms\nthreads: 1"
          },
          {
            "name": "BM_DGEMVSolver/1000/2/5/128",
            "value": 1.1050385215565035,
            "unit": "kHz",
            "extra": "iterations: 6\ncpu: 115.83120699999996 ms\nthreads: 1"
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
          "id": "00918405fc62742e2a90f344632556e55e570f94",
          "message": "parse benchmark json",
          "timestamp": "2024-05-21T19:10:08+02:00",
          "tree_id": "48148a757d330d7cdd12c55b8c1b04c505b2a013",
          "url": "https://github.com/artpelling/state-space-renderer/commit/00918405fc62742e2a90f344632556e55e570f94"
        },
        "date": 1716311513704,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_SinglePNativeSolver/10/2/5/128",
            "value": 1975.7352436371232,
            "unit": "kHz/iter",
            "extra": "iterations: 10773\ncpu: 1975.8339532841728 kHz\nthreads: 1"
          },
          {
            "name": "BM_SinglePNativeSolver/100/2/5/128",
            "value": 26.14051619116373,
            "unit": "kHz/iter",
            "extra": "iterations: 143\ncpu: 26.141501496836604 kHz\nthreads: 1"
          },
          {
            "name": "BM_SinglePNativeSolver/1000/2/5/128",
            "value": 0.26821401511224147,
            "unit": "kHz/iter",
            "extra": "iterations: 2\ncpu: 0.26823051089706146 kHz\nthreads: 1"
          },
          {
            "name": "BM_DoublePNativeSolver/10/2/5/128",
            "value": 1968.311720469014,
            "unit": "kHz/iter",
            "extra": "iterations: 10790\ncpu: 1968.341222691092 kHz\nthreads: 1"
          },
          {
            "name": "BM_DoublePNativeSolver/100/2/5/128",
            "value": 26.139372124655257,
            "unit": "kHz/iter",
            "extra": "iterations: 143\ncpu: 26.141007192041762 kHz\nthreads: 1"
          },
          {
            "name": "BM_DoublePNativeSolver/1000/2/5/128",
            "value": 0.2679493701668395,
            "unit": "kHz/iter",
            "extra": "iterations: 2\ncpu: 0.2679652828115329 kHz\nthreads: 1"
          },
          {
            "name": "BM_FGEMVSolver/10/2/5/128",
            "value": 5935.795666594731,
            "unit": "kHz/iter",
            "extra": "iterations: 32430\ncpu: 5935.796379579464 kHz\nthreads: 1"
          },
          {
            "name": "BM_FGEMVSolver/100/2/5/128",
            "value": 146.9747910322996,
            "unit": "kHz/iter",
            "extra": "iterations: 801\ncpu: 146.97484581162766 kHz\nthreads: 1"
          },
          {
            "name": "BM_FGEMVSolver/1000/2/5/128",
            "value": 1.1058475630920344,
            "unit": "kHz/iter",
            "extra": "iterations: 6\ncpu: 1.1059140048844147 kHz\nthreads: 1"
          },
          {
            "name": "BM_DGEMVSolver/10/2/5/128",
            "value": 6028.608630558527,
            "unit": "kHz/iter",
            "extra": "iterations: 33079\ncpu: 6028.768685486678 kHz\nthreads: 1"
          },
          {
            "name": "BM_DGEMVSolver/100/2/5/128",
            "value": 148.21819253123422,
            "unit": "kHz/iter",
            "extra": "iterations: 811\ncpu: 148.22245165803525 kHz\nthreads: 1"
          },
          {
            "name": "BM_DGEMVSolver/1000/2/5/128",
            "value": 1.104749545431223,
            "unit": "kHz/iter",
            "extra": "iterations: 6\ncpu: 1.1047492037627993 kHz\nthreads: 1"
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
          "id": "1bc838bd21a3b343f35ef31934e86e1cea9dbaf5",
          "message": "add logo",
          "timestamp": "2024-05-22T11:52:13+02:00",
          "tree_id": "4eb681103064b6dc8df6abd898e6efaf29ea2110",
          "url": "https://github.com/artpelling/state-space-renderer/commit/1bc838bd21a3b343f35ef31934e86e1cea9dbaf5"
        },
        "date": 1716371629247,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_SinglePNativeSolver/10/2/5/128",
            "value": 1977.7028074495036,
            "unit": "kHz/iter",
            "extra": "iterations: 10756\ncpu: 1977.6940574083571 kHz\nthreads: 1"
          },
          {
            "name": "BM_SinglePNativeSolver/100/2/5/128",
            "value": 26.14315882366305,
            "unit": "kHz/iter",
            "extra": "iterations: 143\ncpu: 26.14461615899054 kHz\nthreads: 1"
          },
          {
            "name": "BM_SinglePNativeSolver/1000/2/5/128",
            "value": 0.26827971353502905,
            "unit": "kHz/iter",
            "extra": "iterations: 2\ncpu: 0.2682901805381008 kHz\nthreads: 1"
          },
          {
            "name": "BM_DoublePNativeSolver/10/2/5/128",
            "value": 1989.7488376451042,
            "unit": "kHz/iter",
            "extra": "iterations: 10808\ncpu: 1989.8169024660658 kHz\nthreads: 1"
          },
          {
            "name": "BM_DoublePNativeSolver/100/2/5/128",
            "value": 26.1488283221336,
            "unit": "kHz/iter",
            "extra": "iterations: 143\ncpu: 26.149652641994884 kHz\nthreads: 1"
          },
          {
            "name": "BM_DoublePNativeSolver/1000/2/5/128",
            "value": 0.2678389618116495,
            "unit": "kHz/iter",
            "extra": "iterations: 2\ncpu: 0.2678464563701952 kHz\nthreads: 1"
          },
          {
            "name": "BM_FGEMVSolver/10/2/5/128",
            "value": 5939.263897620164,
            "unit": "kHz/iter",
            "extra": "iterations: 32421\ncpu: 5939.326774225613 kHz\nthreads: 1"
          },
          {
            "name": "BM_FGEMVSolver/100/2/5/128",
            "value": 146.99876249741956,
            "unit": "kHz/iter",
            "extra": "iterations: 802\ncpu: 147.003749307313 kHz\nthreads: 1"
          },
          {
            "name": "BM_FGEMVSolver/1000/2/5/128",
            "value": 1.1050369538313436,
            "unit": "kHz/iter",
            "extra": "iterations: 6\ncpu: 1.1051119615836602 kHz\nthreads: 1"
          },
          {
            "name": "BM_DGEMVSolver/10/2/5/128",
            "value": 6021.379092149265,
            "unit": "kHz/iter",
            "extra": "iterations: 33008\ncpu: 6022.06574122547 kHz\nthreads: 1"
          },
          {
            "name": "BM_DGEMVSolver/100/2/5/128",
            "value": 147.8672687099438,
            "unit": "kHz/iter",
            "extra": "iterations: 808\ncpu: 147.87397172532252 kHz\nthreads: 1"
          },
          {
            "name": "BM_DGEMVSolver/1000/2/5/128",
            "value": 1.101922586463253,
            "unit": "kHz/iter",
            "extra": "iterations: 6\ncpu: 1.1020094536856724 kHz\nthreads: 1"
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
          "id": "633ac6648d8d331dab58e2fb960301ac1864b32e",
          "message": "fixup! add logo",
          "timestamp": "2024-05-22T11:53:35+02:00",
          "tree_id": "de3c121811549eed459ab8ec56f1d0fc73d34bc1",
          "url": "https://github.com/artpelling/state-space-renderer/commit/633ac6648d8d331dab58e2fb960301ac1864b32e"
        },
        "date": 1716371709928,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_SinglePNativeSolver/10/2/5/128",
            "value": 1971.9076983312011,
            "unit": "kHz/iter",
            "extra": "iterations: 10817\ncpu: 1971.9765398233365 kHz\nthreads: 1"
          },
          {
            "name": "BM_SinglePNativeSolver/100/2/5/128",
            "value": 26.117884257774094,
            "unit": "kHz/iter",
            "extra": "iterations: 143\ncpu: 26.119269786953637 kHz\nthreads: 1"
          },
          {
            "name": "BM_SinglePNativeSolver/1000/2/5/128",
            "value": 0.22951351556769847,
            "unit": "kHz/iter",
            "extra": "iterations: 2\ncpu: 0.22952430713472755 kHz\nthreads: 1"
          },
          {
            "name": "BM_DoublePNativeSolver/10/2/5/128",
            "value": 1975.9977159889181,
            "unit": "kHz/iter",
            "extra": "iterations: 10819\ncpu: 1976.1089440103046 kHz\nthreads: 1"
          },
          {
            "name": "BM_DoublePNativeSolver/100/2/5/128",
            "value": 26.08114191492213,
            "unit": "kHz/iter",
            "extra": "iterations: 143\ncpu: 26.085277738021063 kHz\nthreads: 1"
          },
          {
            "name": "BM_DoublePNativeSolver/1000/2/5/128",
            "value": 0.2677446599376084,
            "unit": "kHz/iter",
            "extra": "iterations: 2\ncpu: 0.2677568629998364 kHz\nthreads: 1"
          },
          {
            "name": "BM_FGEMVSolver/10/2/5/128",
            "value": 5789.032274213735,
            "unit": "kHz/iter",
            "extra": "iterations: 32456\ncpu: 5789.298050621415 kHz\nthreads: 1"
          },
          {
            "name": "BM_FGEMVSolver/100/2/5/128",
            "value": 146.34645218213637,
            "unit": "kHz/iter",
            "extra": "iterations: 802\ncpu: 146.35029319043562 kHz\nthreads: 1"
          },
          {
            "name": "BM_FGEMVSolver/1000/2/5/128",
            "value": 1.1059062191698994,
            "unit": "kHz/iter",
            "extra": "iterations: 6\ncpu: 1.1060023626687472 kHz\nthreads: 1"
          },
          {
            "name": "BM_DGEMVSolver/10/2/5/128",
            "value": 6036.361415518285,
            "unit": "kHz/iter",
            "extra": "iterations: 33015\ncpu: 6036.748164476329 kHz\nthreads: 1"
          },
          {
            "name": "BM_DGEMVSolver/100/2/5/128",
            "value": 147.81192549752694,
            "unit": "kHz/iter",
            "extra": "iterations: 798\ncpu: 147.82034712924857 kHz\nthreads: 1"
          },
          {
            "name": "BM_DGEMVSolver/1000/2/5/128",
            "value": 1.1049658575621477,
            "unit": "kHz/iter",
            "extra": "iterations: 6\ncpu: 1.1050588992076655 kHz\nthreads: 1"
          }
        ]
      }
    ]
  }
}