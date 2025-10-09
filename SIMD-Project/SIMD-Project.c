// SIMD-Project.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

// Template C to x86 call
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>
#include <time.h>

extern double ymm_vector_add(size_t n, double* vec);

extern double xmm_vector_add(int len, double* A);
extern double x86_vector_add(size_t n, double* vec);
extern void buffer_asm(size_t n, double* vec);
extern double ymm_vector_add_r8(size_t n, double* vec);
extern double ymm_vector_add_rbx(size_t n, double* vec);

//Length of the vector
#define VECTOR_LEN 1073741824

//Vector Sum Double and Len
DOUBLE VectorSum(double A[], int len) {
	
	double ret = 0.0;

	for (int i = 0; i < len; i++) {
		ret += fabs(A[i]);
	}
	return ret;
}

double TestCVectorSum(double A[], int len, size_t testCounts, double PCFreq) {
	double totalTime = 0;
	LARGE_INTEGER start, end, elapse;

	for (int i = 0; i < testCounts; i++) {
		QueryPerformanceCounter(&start);

		VectorSum(A, len);

		QueryPerformanceCounter(&end);
		
		elapse.QuadPart = end.QuadPart - start.QuadPart;
		printf("Test %u: Time in C = %f ms\n", (i+1), elapse.QuadPart * 1000.0 / PCFreq);

		totalTime += elapse.QuadPart * 1000.0 / PCFreq;
	}

	return totalTime / (double)testCounts;
}

//Using YMM Registers Original
double TestYMMVectorSum(double A[], int len, size_t testCounts, double PCFreq) {
	double totalTime = 0;
	LARGE_INTEGER start, end, elapsed;
	double ans[4];

	for (int i = 0; i < testCounts; i++) {
		QueryPerformanceCounter(&start);

		//Vector Sum Function Here
		ymm_vector_add(len, A);

		QueryPerformanceCounter(&end);
		elapsed.QuadPart = (end.QuadPart - start.QuadPart);
		// printf("%llu - %llu = %llu \n", end.QuadPart, start.QuadPart, elapsed.QuadPart);
		printf("Test %u: Time in YMM = %f ms\n", (i + 1), elapsed.QuadPart * 1000.0 / PCFreq);

		totalTime += elapsed.QuadPart * 1000.0 / PCFreq;
	}

	return totalTime / (double)testCounts;
}

//Using YMM Registers
double TestYMMVectorSumR8(double A[], int len, size_t testCounts, double PCFreq) {
	double totalTime = 0;
	LARGE_INTEGER start, end, elapsed;
	double ans[4];

	for (int i = 0; i < testCounts; i++) {
		QueryPerformanceCounter(&start);

		//Vector Sum Function Here
		ymm_vector_add_r8(len, A);

		QueryPerformanceCounter(&end);
		elapsed.QuadPart = (end.QuadPart - start.QuadPart);
		// printf("%llu - %llu = %llu \n", end.QuadPart, start.QuadPart, elapsed.QuadPart);
		printf("Test %u: Time in YMM(r8) = %f ms\n", (i + 1), elapsed.QuadPart * 1000.0 / PCFreq);

		totalTime += elapsed.QuadPart * 1000.0 / PCFreq;
	}

	return totalTime / (double)testCounts;
}
//Using YMM Registers
double TestYMMVectorSumRBX(double A[], int len, size_t testCounts, double PCFreq) {
	double totalTime = 0;
	LARGE_INTEGER start, end, elapsed;
	double ans[4];

	for (int i = 0; i < testCounts; i++) {
		QueryPerformanceCounter(&start);

		//Vector Sum Function Here
		ymm_vector_add_rbx(len, A);

		QueryPerformanceCounter(&end);
		elapsed.QuadPart = (end.QuadPart - start.QuadPart);
		// printf("%llu - %llu = %llu \n", end.QuadPart, start.QuadPart, elapsed.QuadPart);
		printf("Test %u: Time in YMM(rbx) = %f ms\n", (i + 1), elapsed.QuadPart * 1000.0 / PCFreq);

		totalTime += elapsed.QuadPart * 1000.0 / PCFreq;
	}

	return totalTime / (double)testCounts;
}



//Using XMM Registers
//Using XMM Registers
double* TestXMMVectorSum(double A[], int len, size_t testCounts, double PCFreq, double validate) {
	static double retunResult[2]; // static so it persists after function returns
	double totalTime = 0;
	LARGE_INTEGER start, end, elapse;
	double result;

	for (int i = 0; i < testCounts; i++) {
		QueryPerformanceCounter(&start);
		// len = len / 2;

		 //Vector Sum Function Here
		result = xmm_vector_add(len, A);

		QueryPerformanceCounter(&end);

		elapse.QuadPart = end.QuadPart - start.QuadPart;		
		printf("Test %u: Time in XMM = %f ms\n", (i + 1), elapse.QuadPart * 1000.0 / PCFreq);

		totalTime += elapse.QuadPart * 1000.0 / PCFreq;

	}
	retunResult[0] = result;
	retunResult[1] = totalTime / (double)testCounts;
	return retunResult;
}

//Using x86_64 Registers
double TestX86VectorSum(double A[], int len, size_t testCounts, double PCFreq) {
	double totalTime = 0;
	LARGE_INTEGER  start, end, elapse;

	for (int i = 0; i < testCounts; i++) {
		QueryPerformanceCounter(&start);

		//Vector Sum Function Here
		 x86_vector_add(len, A);

		QueryPerformanceCounter(&end);

		elapse.QuadPart = end.QuadPart - start.QuadPart;
			
		printf("Test %u: Time in x86_64 = %f ms\n", (i + 1), elapse.QuadPart * 1000.0 / PCFreq);;

		totalTime += elapse.QuadPart * 1000.0 / PCFreq;
		
	}

	return totalTime / (double)testCounts;
}



//Using x86_64 Registers
double TestASMBuffer(double A[], int len, size_t testCounts, double PCFreq) {
	double totalTime = 0;
	LARGE_INTEGER start, end, elapse;
	double ans[4];

	for (int i = 0; i < testCounts; i++) {
		QueryPerformanceCounter(&start);

		//Buffer Test to get the total buffer time between asm and C
		buffer_asm(len, &A);

		QueryPerformanceCounter(&end);

		elapse.QuadPart = end.QuadPart - start.QuadPart;
		printf("Test %u: Time in x86_64 = %f ms\n", (i + 1), elapse.QuadPart * 1000.0 / PCFreq);

		totalTime += elapse.QuadPart * 1000.0 / PCFreq;
	}

	return totalTime / (double)testCounts;
}

void CheckValueValid(double ref, double val) {

	//validate
	int refInt = round(ref * 1000);
	int valInt = round(val * 1000);

	if (refInt == valInt)
	{
		printf("Vector sum is correct.\n");
	}
	else
	{
		printf("Vector sum is incorrect.\n");
	}
}

int main(int argc, char* argv[]) {
	//Vector array to create
	double *vec = malloc(VECTOR_LEN * sizeof(double));

	//Initialize performace counter
	LARGE_INTEGER li;
	long long int start, end;
	double PCFreq, elapse, elapse1;
	QueryPerformanceFrequency(&li);
	PCFreq = (double)(li.QuadPart);

	//Creates the vector to pass
	for (int i = 0; i < VECTOR_LEN; i++) {
		vec[i] = sin((double)i * 0.0003) * cos((double)i * 0.0007) * 1000.0;// 1.000001;
		//printf("Vector[%u]: %Lf\n", i, vec[i]);
		//printf("%f, ", vec[i]);
	}

	double C_VecSum = VectorSum(vec, VECTOR_LEN);
	double ymmSum = ymm_vector_add(VECTOR_LEN, vec);
	double x86Sum = x86_vector_add(VECTOR_LEN, vec);

	size_t testCounts = 100;
	printf("\nTesting in C\n");
	double aveTimeC = TestCVectorSum(vec, VECTOR_LEN, testCounts, PCFreq);
	
	printf("\nTesting in Buffer\n");
	double aveTimeBuffer = TestASMBuffer(vec, VECTOR_LEN, testCounts, PCFreq);
	
	printf("\nTesting in YMM\n");
	double aveTimeYMM = TestYMMVectorSum(vec, VECTOR_LEN, testCounts, PCFreq);
	//printf("\nTesting in YMM(R8)\n");
	//double aveTimeYMMR8 = TestYMMVectorSumR8(vec, VECTOR_LEN, testCounts, PCFreq);
	//printf("\nTesting in YMM(RBX)\n");
	//double aveTimeYMMRBX = TestYMMVectorSumRBX(vec, VECTOR_LEN, testCounts, PCFreq);

	printf("\nTesting in XMM\n");
	double* aveTimeXMM = TestXMMVectorSum(vec, VECTOR_LEN, testCounts, PCFreq, C_VecSum);
	
	printf("\nTesting in x86_64\n");
	double aveTimeX86 = TestX86VectorSum(vec, VECTOR_LEN, testCounts, PCFreq);
	
	printf("\nOutput after %u tests\n", (int)testCounts);
	printf("\nVector Sum in C: %#.3f\n", C_VecSum);
	CheckValueValid(C_VecSum, C_VecSum);
	printf("Average Time after %u tests in C: %f ms\n", (int)testCounts, aveTimeC);

	printf("\nVector Buffer\n");
	printf("Average Time after %u tests in buffer: %f\n", (int)testCounts, aveTimeBuffer);

	printf("\nVector Sum in YMM: %#.3f\n", ymmSum);
	CheckValueValid(C_VecSum, ymmSum);
	printf("Average Time after %u tests in YMM: %f ms\n", (int)testCounts, aveTimeYMM);
	//printf("Average Time after %u tests in YMM(r8): %f ms\n", (int)testCounts, aveTimeYMMR8);
	//printf("Average Time after %u tests in YMM(rbx): %f ms\n", (int)testCounts, aveTimeYMMRBX);

	printf("\nVector Sum in XMM: %#.3f\n", aveTimeXMM[0]);
	CheckValueValid(C_VecSum, aveTimeXMM[0]);
	printf("Average Time after %u tests in XMM: %f ms\n", (int)testCounts, aveTimeXMM[1]);

	printf("\nVector Sum in x86_64: %#.3f\n", x86Sum);
	CheckValueValid(C_VecSum, x86Sum);
	printf("Average Time after %u tests in x86_64: %f ms\n", (int)testCounts, aveTimeX86);

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
