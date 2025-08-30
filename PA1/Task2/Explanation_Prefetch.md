MSHR Limitations in Intel i5-12400:
L1 MSHR: ~10-12 entries per core

L2 MSHR: ~16-20 entries per core

L3 MSHR: ~32-48 entries (shared)

Memory Controller: ~64-128 outstanding requests


Cache Latencies (Average Case):
L1 Data Cache: 2 cycles (hit)

L2 Cache: 12 cycles (hit)

L3 (LLC) Cache: 30 cycles (hit)

DRAM Memory: 200 cycles (average access)


Forward Path (CPU → DRAM):
text
CPU _mm_prefetch() → L1 miss → L2 miss → L3 miss → DRAM request

Timeline:
- Prefetch instruction issue: 0 cycles
- L1 miss detection: 2 cycles
- L2 miss detection: 2 + 12 = 14 cycles  
- L3 miss detection: 14 + 30 = 44 cycles
- DRAM access begins: 44 + 200 = 244 cycles


Reverse Path (DRAM → Cache Fill):
text
DRAM data return → Cache hierarchy fill

For L3 Prefetch (_MM_HINT_T2):
- Data available in L3: 200 + 30 = 230 cycles

For L2 Prefetch (_MM_HINT_T1):  
- Data fills L3: 230 cycles
- Data available in L2: 230 + 12 = 242 cycles

For L1 Prefetch (_MM_HINT_T0):
- Data fills L3: 230 cycles
- Data fills L2: 242 cycles  
- Data available in L1: 242 + 2 = 244 cycles


Effective Prefetch Completion Times:
L1 Prefetch: 244 cycles (complete pipeline)

L2 Prefetch: 242 cycles

L3 Prefetch: 230 cycles

2. Loop Iteration Timing Recalculation
Your Embedding Loop Breakdown:
cpp
for (int j = start_idx; j < end_idx; ++j) {
    // Prefetch: 1-2 cycles
    _mm_prefetch((char*)prefetch_ptr, _MM_HINT_T1);
    
    // Address calculation: 1-2 cycles
    const float* data_ptr = &embedding_table[input[j] * embedding_dim];
    
    // Inner loop (128 dimensions): 100-130 cycles (cache dependent)
    for (int d = 0; d < embedding_dim; ++d) {
        bag_embedding[d] += data_ptr[d];
    }
}
Average Iteration Time: ~110-140 cycles per iteration (mixed cache performance)

