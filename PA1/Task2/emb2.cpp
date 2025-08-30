#include <iostream>
#include <vector>
#include <random>
#include <iomanip>
#include <chrono>
#include <immintrin.h> 
#include <cstdlib>

using namespace std;
using namespace std::chrono;

const int embedding_table_size = 1000000;
const int embedding_dim = 128;
const int input_size = 720;
const int num_bags = 20;


int random_int(int range) {
    static random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<> dis(0, range - 1);
    return dis(gen);
}

long long run_with_prefetching(const vector<float>& embedding_table, const vector<int>& input, const vector<int>& offsets) {

    auto start = high_resolution_clock::now();

    // --------------------------- Software prefetching (scalar accumulation) ---------------------------
    // Tune these:
    const int prefetch_distance = 8;       // try {2,4,8,16,32}
    const int hint = _MM_HINT_T0;          // try {_MM_HINT_T0, _MM_HINT_T1, _MM_HINT_T2, _MM_HINT_NTA}

    vector<vector<float>> output;
    output.reserve(offsets.size());

    for (size_t i = 0; i < offsets.size(); ++i) {
        int start_idx = offsets[i];
        int end_idx   = (i + 1 < offsets.size()) ? offsets[i + 1] : (int)input.size();

        vector<float> bag_embedding(embedding_dim, 0.0f);

        for (int j = start_idx; j < end_idx; ++j) {
            // Prefetch a future row within the same bag (if available)
            int pfj = j + prefetch_distance;
            if (pfj < end_idx) {
                int pf_row = input[pfj];
                const float* pf_ptr = &embedding_table[(size_t)pf_row * embedding_dim];
                _mm_prefetch((const char*)pf_ptr, _MM_HINT_T0);
            }

            int row = input[j];
            const float* data_ptr = &embedding_table[(size_t)row * embedding_dim];

            // Scalar accumulate (keeps this function focused on prefetching effect)
            for (int d = 0; d < embedding_dim; ++d) {
                bag_embedding[d] += data_ptr[d];
            }
        }

        output.push_back(std::move(bag_embedding));
    }
    // --------------------------------------------------------------------------------------------------

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    cout << "\nTime WITH software prefetching: " << duration.count() << " microseconds.";

    return duration.count();
}

long long run_with_simd(const vector<float>& embedding_table, const vector<int>& input, const vector<int>& offsets) {

    auto start = high_resolution_clock::now();
    
    //--------------------------------start_idx--------------------- Write your code here ----------------------------------------------------------------
    static_assert(embedding_dim % 8 == 0, "embedding_dim must be multiple of 8 for AVX2 path.");

    vector<vector<float>> output;
    output.reserve(offsets.size());

    const int vec_width = 8;                            // AVX2 processes 8 floats per register
    const int num_vecs  = embedding_dim / vec_width;    // 128/8 = 16

    for (size_t i = 0; i < offsets.size(); ++i) {
        int start_idx = offsets[i];
        int end_idx   = (i + 1 < offsets.size()) ? offsets[i + 1] : (int)input.size();

        // 16 accumulators for 128-dim (AVX2)
        __m256 acc[16];
        for (int v = 0; v < num_vecs; ++v) acc[v] = _mm256_setzero_ps();

        for (int j = start_idx; j < end_idx; ++j) {
            int row = input[j];
            const float* base = &embedding_table[(size_t)row * embedding_dim];

            // Load 16 chunks of 8 floats and accumulate
            for (int v = 0; v < num_vecs; ++v) {
                __m256 x = _mm256_loadu_ps(base + v * vec_width);
                acc[v] = _mm256_add_ps(acc[v], x);
            }
        }

        // Store back to scalar buffer
        vector<float> bag_embedding(embedding_dim);
        for (int v = 0; v < num_vecs; ++v) {
            _mm256_storeu_ps(&bag_embedding[v * vec_width], acc[v]);
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
    
    static_assert(embedding_dim % 8 == 0, "embedding_dim must be multiple of 8 for AVX2 path.");

    // Tune these:
    const int prefetch_distance = 8;       // try {2,4,8,16,32}
    const int hint = _MM_HINT_T0;          // try {_MM_HINT_T0, _MM_HINT_T1, _MM_HINT_T2, _MM_HINT_NTA}

    vector<vector<float>> output;
    output.reserve(offsets.size());

    const int vec_width = 8;
    const int num_vecs  = embedding_dim / vec_width;    // 16 for 128-dim

    for (size_t i = 0; i < offsets.size(); ++i) {
        int start_idx = offsets[i];
        int end_idx   = (i + 1 < offsets.size()) ? offsets[i + 1] : (int)input.size();

        __m256 acc[16];
        for (int v = 0; v < num_vecs; ++v) acc[v] = _mm256_setzero_ps();

        for (int j = start_idx; j < end_idx; ++j) {
            // Prefetch a future row (if available)
            int pfj = j + prefetch_distance;
            if (pfj < end_idx) {
                int pf_row = input[pfj];
                const float* pf_ptr = &embedding_table[(size_t)pf_row * embedding_dim];
                _mm_prefetch((const char*)pf_ptr, _MM_HINT_T0);
            }

            int row = input[j];
            const float* base = &embedding_table[(size_t)row * embedding_dim];

            // Vector accumulate
            for (int v = 0; v < num_vecs; ++v) {
                __m256 x = _mm256_loadu_ps(base + v * vec_width);
                acc[v] = _mm256_add_ps(acc[v], x);
            }
        }

        vector<float> bag_embedding(embedding_dim);
        for (int v = 0; v < num_vecs; ++v) {
            _mm256_storeu_ps(&bag_embedding[v * vec_width], acc[v]);
        }

        output.push_back(std::move(bag_embedding));
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
            float* data_ptr = &embedding_table[input[j] * embedding_dim];
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

int main() {
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

    // Run naive code 
    long long time_without_prefetch = naive_emb(embedding_table, input, offsets);
    
    // ---------- Flush Cache Before Running Prefetching ----------
    for (size_t i = 0; i < embedding_table.size(); i += 16) {
        _mm_clflush(&embedding_table[i]);
    }
    _mm_mfence();
    
    // Run emb with software prefetching 
    long long time_with_prefetch = run_with_prefetching(embedding_table, input, offsets);
    // Run emb with simd 
    long long time_with_simd = run_with_simd(embedding_table, input, offsets);
    // Run emb with software prefetching and simd
    long long time_with_prefetch_simd = run_with_prefetching_simd(embedding_table, input, offsets);
    

    // Compute speedup
    double speedup1 = static_cast<double>(time_without_prefetch) / time_with_prefetch;
    double speedup2 = static_cast<double>(time_without_prefetch) / time_with_simd;
    double speedup3 = static_cast<double>(time_without_prefetch) / time_with_prefetch_simd;
    cout << fixed << setprecision(3);
    cout << "\n\nSpeedup (with software prefetching) = " << speedup1 << "x\n";
    cout << "Speedup (with simd) = " << speedup2 << "x\n";
    cout << "Speedup (with software prefetching and simd) = " << speedup3 << "x\n";

    return 0;
}

