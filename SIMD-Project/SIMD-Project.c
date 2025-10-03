// SIMD-Project.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

// Template C to x86 call
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>

extern void ymm_vector_add();
extern void xmm_vector_add();
extern void x86_vector_add();

//Length of the vector
#define VECTOR_LEN 8

//Vector Sum Double and Len
double VectorSum(double A[], int len) {
	double ret = 0.0;
	for (int i = 0; i < len; i++) {
		ret += abs(A[i]);
	}
	return ret;
}

int main(int argc, char* argv[]) {
	//Vector array to create
	double vec[VECTOR_LEN];

	//Creates the vector to pass
	for (int i = 0; i < VECTOR_LEN; i++) {
		vec[i] = sin((double)i * 0.0003) * cos((double)i * 0.0007) * 1000.0;
		printf("Vector[%u]: %f\n", i, vec[i]);
	}

	double c_sum = VectorSum(vec, VECTOR_LEN);
	printf("Vec sum in C: %f\n", c_sum);

	ymm_vector_add();
	xmm_vector_add();
	x86_vector_add();

	return 0;
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
