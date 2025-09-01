import matplotlib.pyplot as plt
import numpy as np
import matplotlib
matplotlib.use("Agg")  
# Data from your report
matrix_sizes = [128, 256, 512, 1024, 2048]

# Execution time in ms
time_basic   = [1.2, 11.4, 91, 1202, 14138]
time_reorder = [0.8, 7.6, 58, 448, 4828]
time_unroll  = [0.1, 5.2, 53, 530, 6286]   # taking 0.1 avg for 0.0–0.2 unstable
time_tiled   = [0.8, 7.8, 73, 567, 4955]
time_simd    = [1.4, 2.0, 16.8, 139, 1805]

# Speedups
speedup_basic   = [1.0, 1.0, 1.0, 1.0, 1.0]
speedup_reorder = [1.5, 1.5, 1.25, 2.68, 2.93]
speedup_unroll  = [5.0, 2.19, 1.72, 2.27, 2.25]  # 5.0 placeholder for unstable >>1
speedup_tiled   = [1.5, 1.46, 1.25, 2.05, 2.85]
speedup_simd    = [0.86, 5.70, 5.42, 8.63, 7.84]

# -------- Execution time plot --------
plt.figure(figsize=(10,6))
plt.plot(matrix_sizes, time_basic,   marker='o', label="Basic (ijk)")
plt.plot(matrix_sizes, time_reorder, marker='o', label="Reorder (ikj)")
plt.plot(matrix_sizes, time_unroll,  marker='o', label="Unrolled")
plt.plot(matrix_sizes, time_tiled,   marker='o', label="Tiled")
plt.plot(matrix_sizes, time_simd,    marker='o', label="SIMD")

plt.xlabel("Matrix size (N)")
plt.ylabel("Execution Time (ms)")
plt.title("Execution Time Comparison of Matrix Multiplication Optimizations")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig("plot0.jpg", dpi=300)   # <-- Save as JPG

plt.show()

# -------- Speedup plot --------
plt.figure(figsize=(10,6))
plt.plot(matrix_sizes, speedup_basic,   marker='o', label="Basic (ijk)")
plt.plot(matrix_sizes, speedup_reorder, marker='o', label="Reorder (ikj)")
plt.plot(matrix_sizes, speedup_unroll,  marker='o', label="Unrolled")
plt.plot(matrix_sizes, speedup_tiled,   marker='o', label="Tiled")
plt.plot(matrix_sizes, speedup_simd,    marker='o', label="SIMD")

plt.xlabel("Matrix size (N)")
plt.ylabel("Speedup (× vs Basic)")
plt.title("Speedup of Optimizations over Basic ijk")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig("plot1.jpg", dpi=300)   # <-- Save as JPG

plt.show()
