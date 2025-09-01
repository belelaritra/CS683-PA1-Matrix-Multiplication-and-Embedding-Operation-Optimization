import matplotlib.pyplot as plt
import numpy as np
import matplotlib
matplotlib.use("Agg")  
# Data
matrix_sizes = [128, 256, 512, 1024, 2048]
methods = [
    "SIMD 128-bit",
    "SIMD 256-bit",
    "Loop unroll + Loop reorder",
    "Loop reorder (i-k-j)",
    "Tiling",
    "Tiling + SIMD 256-bit",
    "Tiling + SIMD + unroll + reorder"
]

execution_times = [
    [0.003, 0.002, 5, 1, 0, 0, 1],          # 128
    [ 4, 3, 34, 7, 3, 4, 8],                # 256
    [24, 20, 156, 42, 19, 24, 50],        # 512
    [241, 184, 991, 216, 169, 241, 193], # 1024
    [3322, 2734, 9275, 3221, 1420, 3322, 1397] # 2048
]

# Convert to numpy array for easy plotting
execution_times = np.array(execution_times)

# Bar chart setup
bar_width = 0.12
x = np.arange(len(matrix_sizes))

plt.figure(figsize=(14, 7))

# Plot each method
for i in range(len(methods)):
    plt.bar(x + i*bar_width, execution_times[:, i], width=bar_width, label=methods[i])

# Labels and formatting
plt.xlabel("Matrix Size (N)", fontsize=12)
plt.ylabel("Execution Time (ms)", fontsize=12)
plt.title("Execution Time for Various Matrix Sizes and Optimizations", fontsize=14)
plt.xticks(x + bar_width*(len(methods)//2), matrix_sizes)
plt.legend(loc="upper left", bbox_to_anchor=(1,1))
plt.grid(axis="y", linestyle="--", alpha=0.6)

plt.tight_layout()
plt.savefig("performance.jpg", dpi=300)   # <-- Save as JPG

plt.show()
