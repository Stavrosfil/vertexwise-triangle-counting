#include <stdint.h>
#include <stdio.h>
#include "pthread.h"

#ifndef matrixFunctions
#define matrixFunctions
#include "../include/matrixFunctions.h"
#endif

pthread_mutex_t lock;
uint32_t iteration = 0;

struct data {
    uint32_t N, *c, *csr_row_ptr, *csr_col;
};

void *multiplyMatrixVector(void *args) {

    struct data *d = (struct data *)args;
    uint32_t i     = 0;

    while (iteration < d->N) {

        pthread_mutex_lock(&lock);
        i = iteration++;
        pthread_mutex_unlock(&lock);

        for (int j = d->csr_row_ptr[i]; j < d->csr_row_ptr[i + 1]; j++)
            multiplyRowCol(i, d->csr_col[j], d->c, d->csr_row_ptr, d->csr_col);
        d->c[i] /= 2;
    }
    pthread_exit(NULL);
}

void triangleCountV4(uint32_t N, uint32_t *c, uint32_t *csr_row_ptr, uint32_t *csr_col, uint32_t num_threads) {

    pthread_t threads[num_threads];
    struct data thread_data;

    thread_data.N           = N;
    thread_data.c           = c;
    thread_data.csr_row_ptr = csr_row_ptr;
    thread_data.csr_col     = csr_col;

    for (int i = 0; i < num_threads; i++)
        pthread_create(&threads[i], NULL, multiplyMatrixVector, &thread_data);

    for (int i = 0; i < num_threads; i++)
        pthread_join(threads[i], NULL);
}