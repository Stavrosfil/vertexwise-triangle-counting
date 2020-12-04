#include <stdint.h>
#include <stdio.h>
#include <cilk/cilk.h>
#include <cilk/reducer_opadd.h> //needs to be included to use the addition reducer
#include <cilk/cilk_api.h>

#ifndef matrixFunctions
#define matrixFunctions
#include "../include/matrixFunctions.h"
#endif

void triangleCountV4(uint32_t N, uint32_t *c, uint32_t *csr_row_ptr, uint32_t *csr_col, uint32_t num_threads) {

    cilk_for(int i = 0; i < N; i++) {
        for (int j = csr_row_ptr[i]; j < csr_row_ptr[i + 1]; j++)
            multiplyRowCol(i, csr_col[j], c, csr_row_ptr, csr_col);
        c[i] /= 2;
    }
}