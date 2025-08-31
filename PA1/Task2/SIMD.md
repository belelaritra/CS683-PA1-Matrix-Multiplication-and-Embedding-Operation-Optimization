# Task 2B

## Embedding Size = 1,000,000
|                    | Embedding Dimension → | 32     | 64     | 128    | 256    | 512    | 1024   | 2048   |
|--------------------|------------------------|--------|--------|--------|--------|--------|--------|--------|
| **NO SIMD**        | Instructions           | 10.6B  | 21.2B  | 42.3B  | 84.7B  | 169.6B | 339.1B | 678.2B |
|                    | Execution Time (µs)         | 170    | 216    | 319    | 520    | 722    | 1200   | 2346   |
| **SIMD 128 bits**  | Instructions           | 10.6B  | 21.1B  | 42.1B  | 84.1B  | 168.1B | 336.2B | 672.5B |
|                    | Execution Time (µs)         | 36     | 67     | 110    | 205    | 384    | 734    | 1420   |
| **SIMD 256 bits**  | Instructions           | 10.5B  | 21.0B  | 42.0B  | 84.0B  | 168.1B | 336.3B | 672.5B |
|                    | Execution Time (µs)         | 30     | 53     | 108    | 195    | 333    | 600    | 1189   |
| **Speedup**        | SIMD 128               | 4.36x  | 3.19x  | 2.87x  | 2.32x  | 1.80x  | 1.77x  | 1.65x  |
|                    | SIMD 256               | 5.50x  | 4.04x  | 3.14x  | 2.42x  | 2.18x  | 2.06x  | 1.94x  |



---

### Plots

**SIMD Execution Time vs Embedding Dimension**

![SIMD Execution Time vs Dim](Plots/SIMD-ET-VS-DIM.png)

**Speedup vs Embedding Dimension**

![Speedup vs Dim](Plots/SIMD-SPEEDUP-VS-DIM.png)

---

### Notes
- **NO FLUSH before running SIMD used**  
- **CPU frequency pinned in *performance mode***  
- **K = Thousand, M = Million, B = Billion**  
- **Execution Time** is in **microseconds (µs)**  
- **SIMD width**: 128 bits and 256 bits used  