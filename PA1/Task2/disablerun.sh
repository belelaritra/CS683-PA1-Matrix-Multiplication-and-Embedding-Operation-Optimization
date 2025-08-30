#!/bin/bash

# Disable hardware prefetchers on core 1
sudo wrmsr -p 1 0x1A4 0xF
echo "Hardware prefetchers disabled on core 1"

# Compile
g++ -O3 -mavx2 -mfma -march=native -std=c++17 -o emb emb.cpp

# Run comprehensive performance analysis
echo "=== Running Performance Analysis ==="
taskset -c 1 perf stat \
  -e instructions,cycles,cache-misses,cache-references \
  -e L1-dcache-loads,L1-dcache-load-misses \
  -e LLC-loads,LLC-load-misses \
  -e sw_prefetch_access.t0,sw_prefetch_access.t1_t2,sw_prefetch_access.nta \
  ./emb

# Restore hardware prefetchers
sudo wrmsr -p 1 0x1A4 0x3
echo "Hardware prefetchers restored"
