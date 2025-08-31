import matplotlib.pyplot as plt
import numpy as np

# ---------------- Data ----------------
dims = np.array([32, 64, 128, 256, 512, 1024, 2048])

# Instructions (Billions)
instr_no_simd   = np.array([10611935991, 21159197880, 42349488441, 84736698100, 169586331369, 339128114631, 678238325114]) / 1e9
instr_simd128   = np.array([10551747738, 21052286410, 42092916469, 84060651579, 168090051012, 336246138803, 672546584909]) / 1e9
instr_simd256   = np.array([10540923279, 21045566496, 42048503552, 83997520527, 168147872907, 336318968662, 672522494047]) / 1e9

# Execution time (µs)
time_no_simd    = np.array([170, 216, 319, 520, 722, 1200, 2346])
time_simd128    = np.array([36, 67, 110, 205, 384, 734, 1420])
time_simd256    = np.array([30, 53, 108, 195, 333, 600, 1189])

# Speedup
speedup_simd128 = np.array([4.36111, 3.19403, 2.87273, 2.32195, 1.79688, 1.76703, 1.64507])
speedup_simd256 = np.array([5.5, 4.03774, 3.13889, 2.41538, 2.18318, 2.05833, 1.94281])

# ---------------- Plot 1: Execution Time ----------------
plt.figure(figsize=(9,6))
plt.plot(dims, time_no_simd,   marker='o', label="NO SIMD")
plt.plot(dims, time_simd128,   marker='s', label="SIMD 128-bit")
plt.plot(dims, time_simd256,   marker='^', label="SIMD 256-bit")
plt.xlabel("Embedding Dimension")
plt.ylabel("Execution Time (µs)")
plt.title("Execution Time vs Embedding Dimension")
plt.xscale("log", base=2)
plt.xticks(dims, labels=dims)
plt.legend()
plt.grid(True, which="both")
plt.show()

# ---------------- Plot 2: Instructions ----------------
plt.figure(figsize=(9,6))
plt.plot(dims, instr_no_simd,   marker='o', label="NO SIMD", markersize=8)
plt.plot(dims, instr_simd128,   marker='s', label="SIMD 128-bit", markersize=8)
plt.plot(dims, instr_simd256,   marker='^', label="SIMD 256-bit", markersize=8)
plt.xlabel("Embedding Dimension")
plt.ylabel("Instructions (Billions)")
plt.title("Instruction Count vs Embedding Dimension")
plt.xscale("log", base=2)
plt.xticks(dims, labels=dims)
plt.legend()
plt.grid(True, which="both")
plt.show()
# Small offsets for visibility
# Subtract first value of NO SIMD from all arrays for better visualization
# Subtract NO SIMD value for each row
# Compute deltas w.r.t NO SIMD
# instr_no_simd_delta = np.zeros_like(instr_no_simd)  # baseline = 0
# instr_simd128_delta = instr_simd128 - instr_no_simd
# instr_simd256_delta = instr_simd256 - instr_no_simd

# plt.figure(figsize=(9,6))
# plt.plot(dims, instr_no_simd_delta, marker='o', label="NO SIMD (baseline)", markersize=8)
# plt.plot(dims, instr_simd128_delta, marker='s', label="SIMD 128-bit Δ", markersize=8)
# plt.plot(dims, instr_simd256_delta, marker='^', label="SIMD 256-bit Δ", markersize=8)
# plt.xlabel("Embedding Dimension")
# plt.ylabel("Instruction Count Δ (Millions)")
# plt.title("Instruction Count Reduction vs Embedding Dimension")
# plt.xscale("log", base=2)
# plt.xticks(dims, labels=dims)
# plt.legend()
# plt.grid(True, which="both")
# plt.show()

# ---------------- Plot 3: Speedup ----------------
plt.figure(figsize=(9,6))
plt.plot(dims, speedup_simd128, marker='s', label="Speedup SIMD 128-bit", markersize=8)
plt.plot(dims, speedup_simd256, marker='^', label="Speedup SIMD 256-bit", markersize=8)
plt.axhline(1, color='red', linestyle='--', label="Baseline (NO SIMD)")
plt.xlabel("Embedding Dimension")
plt.ylabel("Speedup (×)")
plt.title("Speedup vs Embedding Dimension")
plt.xscale("log", base=2)
plt.xticks(dims, labels=dims)
plt.legend()
plt.grid(True, which="both")
plt.show()
