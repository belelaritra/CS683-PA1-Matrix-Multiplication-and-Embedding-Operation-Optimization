#!/bin/bash

# Usage: ./run_analysis.sh [mode] [hw_prefetch]
# mode: naive, prefetch, simd, or both
# hw_prefetch: disable | enable (optional, default is enable)

MODE=$1
HW_PREFETCH_OPTION=$2
CORE=1

# Check if mode is provided
if [ -z "$MODE" ]; then
    echo "Usage: $0 [mode] [hw_prefetch]"
    echo "Modes: naive, prefetch, simd, both"
    echo "HW Prefetch: disable, enable (optional, default is enable)"
    exit 1
fi

# Determine hardware prefetcher action based on argument
if [ "$HW_PREFETCH_OPTION" == "disable" ]; then
    DISABLE_HW_PF=1
    HW_STATUS="disabled"
    echo "Hardware prefetcher : DISABLED"
elif [ "$HW_PREFETCH_OPTION" == "enable" ] || [ -z "$HW_PREFETCH_OPTION" ]; then
    DISABLE_HW_PF=0
    HW_STATUS="enabled"
    echo "Hardware prefetcher : ENABLED"
else
    echo "Invalid hw_prefetch option. Use 'disable', 'enable', or leave blank."
    exit 1
fi

# Output file (append mode)
LOG_FILE="perf_${MODE}_pf${HW_STATUS}.log"

# Function to fix CPU frequency (ADDED)
fix_cpu_frequency() {
    echo "=== Fixing CPU Frequency ==="
    # Get original governor
    ORIGINAL_GOVERNOR=$(cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor 2>/dev/null || echo "ondemand")
    # Set performance governor
    echo performance | sudo tee /sys/devices/system/cpu/cpu*/cpufreq/scaling_governor > /dev/null
    # Disable boost to prevent frequency variations
    echo 0 | sudo tee /sys/devices/system/cpu/cpufreq/boost > /dev/null 2>&1 || true
    echo "CPU frequency fixed to performance mode"
}

# Function to restore CPU frequency (ADDED)
restore_cpu_frequency() {
    echo "=== Restoring CPU Frequency ==="
    echo ${ORIGINAL_GOVERNOR:-ondemand} | sudo tee /sys/devices/system/cpu/cpu*/cpufreq/scaling_governor > /dev/null
    echo 1 | sudo tee /sys/devices/system/cpu/cpufreq/boost > /dev/null 2>&1 || true
    echo "CPU frequency restored"
}

# Function to disable hardware prefetchers
disable_hw_prefetch() {
    if [ "$DISABLE_HW_PF" -eq 1 ]; then
        sudo wrmsr -p $CORE 0x1A4 0xF
        echo "Hardware prefetchers disabled on core $CORE"
    fi
}

# Function to enable hardware prefetchers
enable_hw_prefetch() {
    if [ "$DISABLE_HW_PF" -eq 1 ]; then
        sudo wrmsr -p $CORE 0x1A4 0x3
        echo "Hardware prefetchers re-enabled on core $CORE"
    fi
}

# Function to compile the code
compile() {
    echo "======================================================"
    echo "=== Compiling ==="
    # g++ -O3 -mavx2 -mfma -march=native -std=c++17 -o emb emb.cpp
    g++ -O0 -mavx2 -mfma -fno-tree-vectorize -fno-unroll-loops -fno-inline -std=c++17 -o emb emb.cpp
    if [ $? -ne 0 ]; then
        echo "Compilation failed!"
        exit 1
    fi
}

# Function to run performance analysis
run_perf() {
    echo "=== Running Performance Analysis for mode: $MODE (PF $HW_STATUS) ==="

    # Run perf and capture both stdout+stderr into terminal & log
    taskset -c $CORE perf stat \
        -e instructions,cycles,cache-misses \
        -e cpu_core/L1-dcache-load-misses/ \
        -e cpu_core/l2_rqsts.all_demand_miss/ \
        -e cpu_core/LLC-load-misses/ \
        -e cpu_core/l2_rqsts.swpf_hit/ \
        -e cpu_core/l2_rqsts.swpf_miss/ \
        -e sw_prefetch_access.t0,sw_prefetch_access.t1_t2,sw_prefetch_access.nta \
        ./emb $MODE 2>&1 | tee -a "$LOG_FILE"

    echo "Results stored in: $LOG_FILE"
}

# Main execution
echo "Starting analysis for mode: $MODE (PF $HW_STATUS)"

fix_cpu_frequency          # ADDED: Fix CPU frequency before test
disable_hw_prefetch
compile
run_perf
enable_hw_prefetch
restore_cpu_frequency      # ADDED: Restore CPU frequency after test

echo "Analysis complete! Log stored at $LOG_FILE"
