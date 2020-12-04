#include <stdint.h>
#include <stdio.h>
#include "pthread.h"

#define NUM_THREADS 8

pthread_mutex_t lock;
uint32_t iteration = 0;

struct data {
    uint32_t N, *c, *csr_row_ptr, *csr_col;
};

void multiplyRowCol(uint32_t row, uint32_t *c, uint32_t col, uint32_t *csr_row_ptr, uint32_t *csr_col) {

    uint32_t i = csr_row_ptr[row];
    uint32_t j = csr_row_ptr[col];

    uint32_t ans = 0;

    for (; i < csr_row_ptr[row + 1] && j < csr_row_ptr[col + 1];) {
        if (csr_col[i] == csr_col[j]) {
            c[row]++;
            i++;
            j++;
        } else if (csr_col[i] < csr_col[j])
            i++;
        else
            j++;
    }
}

void *multiplyMatrixVector(void *args) {

    struct data *d = (struct data *)args;
    uint32_t i     = 0;

    while (iteration < d->N) {

        pthread_mutex_lock(&lock);
        i = iteration++;
        pthread_mutex_unlock(&lock);

        for (int j = d->csr_row_ptr[i]; j < d->csr_row_ptr[i + 1]; j++)
            multiplyRowCol(i, d->c, d->csr_col[j], d->csr_row_ptr, d->csr_col);
        d->c[i] /= 2;
    }
    pthread_exit(NULL);
}

void triangleCountV4(uint32_t N, uint32_t *c, uint32_t *csr_row_ptr, uint32_t *csr_col) {

    pthread_t threads[NUM_THREADS];
    struct data thread_data;

    thread_data.N           = N;
    thread_data.c           = c;
    thread_data.csr_row_ptr = csr_row_ptr;
    thread_data.csr_col     = csr_col;

    for (int i = 0; i < NUM_THREADS; i++)
        pthread_create(&threads[i], NULL, multiplyMatrixVector, &thread_data);

    for (int i = 0; i < NUM_THREADS; i++)
        pthread_join(threads[i], NULL);
}