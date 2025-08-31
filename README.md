# Matrix Multiplication & Embedding Optimization

## Row-major order (C/C++ default)

A 2D array `A[m][n]` is stored as:

A[0][0], A[0][1], …, A[0][n-1], A[1][0], …, A[1][n-1], …

- Row elements are contiguous.  
- Moving across rows requires a stride of `n`.

## Cache line (CL)

- Smallest unit fetched from memory to cache.  
- Typical **CL = 64 bytes**.  

Meaning:  
- `int (4B)`: 16 integers per CL.  
- `float (4B)`: 16 floats per CL.  
- `double (8B)`: 8 doubles per CL.  

👉 Accessing `A[i][j]` automatically loads the entire cache line containing that element.  
If the program next accesses `A[i][j+1]`, it’s already in cache (**spatial locality**).  

---

# 2. Temporal vs Spatial Locality

- **Spatial Locality** → Nearby addresses are used together.  
  - Example: Traversing an array row-by-row.  
  - Cache line utilization improves.  

- **Temporal Locality** → Same data is reused soon after first access.  
  - Example: In `C[i][j] += A[i][k] * B[k][j]`, the same `A[i][k]` may be reused across multiple `j` values.  
  - Prevents repeated cache misses for the same element.  

Optimizations either exploit:  
- **Spatial locality** → loop reordering, SIMD.  
- **Temporal locality** → tiling, caching.  
- **Parallelism** → unrolling, SIMD, prefetching.  

---

# 3. Loop Reordering (Improving Spatial Locality)

### Naïve order (i–j–k):

```cpp
for (i=0; i<N; i++)
  for (j=0; j<N; j++)
    for (k=0; k<N; k++)
      C[i][j] += A[i][k] * B[k][j];
```
- `A[i][k]` → row-major storage → good access if `i fixed` and `k varies`.

- `B[k][j]` → column-wise traversal → stride = N → every access may cause new CL fetch. So, good access if `k fixed` and `j varies`.

- `C[i][j]`: row traversal okay, but reuse is poor.  

### Optimized i–k–j order :

```cpp
for (int i=0; i<N; i++)
  for (int k=0; k<N; k++) {
    double a_ik = A[i][k];        // Register temporal reuse
    
    for (int j=0; j<N; j++)
      C[i][j] += a_ik * B[k][j];  // j varies row-wise (spatial)
  }
```
Benefits:
- `A[i][k]` cached in register provides temporal reuse across j loop
- `B[k][j]` accessed row-wise exploits spatial locality
- `C[i][j]` updated contiguously enables efficient SIMD vectorization


### Alternative k–i–j order:

```cpp
for (int k=0; k<N; k++)
  for (int i=0; i<N; i++) {
    double a_ik = A[i][k];
    for (int j=0; j<N; j++)
      C[i][j] += a_ik * B[k][j];
  }
```
Effective when `B[k][:]` row `fits entirely in cache`, enabling temporal reuse across i iterations.

👉 **Takeaway:** Reordering improves spatial locality by ensuring row-major layout is respected.  
It also reduces **conflict misses** (when two arrays map to the same cache sets).  

---

# 4. Loop Unrolling (Instruction-Level Parallelism)

### Example: Dot Product

**Scalar version:**

```cpp
for (i=0; i<N; i++)
    sum += A[i] * B[i];
```

**Unrolled by 4:**

```cpp
for (i=0; i<N; i+=4) {
    sum0 += A[i]   * B[i];
    sum1 += A[i+1] * B[i+1];
    sum2 += A[i+2] * B[i+2];
    sum3 += A[i+3] * B[i+3];
}
sum = sum0 + sum1 + sum2 + sum3;
```

### Why it helps:
- Fewer loop branches → less branch misprediction.  
- Reduces loop overhead from N to N/4 branch instructions
- Creates independent accumulators (`sum0..sum3`) → CPU can overlap loads and multiplies.  
- Exploits **Instruction-Level Parallelism (ILP)**: while one instruction waits on data, others proceed.  

### Superscalar CPU context:
- Modern CPUs: fetch **6–8 instructions per cycle**.  
- Execute **out-of-order** to hide latencies.  
- Retire **in-order** to preserve program semantics.  

👉 **Effect:** Improves ILP, reduces loop overhead, partially hides memory latency.  

⚠️ Too much unrolling → larger code (I-cache pressure), register spills.  

---

# 5. Loop Tiling / Blocking (Improving Temporal Locality)

Large matrices don’t fit in cache. Without tiling, reused values get evicted before reuse.  

### Naïve:

```cpp
for (i=0; i<N; i++)
  for (j=0; j<N; j++)
    for (k=0; k<N; k++)
      C[i][j] += A[i][k] * B[k][j];
```

- Each `A[i][k]` and `B[k][j]` reused many times but cache can’t hold them → **capacity misses**.  

### Tiled:

```cpp
for (ii=0; ii<N; ii+=T)
  for (jj=0; jj<N; jj+=T)
    for (kk=0; kk<N; kk+=T)
      for (i=ii; i<ii+T; i++)
        for (j=jj; j<jj+T; j++)
          for (k=kk; k<kk+T; k++)
            C[i][j] += A[i][k] * B[k][j];
```

- Work on **submatrices of size T×T**.  
- Sub-blocks of A, B, C fit into L1/L2.  
- Values reused before eviction.  

👉 **Effect:** 
- Improves temporal locality → reduces **capacity misses**.  
- Reduces memory traffic from O(N³) to O(N³/B) where B = cache lines per tile


⚠️ Tile too small → overhead dominates.  
⚠️ Tile too large → thrashing (evictions).  

---

# 6. SIMD (Vectorization, Data-Level Parallelism)

### Scalar vs SIMD

**Scalar add:**

```cpp
for (i=0; i<N; i++)
    C[i] = A[i] + B[i];
```

**SIMD add (SSE, 128-bit):**

```cpp
for (i=0; i<N; i+=4) {
    __m128 a = _mm_loadu_ps(&A[i]); // Load 4 floats
    __m128 b = _mm_loadu_ps(&B[i]); // Load 4 floats
    __m128 c = _mm_add_ps(a, b);    // Add 4 floats simultaneously (Single instruction, 4 operations)
    _mm_storeu_ps(&C[i], c);        // Store 4 floats
}
```

### Performance scaling:
- SSE (128-bit): 4 floats.  
- AVX (256-bit): 8 floats.  
- AVX-512 (512-bit): 16 floats.  

### Untiled SIMD vs Tiled SIMD
- **Untiled SIMD**: operates on raw vectors; cache may evict values if matrices too big.  
- **Tiled SIMD**: combine **tiling + SIMD** → vectors operate on blocks that fit cache.  

👉 Effect: Exploits **data-level parallelism (DLP)**, reduces instruction count, improves cache line utilization.  

⚠️ Requires data alignment (aligned loads faster). Misaligned loads = penalties.  

---

# 7. Software Prefetching (Hiding Memory Latency)

### Idea:
- Bring cache lines into L1/L2/L3 *before* they are needed.  
- Overlaps memory access latency with computation by issuing non-blocking cache line requests ahead of actual usage.

```cpp
for (int i=0; i<N; i++) {
    _mm_prefetch((char*)&A[i+8], _MM_HINT_T0); // Prefetch 8 ahead
    process(A[i]);
}
```
### Prefetch distance calculation:
```bash
    Optimal_PD = ⌈(Memory_Latency_Cycles) / (Cycles_Per_Loop_Iteration)⌉
```
### Cache Fill Levels:
- **T0**: Prefetch into L1 (fastest, but may pollute).  
- **T1**: Prefetch into L2.  
- **T2**: Prefetch into L3.  
- **NTA**: Non-temporal → bypass caches (good for streaming).  

### Latencies:
- L1: ~2–4 cycles  
- L2: ~12 cycles  
- L3: ~30–40 cycles  
- DRAM: ~200 cycles  

### Prefetch Distance (PD):
- Too small (PD=2) → can’t hide DRAM latency (~200 cycles).  
- Too large (PD=64) → fetched too early, evicted → **cache pollution**.  
- Sweet spot: **PD ≈ 6–12** (depends on workload). 
- Rule: MaxPrefetches ≤ 0.7 × Available MSHRs 

👉 **Effect:** Overlaps **memory-level parallelism (MLP)** with computation.  

⚠️ Excessive prefetch = thrashing. Limited by **MSHRs** (Miss Status Holding Registers).  

---

# 8. Types of Cache Misses

- **Compulsory miss:** first-time access.  
- **Capacity miss:** working set > cache size.  
- **Conflict miss:** two arrays map to same cache set.  

### Optimizations vs Miss Types:
- **Reordering** → reduces conflict misses.  
- **Tiling** → reduces capacity misses.  
- **Unrolling & SIMD** → reduce compulsory stalls, expose ILP/DLP.  
- **Prefetching** → hides latency, avoids stall cycles.  

---

# ✅ Final Takeaways

- **Loop Reordering** → improves **spatial locality**.  
- **Loop Unrolling** → improves **ILP**, uses superscalar execution.  
- **Tiling** → improves **temporal locality**, fits reuse into cache.  
- **SIMD** → improves **DLP**, processes multiple elements per instruction.  
- **Prefetching** → overlaps **MLP**, hides DRAM latency if PD tuned.  
