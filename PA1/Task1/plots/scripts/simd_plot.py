import matplotlib.pyplot as plt
import matplotlib

matplotlib.use("Agg") 

matrix_sizes = [256, 512, 1024, 2048]   # <-- 4 values

naive_times = [33.2, 267.8, 2414, 41317] 
simd128_times = [8.6, 30.6, 242.8, 4138.8]  # 4 values
simd256_times = [6.6, 26.6, 210.2, 3078.6] 

# Speedups vs Naive
speedup128 = [naive_times[i] / simd128_times[i] for i in range(len(matrix_sizes))]
speedup256 = [naive_times[i] / simd256_times[i] for i in range(len(matrix_sizes))]

# Plot 1: Execution times
plt.figure(figsize=(8,6))
plt.plot(matrix_sizes, naive_times, marker='o', label='Naive')
plt.plot(matrix_sizes, simd128_times, marker='s', label='SIMD 128-bit')
plt.plot(matrix_sizes, simd256_times, marker='^', label='SIMD 256-bit')
plt.yscale('log')
plt.xlabel('Matrix Size (N)')
plt.ylabel('Execution Time (ms, log scale)')
plt.title('Execution Time vs Matrix Size')
plt.legend()
plt.grid(True, which="both", linestyle='--', linewidth=0.5)
plt.savefig("simd1.jpg", dpi=300)   # <-- Save as JPG

plt.show()

# Plot 2: Speedup vs Naive
plt.figure(figsize=(8,6))
plt.plot(matrix_sizes, speedup128, marker='s', label='SIMD 128-bit Speedup')
plt.plot(matrix_sizes, speedup256, marker='^', label='SIMD 256-bit Speedup')
plt.xlabel('Matrix Size (N)')
plt.ylabel('Speedup (x)')
plt.title('Speedup vs Matrix Size (Relative to Naive)')
plt.legend()
plt.grid(True, linestyle='--', linewidth=0.5)
plt.savefig("simd2.jpg", dpi=300)   # <-- Save as JPG

plt.show()
