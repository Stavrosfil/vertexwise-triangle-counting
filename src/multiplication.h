#include <stdio.h>
#define sz 4

void multiplyMatrixVector(bool A[sz][sz], uint32_t V[sz], uint32_t ans[sz]) {
    for (int i = 0; i < sz; i++) {
        // ans[i] = 0;
        for (int j = 0; j < sz; j++) {
            ans[i] += A[i][j] * V[j];
        }
    }
}

void multiplyMatrixVectorCSR(uint32_t V[sz], uint32_t *ans, uint32_t *csr_row_ptr, uint32_t *csr_col) {

    // for (k = 0; k < N; k = k + 1)
    // result[i] = 0;

    for (int i = 0; i < sz; i++) {
        for (int j = csr_row_ptr[i]; j < csr_row_ptr[i + 1]; j++) {
            ans[i] += V[csr_col[j]];
        }
    }
}