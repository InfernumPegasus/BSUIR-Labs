#include "stdio.h"
#include "windows.h"

#define SIZE 10
float array[SIZE];
float result = 0.0;
float amount = (float)SIZE;

void inputArray();
void outputArray();
void asmAlgorithm();
float getAverage();

int main() {
	inputArray();
	printf("Input array: \n");
	outputArray();

	asmAlgorithm();

	printf("\nResult of FPU: %.3f\n", result);
	printf("Result of non-FPU: %.3f\n", getAverage());
	
	return 0;
}

float getAverage() {
	float sum = 0;
	for (int i = 0; i < SIZE; i++) {
		sum += array[i];
	}
	return sum / SIZE;
}

void inputArray() {
	int res;
	printf("Input 10 elements: \n");

	for (int i = 0; i < SIZE; ++i) {
		do {
			printf("Array[%d] : ", i);
			res = scanf("%f", &array[i]);
			while (getchar() != '\n');
			if (res != 1) printf("Invalid input\n");
		} while (res != 1);
	}
}

void outputArray() {
	for (int i = 0; i < SIZE; ++i) {
		printf("%.3f ", array[i]);
	}
}

void asmAlgorithm() {
	asm {
		finit
		mov cx, SIZE
		lea bx, array
	}
	calculate_loop:
	asm {
		fld dword ptr [bx]
		fadd result
		fstp result

		add bx, 4
		loop calculate_loop
	}
	end:
	asm {
		fld result		// по итогу st(1)
		fld amount		// st(0)
		fdivp
		fstp result
		fwait
	}
}
