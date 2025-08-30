# Embedding Operation with Software Prefetch Optimization & SIMD -Embed it

In this part of the assignment we want you to implements an optimised embedding operation with software prefetch instructions and SIMD instructions to improve cache performance and reduce memory latency.
Understand the code for embedding operation given in  `emb.cpp`, and do the necessary changes to incorporate software prefetching & SIMD

## Compilation

To build the optimised embedding implementation:
```bash
make emb
```

## Usage

After compilation, run the optimised embedding operation:
```
./emb
```

## Expected Deliverables 

Make a folder `plots` and add the supporting plots inside that folder. Along with that create a `cache_effect.pdf` reporting and documenting the analysis you did in this part of the assignment.


Good luck embedding the solutions :)




## HW Prefetcher
<!-- HW Prefetcher -->
To Disable HW Prefetcher used msr-tools
```bash
sudo apt-get install msr-tools
sudo modprobe msr
```

To read HW prefetcher status on core 1
```bash
sudo rdmsr -p 1 0x1A4
```

To disable HW prefetcher on core 1 
```bash
sudo wrmsr -p 1 0x1A4 0xF
```

To enable HW prefetcher on core 1
```bash 
sudo wrmsr -p 1 0x1A4 0x3
```

Where 
[0/1] means [Enabled/Disabled] for following bits (MSR 0x1A4 bits):
- Bit 0: L2 Hardware Prefetcher (Streamer)
- Bit 1: L2 Adjacent Cache Line - Prefetcher
- Bit 2: L1 DCU Hardware Prefetcher
- Bit 3: L1 DCU IP Prefetcher

Initially the value was 0x3 means (0011)
means 
- L2 Hardware Prefetcher - Disabled
- L2 Adjacent Cache Line Prefetcher - Disabled
- L1 DCU Hardware Prefetcher - Enabled
- L1 DCU IP Prefetcher - Enabled

