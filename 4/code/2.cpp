#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>
#include <iostream>
#include<algorithm>

double starttime, elapsedtime;
using namespace std;

void fill_array(int *a, int *b, size_t n);

int prefix_sum(int *a, int *b, size_t n, int chk);

void print_array(int *a, int *b, size_t n, int chk);

void printElapsedTime();

int main(int argc, char *argv[]) {
    omp_set_num_threads(2);
    size_t n = 0;
    printf("[-] Please enter N: ");
    scanf("%uld\n", &n);
    int chk = 0;
    int *a = (int *) malloc(n * sizeof a);
    int *b = (int *) malloc(n * sizeof b);
    fill_array(a, b, n);
//    print_array(a, b, n, chk);
    chk = prefix_sum(a, b, n, chk);
//    print_array(a, b, n, chk);
    free(a);
    free(b);
    printElapsedTime();
    return EXIT_SUCCESS;
}

int prefix_sum(int *a, int *b, size_t n, int chk) {
    int i;
    int step_length;
    starttime = omp_get_wtime();
    for (step_length = 1; step_length < n; step_length *= 2) {
//#pragma omp parallel
        {
//#pragma omp single
            {
                for (i = 0; i < n; ++i) {
                    if (i < step_length) {
                        if (chk % 2 == 0) {
                            b[i] = a[i];
                        } else {
                            a[i] = b[i];
                        }
                    } else {
                        if (chk % 2 == 0) {
//#pragma omp task
                            { b[i] = a[i] + a[i - step_length]; }
                        } else {
//#pragma omp task
                            { a[i] = b[i] + b[i - step_length]; }
                        }
                    }
                }
//#pragma omp taskwait
            }
        }
        chk += 1;
    }
    elapsedtime = omp_get_wtime() - starttime;
    return chk;
}

void print_array(int *a, int *b, size_t n, int chk) {
    int i;
    if (chk % 2 == 0){
        printf("[-] a array: ");
        for (i = 0; i < n; ++i) {
            printf("%d, ", a[i]);
        }
        printf("\b\b  \n");
    } else {
        printf("[-] b array: ");
        for (i = 0; i < n; ++i) {
            printf("%d, ", b[i]);
        }
        printf("\b\b  \n");
    }
    printf("\b\b  \n");
}

void fill_array(int *a, int *b, size_t n) {
    int i;
    for (i = 0; i < n; ++i) {
        a[i] = i + 1;
    }
    for (i = 0; i < n; ++i) {
        b[i] = -1;
    }
}

void printElapsedTime() {
    printf("Time Elapsed: %f Secs\n", elapsedtime);
}