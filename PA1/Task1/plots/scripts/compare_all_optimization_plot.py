import matplotlib.pyplot as plt
import numpy as np
import matplotlib

matplotlib.use("Agg")

# Data
matrix_sizes = [128, 256, 512, 1024, 2048]

naive = [4.6, 33.2, 267.8, 2414.2, 41317.8]
simd128 = [1.0, 8.6, 30.6, 242.8, 4138.8]
simd256 = [0.2, 6.6, 26.6, 210.2, 3078.6]
unroll = [5, 34, 156, 991, 9275]
reorder = [1, 7, 42, 216, 3221]
tiling = [0, 3, 19, 169, 1420]
tiling_simd256 = [0, 4, 24, 241, 3322]
all_opt = [1, 8, 50, 193, 1397]

times = {
    "Naive": naive,
    "SIMD 128": simd128,
    "SIMD 256": simd256,
    "Loop Unroll": unroll,
    "Loop Reorder": reorder,
    "Tiling": tiling,
    "Tiling+SIMD 256": tiling_simd256,
    "All Opt": all_opt
}

speedups = {key: [na/t if t != 0 else 0 for na, t in zip(naive, val)] for key, val in times.items()}

x = np.arange(len(matrix_sizes))
bar_width = 0.1

# --- Plot 1: Execution Time ---
plt.figure(figsize=(14, 7))
for i, (label, vals) in enumerate(times.items()):
    plt.bar(x + i*bar_width, vals, width=bar_width, label=label)

plt.ylabel("Execution Time (ms, log scale)")
plt.yscale("log")
plt.xticks(x + 3*bar_width, matrix_sizes)
plt.xlabel("Matrix Size (N)")
plt.title("Execution Time Comparison")
plt.legend()
plt.tight_layout()
plt.savefig("execution_time-combined.jpg", dpi=300)

# --- Plot 2: Speedup ---
plt.figure(figsize=(14, 7))
for i, (label, vals) in enumerate(speedups.items()):
    plt.bar(x + i*bar_width, vals, width=bar_width, label=label)

plt.ylabel("Speedup (vs Naive)")
plt.xticks(x + 3*bar_width, matrix_sizes)
plt.xlabel("Matrix Size (N)")
plt.title("Speedup Comparison")
plt.legend()
plt.tight_layout()
plt.savefig("speedup-combined.jpg", dpi=300)

plt.show()
