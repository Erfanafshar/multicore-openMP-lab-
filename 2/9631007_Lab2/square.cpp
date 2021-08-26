#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>

double starttime, elapsedtime;
const int squareSize = 5;

typedef struct {
	int *A, *B, *C;
	int n, m;
} DataSet;

void fillDataSet(DataSet *dataSet);
void add(DataSet dataSet);
void printDataSet(DataSet dataSet);
void printElapsedTime();
void closeDataSet(DataSet dataSet);

int main(void) {
	DataSet dataSet;

	printf("enter matrix dimension : ");
	scanf("%d %d", &dataSet.n, &dataSet.m);

	fillDataSet(&dataSet);
	add(dataSet);
	// printDataSet(dataSet);
	printElapsedTime();
	closeDataSet(dataSet);
	system("PAUSE");
	return EXIT_SUCCESS;
}

void fillDataSet(DataSet *dataSet) {
	int i, j;

	dataSet->A = (int *)malloc(sizeof(int)* dataSet->n * dataSet->m);
	dataSet->B = (int *)malloc(sizeof(int)* dataSet->n * dataSet->m);
	dataSet->C = (int *)malloc(sizeof(int)* dataSet->n * dataSet->m);

	srand(time(NULL));

	for (i = 0; i < dataSet->n; i++) {
		for (j = 0; j < dataSet->m; j++) {
			dataSet->A[i*dataSet->m + j] = rand() % 100;
			dataSet->B[i*dataSet->m + j] = rand() % 100;
		}
	}
}

void add(DataSet dataSet) {
	int t, i, j;
	starttime = omp_get_wtime();
	int length = dataSet.n / squareSize;
	// loopLenght *= loopLenght;
	// printf("%d \n", loopLenght);

#pragma omp parallel for private(i, j) num_threads(2)
	for (t = 0; t < length * length; t++) {
		int div = t / length;
		int mod = t % length;

		for (i = div * squareSize; i < (div + 1) * squareSize; i++) {
			for (j = mod * squareSize; j < (mod + 1) * squareSize; j++) {
				dataSet.C[i * dataSet.m + j] = dataSet.A[i * dataSet.m + j] + dataSet.B[i * dataSet.m + j];
			}
		}
		
	}
	elapsedtime = omp_get_wtime() - starttime;
}

void printDataSet(DataSet dataSet) {
	int i, j;

	printf("[-] Matrix A\n");
	for (i = 0; i < dataSet.n; i++) {
		for (j = 0; j < dataSet.m; j++) {
			printf("%-4d", dataSet.A[i*dataSet.m + j]);
		}
		putchar('\n');
	}

	printf("[-] Matrix B\n");
	for (i = 0; i < dataSet.n; i++) {
		for (j = 0; j < dataSet.m; j++) {
			printf("%-4d", dataSet.B[i*dataSet.m + j]);
		}
		putchar('\n');
	}

	printf("[-] Matrix C\n");
	for (i = 0; i < dataSet.n; i++) {
		for (j = 0; j < dataSet.m; j++) {
			printf("%-8d", dataSet.C[i*dataSet.m + j]);
		}
		putchar('\n');
	}
}

void printElapsedTime() {
	printf("Time Elapsed: %f Secs\n", elapsedtime);
}

void closeDataSet(DataSet dataSet) {
	free(dataSet.A);
	free(dataSet.B);
	free(dataSet.C);
}


