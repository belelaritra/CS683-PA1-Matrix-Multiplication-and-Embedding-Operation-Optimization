import pandas as pd
import matplotlib.pyplot as plt

# Your data
data = {
    "SIZE": [100000, 1000000, 2000000, 40000, 1000000],
    "DIM": [64, 8, 256, 2048, 31],
    "SW PREFETCH": [1.21805, 1.6, 1.2, 1.10474, 2.45588],
    "SIMD": [1.41085, 1.79412, 1.51623, 2.17653, 1.55446],
    "BOTH": [1.70526, 2.48387, 1.98381, 2.35474, 2.58065],
    # "REMARKS": ["S,S", "L,S", "L,L", "S,L", "Untiled"]
}

# Convert to DataFrame
df = pd.DataFrame(data)

# Format SIZE to K/M for readability
def format_size(x):
    if x >= 1_000_000:
        return f"{x//1_000_000}M"
    else:
        return f"{x//1000}K"

df["SizeLabel"] = df["SIZE"].apply(format_size)

# Build Case label WITHOUT remarks
df["Case"] = "(" + df["SizeLabel"] + ", " + df["DIM"].astype(str) + ")"

# Bar chart data
methods = ["SW PREFETCH", "SIMD", "BOTH"]
colors = ["#1f77b4", "#ff7f0e", "#7f7f7f"]  # Blue, Orange, Gray

# Plot
ax = df.set_index("Case")[methods].plot(
    kind="bar",
    figsize=(10, 5),
    width=0.8,
    color=colors
)

# Styling
plt.ylabel("Normalized speedup\n(no optimization)")
plt.xlabel("(embedding table size, embedding dimension)")
# plt.title("Speedup Comparison")
plt.xticks(rotation=0)  # angled for readability
plt.ylim(1, None)  # <-- start from 1.0

plt.legend(["Software prefetching", "SIMD", "Software prefetching + SIMD"], 
           loc="upper center", bbox_to_anchor=(0.5, 1.12), ncol=3, frameon=False)

plt.grid(axis="y", linestyle="--", alpha=0.6)
plt.tight_layout()
plt.show()
