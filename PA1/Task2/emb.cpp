#include <iostream>
#include <vector>
#include <random>
#include <iomanip>
#include <chrono>
#include <immintrin.h> 
#include <cstdlib>

using namespace std;
using namespace std::chrono;

const int embedding_table_size = 1000000; // Size : 100K | 500K | 1M | 2M | 5M
const int embedding_dim = 128;            // Dim :  64 | 128 | 256 | 512 | 1024
const int input_size = 720;
const int num_bags = 20;


const int PREFETCH_DISTANCE = 8;    // PD : 2 | 4 | 8 | 16 | 32 | 64
const int CACHE_LEVEL = 1;          // CL : 0 | 1 | 2 | 3 
const int SIMD_WIDTH = 128;         // SIMD : 128 | 256

int random_int(int range) {
    static random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<> dis(0, range - 1);
    return dis(gen);
}

long long run_with_prefetching(const vector<float>& embedding_table, const vector<int>& input, const vector<int>& offsets) {

    auto start = high_resolution_clock::now();
    
    //----------------------------------------------------- Write your code here ----------------------------------------------------------------
    
    vector<vector<float>> output;

    for (size_t i = 0; i < offsets.size(); ++i) { // Runs for `num_bags` = 20 times
        int start_idx = offsets[i];
        int end_idx = (i + 1 < offsets.size()) ? offsets[i + 1] : input.size();
        
        vector<float> bag_embedding(embedding_dim, 0.0f);
        
        for (int j = start_idx; j < end_idx; ++j) { // Runs for `bag_size` = (input_size / num_bags = 720/20) 36 times
            // -------------- PREFETCH PART START --------------
            if (j + PREFETCH_DISTANCE < end_idx) {
                const float* prefetch_ptr = &embedding_table[input[j + PREFETCH_DISTANCE] * embedding_dim];
                switch(CACHE_LEVEL) {
                    case 0: _mm_prefetch((char*)prefetch_ptr, _MM_HINT_T0); break; // L1
                    case 1: _mm_prefetch((char*)prefetch_ptr, _MM_HINT_T1); break; // L2
                    case 2: _mm_prefetch((char*)prefetch_ptr, _MM_HINT_T2); break; // L3
                    case 3: _mm_prefetch((char*)prefetch_ptr, _MM_HINT_NTA); break; // Non-temporal
                }
            }
            // -------------- PREFETCH PART END --------------
            const float* data_ptr = &embedding_table[input[j] * embedding_dim];
            for (int d = 0; d < embedding_dim; ++d) {
                bag_embedding[d] += data_ptr[d];
            }
        }
        
        output.push_back(bag_embedding);
    }

    
    //-------------------------------------------------------------------------------------------------------------------------------------------
    
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    cout << "\nTime WITH software prefetching: " << duration.count() << " microseconds.";

    return duration.count();
}

long long run_with_simd(const vector<float>& embedding_table, const vector<int>& input, const vector<int>& offsets) {

    auto start = high_resolution_clock::now();
    
    //----------------------------------------------------- Write your code here ----------------------------------------------------------------
    vector<vector<float>> output;

    for (size_t i = 0; i < offsets.size(); ++i) {
        int start_idx = offsets[i];
        int end_idx = (i + 1 < offsets.size()) ? offsets[i + 1] : input.size();
        
        vector<float> bag_embedding(embedding_dim, 0.0f);  // No padding needed
        
        for (int j = start_idx; j < end_idx; ++j) {
            if (input[j] >= embedding_table.size() / embedding_dim) {
                continue; // Bounds checking
            }
            
            const float* data_ptr = &embedding_table[input[j] * embedding_dim];
            
            int d = 0;
            
            // AVX (256-bit SIMD) for 8 floats at a time (unaligned only)
            if (SIMD_WIDTH >= 256) {
                for (; d <= embedding_dim - 8; d += 8) {
                    __m256 bag_vec = _mm256_loadu_ps(&bag_embedding[d]);
                    __m256 data_vec = _mm256_loadu_ps(&data_ptr[d]);
                    __m256 result = _mm256_add_ps(bag_vec, data_vec);
                    _mm256_storeu_ps(&bag_embedding[d], result);
                }
            }
            
            // SSE128: Process 4 floats at a time (unaligned only)
            else if (SIMD_WIDTH >= 128) {
                for (; d <= embedding_dim - 4; d += 4) {
                    __m128 bag_vec = _mm_loadu_ps(&bag_embedding[d]);
                    __m128 data_vec = _mm_loadu_ps(&data_ptr[d]);
                    __m128 result = _mm_add_ps(bag_vec, data_vec);
                    _mm_storeu_ps(&bag_embedding[d], result);
                }
            }
            
            // Scalar fallback for remaining elements
            for (; d < embedding_dim; ++d) {
                bag_embedding[d] += data_ptr[d];
            }
        }
        
        output.push_back(std::move(bag_embedding));
    }

    //-------------------------------------------------------------------------------------------------------------------------------------------
    
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    cout << "\nTime WITH SIMD: " << duration.count() << " microseconds.";

    return duration.count();
}


long long run_with_prefetching_simd(const vector<float>& embedding_table, const vector<int>& input, const vector<int>& offsets) {

    auto start = high_resolution_clock::now();
    
    //----------------------------------------------------- Write your code here ----------------------------------------------------------------
    
    vector<vector<float>> output;

    for (size_t i = 0; i < offsets.size(); ++i) {
        int start_idx = offsets[i];
        int end_idx = (i + 1 < offsets.size()) ? offsets[i + 1] : input.size();
        
        vector<float> bag_embedding(embedding_dim, 0.0f);
        
        for (int j = start_idx; j < end_idx; ++j) {
            if (j + PREFETCH_DISTANCE < end_idx) {
                const float* prefetch_ptr = &embedding_table[input[j + PREFETCH_DISTANCE] * embedding_dim];
                switch(CACHE_LEVEL) {
                    case 0: _mm_prefetch((char*)prefetch_ptr, _MM_HINT_T0); break;
                    case 1: _mm_prefetch((char*)prefetch_ptr, _MM_HINT_T1); break;
                    case 2: _mm_prefetch((char*)prefetch_ptr, _MM_HINT_T2); break;
                    case 3: _mm_prefetch((char*)prefetch_ptr, _MM_HINT_NTA); break;
                }
            }
            
            const float* data_ptr = &embedding_table[input[j] * embedding_dim];
            int d = 0;
            
            // AVX (256-bit SIMD) for 8 floats at a time
            if (SIMD_WIDTH >= 256) {
                for (; d <= embedding_dim - 8; d += 8) {
                    __m256 bag_vec = _mm256_loadu_ps(&bag_embedding[d]);
                    __m256 data_vec = _mm256_loadu_ps(&data_ptr[d]);
                    __m256 result = _mm256_add_ps(bag_vec, data_vec);
                    _mm256_storeu_ps(&bag_embedding[d], result);
                }
            }

            // SSE128: Process 4 floats at a time (unaligned only)
            else if (SIMD_WIDTH >= 128) {
                for (; d <= embedding_dim - 4; d += 4) {
                    __m128 bag_vec = _mm_loadu_ps(&bag_embedding[d]);
                    __m128 data_vec = _mm_loadu_ps(&data_ptr[d]);
                    __m128 result = _mm_add_ps(bag_vec, data_vec);
                    _mm_storeu_ps(&bag_embedding[d], result);
                }
            }
        
            // Scalar fallback for remaining elements
            for (; d < embedding_dim; ++d) {
                bag_embedding[d] += data_ptr[d];
            }
            
        }
        
        output.push_back(bag_embedding);
    }

    
    //-------------------------------------------------------------------------------------------------------------------------------------------
    
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    cout << "\nTime WITH software prefetching and SIMD: " << duration.count() << " microseconds.";

    return duration.count();
}


long long naive_emb(vector<float>& embedding_table, const vector<int>& input, const vector<int>& offsets) {

    auto start = high_resolution_clock::now();
    vector<vector<float>> output;

    for (size_t i = 0; i < offsets.size(); ++i) {
        int start_idx = offsets[i];
        int end_idx = (i + 1 < offsets.size()) ? offsets[i + 1] : input.size();

        vector<float> bag_embedding(embedding_dim, 0.0f);

        for (int j = start_idx; j < end_idx; ++j) {
            const float* data_ptr = &embedding_table[input[j] * embedding_dim];
            for (int d = 0; d < embedding_dim; ++d) {
                bag_embedding[d] += data_ptr[d];
            }
        }

        output.push_back(bag_embedding);
    }

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    cout << "\nTime WITHOUT software prefetching: " << duration.count() << " microseconds.";
    
    return duration.count();
}

int main(int argc, char* argv[]) {
    // Check command line arguments
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " <mode>" << endl;
        cout << "Modes: naive, prefetch, simd, both" << endl;
        return 1;
    }

    string mode = argv[1];
    
    // Prepare embedding table
    vector<float> embedding_table(embedding_table_size * embedding_dim);
    for (auto& val : embedding_table) {
        val = static_cast<float>(random_int(embedding_table_size));
    }

    // Input indices
    vector<int> input(input_size);
    for (auto& idx : input) {
        idx = random_int(embedding_table_size);
    }

    // Offsets
    vector<int> offsets;
    for (int i = 0; i < num_bags; ++i) {
        offsets.push_back((input_size * i) / num_bags);
    }

    
    // Print configuration details
    cout << "\n=== Configuration ===" << endl;
    cout << "Embedding Table Size: " << embedding_table_size << endl;
    cout << "Embedding Dimension: " << embedding_dim << endl;

    // NAIVE MODE
    if (mode == "naive") {
        long long time_naive = naive_emb(embedding_table, input, offsets);
        for (size_t i = 0; i < embedding_table.size(); i += 16) {
            _mm_clflush(&embedding_table[i]);
        }
        _mm_mfence();
        long long time_naive2 = naive_emb(embedding_table, input, offsets);
    }
    // SW PREFETCH MODE
    else if (mode == "prefetch") {
        // Run naive code 
        long long time_without_prefetch = naive_emb(embedding_table, input, offsets);
        
        // ---------- Flush Cache Before Running Prefetching ----------
        for (size_t i = 0; i < embedding_table.size(); i += 16) {
            _mm_clflush(&embedding_table[i]);
        }
        _mm_mfence();

        cout << "Mode: SOFTWARE PREFETCHING" << endl;
        cout << "Prefetch Distance: " << PREFETCH_DISTANCE << endl;
        cout << "Cache Level: " << CACHE_LEVEL;
        switch(CACHE_LEVEL) {
            case 0: cout << " (L1)"; break;
            case 1: cout << " (L2)"; break;
            case 2: cout << " (L3)"; break;
            case 3: cout << " (Non-temporal)"; break;
        }
        cout << endl;

        long long time_with_prefetch = run_with_prefetching(embedding_table, input, offsets);
        double speedup1 = static_cast<double>(time_without_prefetch) / time_with_prefetch;
        cout << "\n\nSpeedup (with software prefetching) = " << speedup1 << "x\n";

    }
    // SIMD MODE
    else if (mode == "simd") {
        long long time_without_prefetch = naive_emb(embedding_table, input, offsets);
        
        // ---------- Flush Cache Before Running ----------
        for (size_t i = 0; i < embedding_table.size(); i += 16) {
            _mm_clflush(&embedding_table[i]);
        }
        _mm_mfence();

        cout << "Mode: SIMD" << endl;
        cout << "SIMD Width: " << SIMD_WIDTH << " bits" << endl;

        long long time_with_simd = run_with_simd(embedding_table, input, offsets);
        double speedup2 = static_cast<double>(time_without_prefetch) / time_with_simd;
        cout << "Speedup (with simd) = " << speedup2 << "x\n";

    }
    // SW PREFETCH + SIMD
    else if (mode == "both") {
        // Run naive code 
        long long time_without_prefetch = naive_emb(embedding_table, input, offsets);
        
        // ---------- Flush Cache Before Running ----------
        for (size_t i = 0; i < embedding_table.size(); i += 16) {
            _mm_clflush(&embedding_table[i]);
        }
        _mm_mfence();

        cout << "Mode: SOFTWARE PREFETCHING + SIMD" << endl;
        cout << "Prefetch Distance: " << PREFETCH_DISTANCE << endl;
        cout << "Cache Level: " << CACHE_LEVEL;
        switch(CACHE_LEVEL) {
            case 0: cout << " (L1)"; break;
            case 1: cout << " (L2)"; break;
            case 2: cout << " (L3)"; break;
            case 3: cout << " (Non-temporal)"; break;
        }
        cout << endl;
        cout << "SIMD Width: " << SIMD_WIDTH << " bits" << endl;

        long long time_with_prefetch_simd = run_with_prefetching_simd(embedding_table, input, offsets);
        double speedup3 = static_cast<double>(time_without_prefetch) / time_with_prefetch_simd;
        cout << "Speedup (with software prefetching and simd) = " << speedup3 << "x\n";

    }
    else {
        cout << "Invalid mode. Choose from: naive, prefetch, simd, both" << endl;
        return 1;
    }

    // -------------- TEMPLATE CODE --------------------
    // // Run emb with software prefetching 
    // long long time_with_prefetch = run_with_prefetching(embedding_table, input, offsets);
    // // Run emb with simd 
    // long long time_with_simd = run_with_simd(embedding_table, input, offsets);
    // // Run emb with software prefetching and simd
    // long long time_with_prefetch_simd = run_with_prefetching_simd(embedding_table, input, offsets);
    

    // // Compute speedup
    // double speedup1 = static_cast<double>(time_without_prefetch) / time_with_prefetch;
    // double speedup2 = static_cast<double>(time_without_prefetch) / time_with_simd;
    // double speedup3 = static_cast<double>(time_without_prefetch) / time_with_prefetch_simd;
    // cout << fixed << setprecision(3);
    // cout << "\n\nSpeedup (with software prefetching) = " << speedup1 << "x\n";
    // cout << "Speedup (with simd) = " << speedup2 << "x\n";
    // cout << "Speedup (with software prefetching and simd) = " << speedup3 << "x\n";

    return 0;
}
