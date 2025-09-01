import matplotlib.pyplot as plt
import numpy as np
import matplotlib
matplotlib.use("Agg")  # To save without displaying

matrix_sizes = [ 256, 512, 1024, 2048]
naive_time = [29, 270, 2399, 42722]
simd_128_time = [ 4, 24, 241, 3322]
simd_256_time = [ 3, 20, 184, 2734]

# Compute speedups
speedup_128 = [n/s for n,s in zip(naive_time, simd_128_time)]
speedup_256 = [n/s for n,s in zip(naive_time, simd_256_time)]

# Bar chart settings
x = np.arange(len(matrix_sizes))  # positions for groups
width = 0.35  # width of bars

plt.figure(figsize=(10,6))
plt.bar(x - width/2, speedup_128, width, label='SIMD 128-bit')
plt.bar(x + width/2, speedup_256, width, label='SIMD 256-bit')

plt.xlabel('Matrix Size (N)')
plt.ylabel('Speedup over Naive')
plt.title('SIMD Speedup vs Matrix Size')
plt.xticks(x, matrix_sizes)
plt.legend()
plt.grid(axis='y')

plt.savefig('simd_speedup_barplot.jpg')
plt.show()
