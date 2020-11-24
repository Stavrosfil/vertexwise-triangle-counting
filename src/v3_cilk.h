#include <stdint.h>
#include <stdio.h>
#include <cilk/cilk.h>
#include <cilk/reducer_opadd.h> //needs to be included to use the addition reducer
#include <cilk/cilk_api.h>

uint32_t hasEdge(uint32_t a, uint32_t b, uint32_t *csr_row, uint32_t *csr_col) {
    for (int i = csr_row[a]; i < csr_row[a + 1]; i++)
        if (b == csr_col[i])
            return 1;
    return 0;
}

uint32_t triangleCountV3Cilk(int N, uint32_t *csr_row, uint32_t *csr_col, uint32_t *c3) {

    cilk::reducer_opadd<uint32_t> triangles;

    int gs         = 10000;
    int numWorkers = __cilkrts_get_nworkers();
    printf("Workers: %d\n", numWorkers);

    // #pragma grainsize = gs
    cilk_for(int i = 0; i < N - 2; i++) {

        int colStartPtr = csr_row[i];
        int colEndPrt   = csr_row[i + 1];

        for (int j = colStartPtr; j < colEndPrt; j++) {
            int a = csr_col[j];
            if (a > i)
                for (int k = j + 1; k < colEndPrt; k++) {
                    int b = csr_col[k];
                    if (hasEdge(a, b, csr_row, csr_col)) {
                        triangles++;
                        c3[i]++;
                        c3[a]++;
                        c3[b]++;
                    }
                }
        }
    }
    return triangles.get_value();
}