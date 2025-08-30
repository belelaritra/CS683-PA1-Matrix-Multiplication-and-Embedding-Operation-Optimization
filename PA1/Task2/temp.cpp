#include <immintrin.h>
#include <iostream>
int main() {
#ifdef __AVX__
    std::cout << "AVX supported (256-bit SIMD)\n";
#endif
#ifdef __AVX2__
    std::cout << "AVX2 supported (256-bit SIMD + integer ops)\n";
#endif
#ifdef __SSE__
    std::cout << "SSE supported (128-bit SIMD)\n";
#endif
}
