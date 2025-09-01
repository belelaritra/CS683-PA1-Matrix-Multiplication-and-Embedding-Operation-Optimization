/*******************************************************************
 * Author: <Name1>, <Name2>
 * Date: <Date>
 * File: mat_mul.c
 * Description: This file contains implementations of matrix multiplication
 *			    algorithms using various optimization techniques.
 *******************************************************************/

// PA 1: Matrix Multiplication
/// try sse vs avx as well 
// includes
#include <stdio.h>
#include <stdlib.h>         // for malloc, free, atoi
#include <time.h>           // for time()
#include <chrono>	        // for timing
#include <xmmintrin.h> 		// for SSE
#include <immintrin.h>		// for AVX

#include "helper.h"			// for helper functions
/// compare 
// defines
// NOTE: you can change this value as per your requirement
#define TILE_SIZE 256	// size of the tile for blocking

/**
 * @brief 		Performs matrix multiplication of two matrices.
 * @param 		A 			pointer to the first matrix
 * @param 		B 			pointer to the second matrix
 * @param 		C 			pointer to the resultant matrix
 * @param 		size 		dimension of the matrices
 */
void naive_mat_mul(double *A, double *B, double *C, int size) {

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			for (int k = 0; k < size; k++) {
				C[i * size + j] += A[i * size + k] * B[k * size + j];
			}
		}
	}
}

/**
 * @brief 		Task 1A: Performs matrix multiplication of two matrices using loop optimization.
 * @param 		A 			pointer to the first matrix
 * @param 		B 			pointer to the second matrix
 * @param 		C 			pointer to the resultant matrix
 * @param 		size 		dimension of the matrices
 */
void loop_opt_mat_mul(double *A, double *B, double *C, int size){
//----------------------------------------------------- Write your code here ----------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------------------------------------------
for (int i = 0; i < size; i++) {
	for (int k = 0; k < size; k++) {
		double a = A[i * size + k];
		int j = 0;
		// unroll by 4
		for (; j < size; j++) {
			C[i * size + j    ] += a * B[k * size + j    ];
			 C[i * size + j + 1] += a * B[k * size + j + 1];
			 C[i * size + j + 2] += a * B[k * size + j + 2];
			C[i * size + j + 3] += a * B[k * size + j + 3];
		}
		for (; j < size; j++) {
			C[i * size + j] += a * B[k * size + j];
		}
	}
}


/// k-i-j
// for (int k = 0; k < size; k++) {
// 	for (int i = 0; i < size; i++) {
// 		double a = A[i * size + k];
// 		for (int j = 0; j < size; j++) {
// 			C[i * size + j] += a * B[k * size + j];
// 		}
// 	}
// }


}


/**
 * @brief 		Task 1B: Performs matrix multiplication of two matrices using tiling.
 * @param 		A 			pointer to the first matrix
 * @param 		B 			pointer to the second matrix
 * @param 		C 			pointer to the resultant matrix
 * @param 		size 		dimension of the matrices
 * @param 		tile_size 	size of the tile
 * @note 		The tile size should be a multiple of the dimension of the matrices.
 * 				For example, if the dimension is 1024, then the tile size can be 32, 64, 128, etc.
 * 				You can assume that the matrices are square matrices.
*/
void tile_mat_mul(double *A, double *B, double *C, int size, int tile_size) {
//----------------------------------------------------- Write your code here ----------------------------------------------------------------
    

//-------------------------------------------------------------------------------------------------------------------------------------------
 
for (int ii = 0; ii < size; ii += tile_size) {
	for (int jj = 0; jj < size; jj += tile_size) {
		for (int kk = 0; kk < size; kk += tile_size) {

			int i_max = (ii + tile_size > size) ? size : ii + tile_size;
			int j_max = (jj + tile_size > size) ? size : jj + tile_size;
			int k_max = (kk + tile_size > size) ? size : kk + tile_size;

			for (int i = ii; i < i_max; i++) {
				for (int k = kk; k < k_max; k++) {
					double a = A[i * size + k];
					for (int j = jj; j < j_max; j++) {
						C[i * size + j] += a * B[k * size + j];
					}
				}
			}
		}
	}
}

}

/**
 * @brief 		Task 1C: Performs matrix multiplication of two matrices using SIMD instructions.
 * @param 		A 			pointer to the first matrix
 * @param 		B 			pointer to the second matrix
 * @param 		C 			pointer to the resultant matrix
 * @param 		size 		dimension of the matrices
 * @note 		You can assume that the matrices are square matrices.
*/
void simd_mat_mul(double *A, double *B, double *C, int size) {
//----------------------------------------------------- Write your code here ----------------------------------------------------------------
    

//-------------------------------------------------------------------------------------------------------------------------------------------
for (int i = 0; i < size; i++) {
    for (int k = 0; k < size; k++) {
        __m256d a_vec = _mm256_set1_pd(A[i * size + k]);  // Broadcast scalar A[i,k]
        int j = 0;

        // Process 4 elements at a time
        for (; j <= size - 4; j += 4) {
            __m256d b_vec = _mm256_loadu_pd(&B[k * size + j]);   // Load 4 elements of B
            __m256d c_vec = _mm256_loadu_pd(&C[i * size + j]);   // Load 4 elements of C
            c_vec = _mm256_add_pd(c_vec, _mm256_mul_pd(a_vec, b_vec)); // Multiply + Add
            _mm256_storeu_pd(&C[i * size + j], c_vec);           // Store back result
        }

        // Handle remaining elements (less than 4)
        for (; j < size; j++) {
            C[i * size + j] += A[i * size + k] * B[k * size + j];
        }
    }
}
}

/**
 * @brief 		Task 1D: Performs matrix multiplication of two matrices using combination of tiling/SIMD/loop optimization.
 * @param 		A 			pointer to the first matrix
 * @param 		B 			pointer to the second matrix
 * @param 		C 			pointer to the resultant matrix
 * @param 		size 		dimension of the matrices
 * @param 		tile_size 	size of the tile
 * @note 		The tile size should be a multiple of the dimension of the matrices.
 * @note 		You can assume that the matrices are square matrices.
*/
void combination_mat_mul(double *A, double *B, double *C, int size, int tile_size) {
//----------------------------------------------------- Write your code here ----------------------------------------------------------------
    
    
//-------------------------------------------------------------------------------------------------------------------------------------------
for (int ii = 0; ii < size; ii += tile_size) {
	for (int jj = 0; jj < size; jj += tile_size) {
		for (int kk = 0; kk < size; kk += tile_size) {

			int i_max = (ii + tile_size > size) ? size : ii + tile_size;
			int j_max = (jj + tile_size > size) ? size : jj + tile_size;
			int k_max = (kk + tile_size > size) ? size : kk + tile_size;

			for (int i = ii; i < i_max; ++i) {
				// Process j in vector-sized chunks (4 doubles per __m256d)
				int j = jj;
				for (; j <= j_max - 4; j += 4) {
					// load current C vector (accumulator)
					__m256d c_vec = _mm256_loadu_pd(&C[i * size + j]);

					// accumulate over k
					for (int k = kk; k < k_max; ++k) {
						// broadcast A[i,k]
						__m256d a_vec = _mm256_set1_pd(A[i * size + k]);
						// load B[k, j..j+3]
						__m256d b_vec = _mm256_loadu_pd(&B[k * size + j]);
						// c_vec += a_vec * b_vec
						c_vec = _mm256_add_pd(c_vec, _mm256_mul_pd(a_vec, b_vec));
					}

					// store back
					_mm256_storeu_pd(&C[i * size + j], c_vec);
				}

				// Handle scalar remainder columns
				for (; j < j_max; ++j) {
					double c_val = C[i * size + j];
					for (int k = kk; k < k_max; ++k) {
						c_val += A[i * size + k] * B[k * size + j];
					}
					C[i * size + j] = c_val;
				}
			}
		}
	}
} 
}

// NOTE: DO NOT CHANGE ANYTHING BELOW THIS LINE
/**
 * @brief 		Main function
 * @param 		argc 		number of command line arguments
 * @param 		argv 		array of command line arguments
 * @return 		0 on success
 * @note 		DO NOT CHANGE THIS FUNCTION
 * 				DO NOT ADD OR REMOVE ANY COMMAND LINE ARGUMENTS
*/
int main(int argc, char **argv) {

	if ( argc <= 1 ) {
		printf("Usage: %s <matrix_dimension>\n", argv[0]);
		return 0;
	}

	else {
		int size = atoi(argv[1]);

		double *A = (double *)malloc(size * size * sizeof(double));
		double *B = (double *)malloc(size * size * sizeof(double));
		double *C = (double *)calloc(size * size, sizeof(double));

		// initialize random seed
		srand(time(NULL));

		// initialize matrices A and B with random values
		initialize_matrix(A, size, size);
		initialize_matrix(B, size, size);

		// perform normal matrix multiplication
		auto start = std::chrono::high_resolution_clock::now();
		 naive_mat_mul(A, B, C, size);
		 auto end = std::chrono::high_resolution_clock::now();
		 auto time_naive_mat_mul = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	     printf("Normal matrix multiplication took %ld ms to execute \n\n", time_naive_mat_mul);

	#ifdef OPTIMIZE_LOOP_OPT
		// Task 1a: perform matrix multiplication with loop optimization

		// initialize result matrix to 0
		initialize_result_matrix(C, size, size);

		start = std::chrono::high_resolution_clock::now();
		loop_opt_mat_mul(A, B, C, size);
		end = std::chrono::high_resolution_clock::now();
		auto time_loop_mat_mul = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
		printf("Loop optimized matrix multiplication took %ld ms to execute \n", time_loop_mat_mul);
		printf("Normalized performance: %f \n\n", (double)time_naive_mat_mul / time_loop_mat_mul);
	#endif

	#ifdef OPTIMIZE_TILING
		// Task 1b: perform matrix multiplication with tiling

		// initialize result matrix to 0
		initialize_result_matrix(C, size, size);

		start = std::chrono::high_resolution_clock::now();
		tile_mat_mul(A, B, C, size, TILE_SIZE);
		end = std::chrono::high_resolution_clock::now();
		auto time_tiling_mat_mul = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
		printf("Tiling matrix multiplication took %ld ms to execute \n", time_tiling_mat_mul);
		printf("Normalized performance: %f \n\n", (double)time_naive_mat_mul / time_tiling_mat_mul);
	#endif

	#ifdef OPTIMIZE_SIMD
		// Task 1c: perform matrix multiplication with SIMD instructions 

		// initialize result matrix to 0
		initialize_result_matrix(C, size, size);

		start = std::chrono::high_resolution_clock::now();
		simd_mat_mul(A, B, C, size);
		end = std::chrono::high_resolution_clock::now();
		auto time_simd_mat_mul = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

		printf("SIMD matrix multiplication took %ld ms to execute \n", time_simd_mat_mul);
		printf("Normalized performance: %f \n\n", (double)time_naive_mat_mul / time_simd_mat_mul);
	#endif

	#ifdef OPTIMIZE_COMBINED
		// Task 1d: perform matrix multiplication with combination of tiling, SIMD and loop optimization

		// initialize result matrix to 0
		initialize_result_matrix(C, size, size);

		start = std::chrono::high_resolution_clock::now();
		combination_mat_mul(A, B, C, size, TILE_SIZE);
		end = std::chrono::high_resolution_clock::now();
		auto time_combination = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
		printf("Combined optimization matrix multiplication took %ld ms to execute \n", time_combination);
		 printf("Normalized performance: %f \n\n", (double)time_naive_mat_mul / time_combination);
	#endif

		// free allocated memory
		free(A);
		free(B);
		free(C);

		return 0;
	}
}
