// SIMD-Project.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

// Template C to x86 call
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>
#include <time.h>

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

double TestCVectorSum(double A[], int len, size_t testCounts, double PCFreq) {
	double totalTime = 0;
	LARGE_INTEGER li = {};
	long long int start, end;
	double elapse;

	for (int i = 0; i < testCounts; i++) {
		QueryPerformanceCounter(&li);
		start = li.QuadPart;

		VectorSum(A, len);

		QueryPerformanceCounter(&li);
		end = li.QuadPart;

		elapse = ((double)(end - start)) * 1000.0 / PCFreq;
		printf("Test %u: Time in C = %f ms\n", (i+1), elapse);

		totalTime += elapse;
	}

	return totalTime / (double)testCounts;
}

//Using YMM Registers
double TestYMMVectorSum(double A[], int len, size_t testCounts, double PCFreq) {
	double totalTime = 0;
	LARGE_INTEGER li = {};
	long long int start, end;
	double elapse;

	for (int i = 0; i < testCounts; i++) {
		QueryPerformanceCounter(&li);
		start = li.QuadPart;

		//Vector Sum Function Here
		ymm_vector_add();

		QueryPerformanceCounter(&li);
		end = li.QuadPart;

		elapse = ((double)(end - start)) * 1000.0 / PCFreq;
		printf("Test %u: Time in YMM = %f ms\n", (i + 1), elapse);

		totalTime += elapse;
	}

	return totalTime / (double)testCounts;
}

//Using XMM Registers
double TestXMMVectorSum(double A[], int len, size_t testCounts, double PCFreq) {
	double totalTime = 0;
	LARGE_INTEGER li = {};
	long long int start, end;
	double elapse;

	for (int i = 0; i < testCounts; i++) {
		QueryPerformanceCounter(&li);
		start = li.QuadPart;

		//Vector Sum Function Here
		xmm_vector_add();

		QueryPerformanceCounter(&li);
		end = li.QuadPart;

		elapse = ((double)(end - start)) * 1000.0 / PCFreq;
		printf("Test %u: Time in XMM = %f ms\n", (i + 1), elapse);

		totalTime += elapse;
	}

	return totalTime / (double)testCounts;
}

//Using x86_64 Registers
double TestX86VectorSum(double A[], int len, size_t testCounts, double PCFreq) {
	double totalTime = 0;
	LARGE_INTEGER li = {};
	long long int start, end;
	double elapse;

	for (int i = 0; i < testCounts; i++) {
		QueryPerformanceCounter(&li);
		start = li.QuadPart;

		//Vector Sum Function Here
		x86_vector_add();

		QueryPerformanceCounter(&li);
		end = li.QuadPart;

		elapse = ((double)(end - start)) * 1000.0 / PCFreq;
		printf("Test %u: Time in x86_64 = %f ms\n", (i + 1), elapse);

		totalTime += elapse;
	}

	return totalTime / (double)testCounts;
}

int main(int argc, char* argv[]) {
	//Vector array to create
	double vec[VECTOR_LEN];

	//Initialize performace counter
	LARGE_INTEGER li = {};
	long long int start, end;
	double PCFreq, elapse, elapse1;
	QueryPerformanceFrequency(&li);
	PCFreq = (double)(li.QuadPart);

	//Creates the vector to pass
	for (int i = 0; i < VECTOR_LEN; i++) {
		vec[i] = sin((double)i * 0.0003) * cos((double)i * 0.0007) * 1000.0;
		printf("Vector[%u]: %f\n", i, vec[i]);
	}

	double C_VecSum = VectorSum(vec, VECTOR_LEN);

	size_t testCounts = 30;
	printf("\nTesting in C\n");
	double aveTimeC = TestCVectorSum(vec, VECTOR_LEN, testCounts, PCFreq);
	printf("\nTesting in YMM\n");
	double aveTimeYMM = TestYMMVectorSum(vec, VECTOR_LEN, testCounts, PCFreq);
	printf("\nTesting in XMM\n");
	double aveTimeXMM = TestXMMVectorSum(vec, VECTOR_LEN, testCounts, PCFreq);
	printf("\nTesting in x86_64\n");
	double aveTimeX86 = TestX86VectorSum(vec, VECTOR_LEN, testCounts, PCFreq);
	
	printf("\nOutput after %u tests\n", (int)testCounts);
	printf("\nVector Sum in C: %f\n", C_VecSum);
	printf("Average Time after %u tests in C: %f\n", (int)testCounts, aveTimeC);

	printf("\nVector Sum in YMM: %f\n", 0.f);
	printf("Average Time after %u tests in YMM: %f\n", (int)testCounts, aveTimeYMM);

	printf("\nVector Sum in XMM: %f\n", 0.f);
	printf("Average Time after %u tests in XMM: %f\n", (int)testCounts, aveTimeXMM);

	printf("\nVector Sum in x86_64: %f\n", 0.f);
	printf("Average Time after %u tests in x86_64: %f\n", (int)testCounts, aveTimeX86);

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
