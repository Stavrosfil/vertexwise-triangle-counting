#include <stdio.h>
#include <omp.h>
#include "../include/matrixFunctions.h"

void triangleCountV4(uint32_t N, uint32_t *c, uint32_t *csr_row_ptr, uint32_t *csr_col) {

#pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < N; i++) {
        for (int j = csr_row_ptr[i]; j < csr_row_ptr[i + 1]; j++) {
            multiplyRowCol(i, csr_col[j], c, csr_row_ptr, csr_col);
        }
        c[i] /= 2;
    }
}