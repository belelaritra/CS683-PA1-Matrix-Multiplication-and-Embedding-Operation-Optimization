import matplotlib.pyplot as plt

# Matrix sizes
sizes = [128, 256, 512, 1024, 2048]

# Speedup values
speedup_ikj = [2.00, 1.86, 6.00, 10.83, 3.87]      # I-J-K vs I-K-J
speedup_kij = [2.00, 1.86, 4.00, 6.82, 2.74]       # I-J-K vs K-I-J
speedup_unrolled = [0.40, 0.38, 1.62, 2.36, 1.34]  # I-J-K vs Unrolled

# Plot
plt.figure(figsize=(10,6))
plt.plot(sizes, speedup_ikj, marker='o', label='I-J-K vs I-K-J')
plt.plot(sizes, speedup_kij, marker='s', label='I-J-K vs K-I-J')
plt.plot(sizes, speedup_unrolled, marker='^', label='I-J-K vs Unrolled')

plt.title('Speedup Comparison for Loop Orders')
plt.xlabel('Matrix Size (N)')
plt.ylabel('Speedup')
plt.xticks(sizes)
plt.grid(True, linestyle='--', alpha=0.7)
plt.legend()
plt.tight_layout()

# Save figure as JPG
plt.savefig('speedup_comparison.jpg', format='jpg', dpi=300)
plt.close()  # Close figure to avoid displaying in some environments
