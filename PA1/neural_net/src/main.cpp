#include <iostream>

#include "performance_benchmark.h"

using namespace std;

int counter =0 ;

int main(){

	// Run matrix multiplication benchmark
	PerformanceBenchmark::benchmarkMatrixMultiplication(128);
	
	Matrix A(1024, 1024);
	A.xavierInit();
	// A.print("Matrix A:");

	cout<<"Matrix multiplication benchmark completed."<<endl;

	// Run neural network benchmark
	PerformanceBenchmark::benchmarkNN();

	return 0;
}
		
