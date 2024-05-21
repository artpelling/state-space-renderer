#!/usr/bin/env python3

import json
from pathlib import Path


benchfile = Path(__file__).parent / "build/benchmark_result.json"
with open(benchfile, 'r') as f:
    data = json.load(f)

for b in data['benchmarks']:
    bs = int(b['name'].split('/')[-1])
    b['real_time'] = bs / b['real_time']
    b['cpu_time'] = bs / b['cpu_time']
    b['time_unit'] = 'kHz'

with open(benchfile, 'w') as f:
    json.dump(data, f, indent=2)
