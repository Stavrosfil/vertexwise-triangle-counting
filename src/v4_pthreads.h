#include <stdint.h>
#include <stdio.h>
#include "pthread.h"

#define NUM_THREADS 10000

struct data {
    uint32_t lo, hi, *c, *csr_row_ptr, *csr_col;
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
        } else if (csr_col[i] < csr_col[j]) {
            i++;
        } else {
            j++;
        }
    }
    // return ans;
}

void *multiplyMatrixVector(void *args) {

    struct data *d = (struct data *)args;

    // uint32_t ans = 0;

    for (int i = d->lo; i < d->hi; i++) {
        for (int j = d->csr_row_ptr[i]; j < d->csr_row_ptr[i + 1]; j++) {
            multiplyRowCol(i, d->c, d->csr_col[j], d->csr_row_ptr, d->csr_col);
        }
    }

    // pthread_exit(NULL);
    return NULL;
}

void triangleCountV4(uint32_t N, uint32_t *c, uint32_t *csr_row_ptr, uint32_t *csr_col) {

    pthread_t threads[NUM_THREADS];

    struct data thread_data[NUM_THREADS];

    uint32_t chunk_size = N / NUM_THREADS;

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_data[i].c           = c;
        thread_data[i].lo          = i * chunk_size;
        thread_data[i].hi          = (i + 1) * chunk_size;
        thread_data[i].csr_row_ptr = csr_row_ptr;
        thread_data[i].csr_col     = csr_col;
    }
    thread_data[NUM_THREADS - 1].hi = N;

    // Starting the threads
    for (int i = 0; i < NUM_THREADS; i++)
        pthread_create(&threads[i], NULL, multiplyMatrixVector, &thread_data[i]);

    // And waiting for the threads to finish
    for (int i = 0; i < NUM_THREADS; i++)
        pthread_join(threads[i], NULL);

    // for (int i = 0; i < N; i++) {
    // multiplyMatrixVector(i, c, csr_row_ptr, csr_col);
    // }
}