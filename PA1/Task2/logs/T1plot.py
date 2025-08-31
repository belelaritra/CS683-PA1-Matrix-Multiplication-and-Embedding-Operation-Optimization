import matplotlib.pyplot as plt
import numpy as np

# PD distribution
PDs = np.array([2, 4, 8, 16, 32, 64])

# Speedup data per dataset (replace with your corrected values)
# Rows = PDs, Columns = L1, L2, LLC

speedup_30K = np.array([
    [1.023, 0.857, 1.093],
    [1.143, 0.829, 0.691],
    [0.757, 1.095, 1.140],
    [1.042, 0.800, 1.020],
    [0.885, 0.771, 0.585],
    [0.500, 0.880, 0.683]
])

speedup_100K = np.array([
    [1.860, 1.727, 1.767],
    [2.000, 2.043, 1.745],
    [1.711, 1.750, 2.196],
    [1.524, 1.630, 1.553],
    [2.020, 1.519, 1.388],
    [1.509, 1.500, 1.434]
])

speedup_1M = np.array([
    [2.407, 2.255, 2.236],
    [1.944, 2.510, 2.592],
    [2.214, 2.878, 2.717],
    [1.814, 2.034, 2.082],
    [1.414, 1.905, 1.653],
    [1.969, 1.675, 1.708]
])

datasets = {
    "30K × 128": speedup_30K,
    "100K × 128": speedup_100K,
    "1M × 128": speedup_1M
}

colors = ['tab:blue', 'tab:orange', 'tab:green']
cache_levels = ["L1", "L2", "LLC"]

for name, data in datasets.items():
    fig, axs = plt.subplots(1, 3, figsize=(18,5), sharey=True)
    
    for i, ax in enumerate(axs):
        ax.plot(PDs, data[:,i], marker='o', linestyle='-', color=colors[i])
        ax.set_title(f"{cache_levels[i]} Cache (Cache Fill Level)")
        ax.set_xlabel("Prefetch Distance (PD)")
        ax.set_xticks(PDs)
        ax.grid(True)
    
    axs[0].set_ylabel("Speedup (×)")
    fig.suptitle(f"Speedup vs Prefetch Distance ({name})", fontsize=16)
    plt.tight_layout(rect=[0, 0, 1, 0.95])
    plt.show()
