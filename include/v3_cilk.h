#include <stdint.h>
#include <stdio.h>
#include <math.h>

#ifndef matrixFunctions
#define matrixFunctions
#include "../include/matrixFunctions.h"
#endif

#include <cilk/cilk.h>
#include <cilk/reducer_opadd.h>
#include <cilk/cilk_api.h>

void triangleCountV3(int N, uint32_t *c3, uint32_t *csr_row_ptr, uint32_t *csr_col) {

    cilk_for(int i = 0; i < N - 2; i++) {
        int colStartPtr = csr_row_ptr[i];
        int colEndPrt   = csr_row_ptr[i + 1];
        for (int j = colStartPtr; j < colEndPrt; j++) {
            int a = csr_col[j];
            for (int k = j + 1; k < colEndPrt; k++) {
                int b = csr_col[k];
                if (hasEdge(a, b, csr_row_ptr, csr_col)) {
                    c3[i]++;
                }
            }
        }
    }
}