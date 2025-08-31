# Task 2C

## SPEEDUP Measurements

| SIZE      | DIM  | SW PREFETCH </br>(PD = 8 , CFL = L1)| SIMD   </br>(256 bits)   | BOTH   </br>(PD = 8 , CFL = L1, </br>SIMD = 256 bits)   | 
|-----------|------|-------------|-----------|-----------|
| 100K  | 64   | 1.218x     | 1.41x   | 1.705x   | 
| 1M | 8    | 1.6x         | 1.794x   | 2.483x   |
| 2M | 256  | 1.2x         | 1.516x   | 1.983x   |
| 40K    | 2048 | 1.104x    | 2.176x   | 2.354x   | 
| 100K   | 31   | 2.455x     | 1.554x   | 2.580x   |



<!-- | SIZE      | DIM  | SW Prefetch (PD = 8, CFL = L1) | SIMD (256 bits) | BOTH (PD = 8, CFL = L1, SIMD = 256 bits) | REMARKS                                           |
|-----------|------|--------------------------------|----------------|------------------------------------------|--------------------------------------------------|
| 100K      | 64   | 1.218x                         | 1.41x          | 1.705x                                   | Fits mostly in L1/L2, SIMD 256-bit effective   |
| 1M        | 8    | 1.6x                           | 1.794x         | 2.483x                                   | L3 cache bound, SIMD benefits                   |
| 2M        | 256  | 1.2x                           | 1.516x         | 1.983x                                   | Larger dataset, partial L3 reuse, SIMD 256-bit |
| 40K       | 2048 | 1.104x                         | 2.176x         | 2.354x                                   | Small dataset, L1/L2 cache fit, SIMD 256-bit   |
| 100K      | 31   | 2.455x                         | 1.554x         | 2.580x                                   | Untiled, smaller dimension                       | -->


---
### Comparative Performance Plot

**SW Prefetch vs SIMD vs BOTH vs Baseline**

![Speedup Comparison](Plots/BOTH.png)

*Description:* For each size × dimension, this plot compares:  
1. **SW Prefetch only (PD = 8, CFL = L1)**  
2. **SIMD only (256 bits)**  
3. **Both combined (PD = 8, CFL = L1, SIMD = 256 bits)**  
4. **Baseline (No SW Prefetch, No SIMD)**  
---
## Notes

- **CPU & System:** Intel 12th Gen Core i5-12400, 6 cores / 12 threads, max 4.4 GHz.  
- **Cache Hierarchy:**  
  - L1 Data: 48 KB/core  
  - L1 Instruction: 32 KB/core  
  - L2: 1.25 MB/core  
  - L3: 18 MB shared  
- **Memory & SIMD Settings:**  
  - Embedding calculation on **float (4 bytes)**  
  - **SIMD 256-bit** used  
  - **Load/Store:** `loadu` / `storeu` instructions  
  - **HW prefetch:** Disabled  
  - **Flush:** Enabled (cold misses observed)  
- **Performance Mode:** CPU frequency fixed  
- **Page-Dispatch (PD):** 8  
- **Cache Fill Level:** 1  
- Notes capture **experimental setup** affecting speedup measurements.
