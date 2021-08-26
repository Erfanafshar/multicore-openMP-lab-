#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>

double starttime, elapsedtime;


void fill_array(int *a, size_t n);

void prefix_sum(int *a, size_t n);

void print_array(int *a, size_t n);

void printElapsedTime();


int main(int argc, char *argv[]) {
    omp_set_num_threads(2);
    size_t n = 0;
    printf("[-] Please enter N: ");
    scanf("%uld\n", &n);
    int *a = (int *) malloc(n * sizeof a);
    fill_array(a, n);
//    print_array(a, n);
    prefix_sum(a, n);
//    print_array(a, n);
    free(a);
    printElapsedTime();
    return EXIT_SUCCESS;
}

void prefix_sum(int *a, size_t n) {
    int i;
    int f_time = 1;
    starttime = omp_get_wtime();
//#pragma omp parallel for schedule(static)
    for (i = 1; i < n; ++i) {
        int ID = omp_get_thread_num();
//        printf("%d, ", ID);
        if (ID == 1 && f_time == 1) {
            a[i] = a[i];
            f_time = 0;
        } else {
            a[i] = a[i] + a[i - 1];
        }
    }
    printf("\n");
    for (i = n / 2 + 1; i < n; ++i) {
        a[i] = a[i] + a[n / 2];
    }
    elapsedtime = omp_get_wtime() - starttime;
}

void print_array(int *a, size_t n) {
    int i;
    printf("[-] array: ");
    for (i = 0; i < n; ++i) {
        printf("%d, ", a[i]);
    }
    printf("\b\b  \n");
}

void fill_array(int *a, size_t n) {
    int i;
    for (i = 0; i < n; ++i) {
        a[i] = i + 1;
    }
}

void printElapsedTime() {
    printf("Time Elapsed: %f Secs\n", elapsedtime);
}