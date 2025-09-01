import matplotlib.pyplot as plt
import matplotlib

matplotlib.use("Agg") 


import matplotlib.pyplot as plt
import numpy as np

# Data from your table
matrix_sizes = [128, 256, 512, 1024, 2048]
tiled_time = [1.0, 4.6, 23.6, 192.6, 1452.8]     # Tiled execution time (ms)
baseline_time = [4.0, 24.4, 265.6, 2413.4, 42324.4]  # Baseline (naive) execution time (ms)
speedup = [4.00, 6.42, 11.28, 12.28, 29.14]      # Naive / Tiled

# Create figure and axis
fig, ax1 = plt.subplots(figsize=(8, 6))

# Plot execution time (log scale on left y-axis)
ax1.set_xlabel("Matrix Size (N)")
ax1.set_ylabel("Execution Time (ms)", color="tab:blue")
ax1.plot(matrix_sizes, baseline_time, marker="o", label="Baseline Time", color="tab:red", linestyle="--")
ax1.plot(matrix_sizes, tiled_time, marker="s", label="Tiled Time", color="tab:blue")
ax1.set_yscale("log")
ax1.tick_params(axis="y", labelcolor="tab:blue")

# Secondary y-axis for speedup
ax2 = ax1.twinx()
ax2.set_ylabel("Speedup (Baseline/Tiled)", color="tab:green")
ax2.plot(matrix_sizes, speedup, marker="^", label="Speedup", color="tab:green")
ax2.tick_params(axis="y", labelcolor="tab:green")

# Title and legends
fig.suptitle("Matrix Multiplication: Baseline vs Tiled Performance", fontsize=14)
ax1.legend(loc="upper left")
ax2.legend(loc="upper right")

plt.tight_layout()
plt.savefig("tiling-1.jpg", dpi=300)   # <-- Save as JPG
plt.show()
