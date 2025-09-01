import matplotlib.pyplot as plt
import matplotlib
matplotlib.use("Agg")  
# Matrix sizes
matrix_sizes = [128, 256, 512, 1024, 2048]

# Instruction counts for each optimization (fill missing values with None for plotting gaps)
naive = [50007415, 23272974, 1627287532, 14099619260, 95532597246]
simd128 = [36076291, None, 315813715, 5218824556, 42004556817]
simd256 = [66287992, 66287992, 315814195, 2672148722, 19866516983]
loop_unroll_reorder = [92201149, 63019022, 2796200266, 21622281236, 224834156457]
reorder_ikj = [10158207, 584485857, 379060588, 2805657638, 18491826727]
tiling = [15006283, 84400953, 373263824, 2782928401, 14602535335]
tiling_simd256 = [10279121, 88195359, 285453070, 2756422822, 25600000420]
tiling_simd_unroll = [10025559, 145817511, 388312921, 2194147598, 18886181660]

# Plotting
plt.figure(figsize=(12, 7))

plt.plot(matrix_sizes, naive, marker='o', label="Naive")
plt.plot(matrix_sizes, simd128, marker='o', label="SIMD 128-bit")
plt.plot(matrix_sizes, simd256, marker='o', label="SIMD 256-bit")
plt.plot(matrix_sizes, loop_unroll_reorder, marker='o', label="Loop Unroll + Reorder (i-k-j)")
plt.plot(matrix_sizes, reorder_ikj, marker='o', label="Reorder (i-k-j)")
plt.plot(matrix_sizes, tiling, marker='o', label="Tiling")
plt.plot(matrix_sizes, tiling_simd256, marker='o', label="Tiling + SIMD 256-bit")
plt.plot(matrix_sizes, tiling_simd_unroll, marker='o', label="Tiling + SIMD + Loop Unroll + Reorder")

plt.xlabel("Matrix Size (N)")
plt.ylabel("Instructions Executed")
plt.title("Comparison of Matrix Multiplication Optimizations")
plt.yscale("log")  # log scale for readability
plt.grid(True, which="both", linestyle="--", linewidth=0.5)
plt.legend()
plt.tight_layout()
plt.savefig("instructions.jpg", dpi=300)   # <-- Save as JPG
plt.show()
