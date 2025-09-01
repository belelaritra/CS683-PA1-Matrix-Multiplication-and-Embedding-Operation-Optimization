#include "matrix_operation.h"
#include <immintrin.h>

Matrix MatrixOperation::NaiveMatMul(const Matrix &A, const Matrix &B) {
	size_t n = A.getRows();
	size_t k = A.getCols();
	size_t m = B.getCols();

	if (k != B.getRows()) {
		throw std::invalid_argument("Matrix dimensions don't match for multiplication");
	}
	
	
	Matrix C(n,m);
	
	for(int i = 0; i < n ; i++) {
		for (int j = 0 ; j< m ; j++) {
			for(int l = 0; l < k; l++) {
				C(i,j) += A(i,l) * B(l,j);
			}
		}
	}
	
	return C;
}

// Loop reordered matrix multiplication (ikj order for better cache locality)
Matrix MatrixOperation::ReorderedMatMul(const Matrix& A, const Matrix& B) {
	size_t n = A.getRows();
	size_t k = A.getCols();
	size_t m = B.getCols();

	if (k != B.getRows()) {
		throw std::invalid_argument("Matrix dimensions don't match for multiplication");
	}
	
	
	Matrix C(n,m);
	
//----------------------------------------------------- Write your code here ----------------------------------------------------------------
    

//-------------------------------------------------------------------------------------------------------------------------------------------

for (size_t i = 0; i < n; i++) {
	for (size_t l = 0; l < k; l++) {
		float a_val = A(i, l);
		for (size_t j = 0; j < m; j++) {
			C(i, j) += a_val * B(l, j);
		}
	}
}

	return C;
}

// Loop unrolled matrix multiplication
Matrix MatrixOperation::UnrolledMatMul(const Matrix& A, const Matrix& B) {
	size_t n = A.getRows();
    size_t k = A.getCols();
    size_t m = B.getCols();

    if (k != B.getRows()) {
        throw std::invalid_argument("Matrix dimensions don't match for multiplication");
    }

    Matrix C(n, m);

    const int UNROLL = 4;
//----------------------------------------------------- Write your code here ----------------------------------------------------------------
    

//-------------------------------------------------------------------------------------------------------------------------------------------

for (size_t i = 0; i < n; i++) {
	for (size_t j = 0; j < m; j++) {
		float sum = 0.0f;
		size_t l = 0;

		// Unrolled loop
		for (; l + UNROLL <= k; l += UNROLL) {
			sum += A(i, l)     * B(l, j)
				 + A(i, l + 1) * B(l + 1, j)
				 + A(i, l + 2) * B(l + 2, j)
				 + A(i, l + 3) * B(l + 3, j);
		}

		// Remaining
		for (; l < k; l++) {
			sum += A(i, l) * B(l, j);
		}

		C(i, j) = sum;
	}
}
    return C;
}

// Tiled (blocked) matrix multiplication for cache efficiency
Matrix MatrixOperation::TiledMatMul(const Matrix& A, const Matrix& B) {
	size_t n = A.getRows();
    size_t k = A.getCols();
    size_t m = B.getCols();

    if (k != B.getRows()) {
        throw std::invalid_argument("Matrix dimensions don't match for multiplication");
    }

    Matrix C(n, m);
    const int T = 32;   // tile size
	size_t i_max = 0;
	size_t k_max = 0;
	size_t j_max = 0;
//----------------------------------------------------- Write your code here ----------------------------------------------------------------
    

//-------------------------------------------------------------------------------------------------------------------------------------------
for (size_t ii = 0; ii < n; ii += T) {
    for (size_t kk = 0; kk < k; kk += T) {
        for (size_t jj = 0; jj < m; jj += T) {
            
            size_t i_max = std::min(ii + T, n);
            size_t k_max = std::min(kk + T, k);
            size_t j_max = std::min(jj + T, m);

            for (size_t i = ii; i < i_max; i++) {
                for (size_t l = kk; l < k_max; l++) {
                    float a_val = A(i, l);
                    for (size_t j = jj; j < j_max; j++) {
                        C(i, j) += a_val * B(l, j);
                    }
                }
            }
        }
    }
}

    return C;
}

// SIMD vectorized matrix multiplication (using AVX2)
Matrix MatrixOperation::VectorizedMatMul(const Matrix& A, const Matrix& B) {
	size_t n = A.getRows();
    size_t k = A.getCols();
    size_t m = B.getCols();

    if (k != B.getRows()) {
        throw std::invalid_argument("Matrix dimensions don't match for multiplication");
    }

    Matrix C(n, m);
//----------------------------------------------------- Write your code here ----------------------------------------------------------------
    

//-------------------------------------------------------------------------------------------------------------------------------------------
for (size_t i = 0; i < n; i++) {
	for (size_t j = 0; j < m; j++) {
		__m256d sum_vec = _mm256_setzero_pd(); // 4 doubles
		size_t l = 0;

		for (; l + 4 <= k; l += 4) {
			__m256d a_vec = _mm256_loadu_pd(&A(i, l));
			__m256d b_vec = _mm256_loadu_pd(&B(l, j)); // careful: access pattern!
			__m256d mul   = _mm256_mul_pd(a_vec, b_vec);
			sum_vec = _mm256_add_pd(sum_vec, mul);
		}

		double temp[4];
		_mm256_storeu_pd(temp, sum_vec);
		double sum = temp[0] + temp[1] + temp[2] + temp[3];

		for (; l < k; l++) {
			sum += A(i, l) * B(l, j);
		}

		C(i, j) = sum;
	}
}
    return C;
}

// Optimized matrix transpose
Matrix MatrixOperation::Transpose(const Matrix& A) {
	size_t rows = A.getRows();
	size_t cols = A.getCols();
	Matrix result(cols, rows);

	for (size_t i = 0; i < rows; ++i) {
		for (size_t j = 0; j < cols; ++j) {
			result(j, i) = A(i, j);
		}
	}

	// Optimized transpose using blocking for better cache performance
	// This is a simple implementation, more advanced techniques can be applied
	// Write your code here and commnent the above code
//----------------------------------------------------- Write your code here ----------------------------------------------------------------
    

//-------------------------------------------------------------------------------------------------------------------------------------------
const int T = 32; // block size

for (size_t i = 0; i < rows; i += T) {
	for (size_t j = 0; j < cols; j += T) {
		size_t i_max = std::min(i + T, rows);
		size_t j_max = std::min(j + T, cols);

		for (size_t ii = i; ii < i_max; ii++) {
			for (size_t jj = j; jj < j_max; jj++) {
				result(jj, ii) = A(ii, jj);
			}
		}
	}
}
	
	return result;
}
