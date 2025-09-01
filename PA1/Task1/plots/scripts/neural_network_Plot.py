import matplotlib.pyplot as plt
import matplotlib
matplotlib.use("Agg")  

# Data
matrix_sizes = [128, 256, 512, 1024]
naive = [0.25, 2.10, 17.3, 140.0]
tiling = [0.08, 0.60, 4.2, 28.5]
simd256 = [0.06, 0.40, 3.0, 20.0]
tiling_simd = [0.05, 0.32, 2.5, 16.0]
speedup = [5.0, 6.6, 6.9, 8.7]

# Plot 1: Execution time comparison
plt.figure(figsize=(10,6))
plt.plot(matrix_sizes, naive, marker='o', label="Naive")
plt.plot(matrix_sizes, tiling, marker='o', label="Tiling")
plt.plot(matrix_sizes, simd256, marker='o', label="SIMD 256-bit")
plt.plot(matrix_sizes, tiling_simd, marker='o', label="Tiling + SIMD")

plt.xlabel("Matrix Size (N)")
plt.ylabel("Execution Time (seconds)")
plt.title("Matrix Multiplication Execution Time Comparison")
plt.yscale("log")  # log scale since times vary a lot
plt.grid(True, which="both", linestyle="--", linewidth=0.5)
plt.legend()
plt.tight_layout()
plt.show()

# Plot 2: Speedup vs Naive
plt.figure(figsize=(8,5))
plt.plot(matrix_sizes, speedup, marker='s', color='green', linewidth=2)
plt.xlabel("Matrix Size (N)")
plt.ylabel("Speedup vs Naive")
plt.title("Speedup of Best Optimization over Naive")
plt.grid(True, linestyle="--", linewidth=0.5)
plt.tight_layout()
plt.savefig("neural.jpg", dpi=300)   # <-- Save as JPG
plt.show()
