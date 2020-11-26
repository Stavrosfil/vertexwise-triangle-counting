#include <stdint.h>
#include <stdio.h>
#include <omp.h>
#include <math.h>

int hasEdge(uint32_t a, uint32_t b, uint32_t *csr_row, uint32_t *csr_col) {
    for (int i = csr_row[a]; i < csr_row[a + 1]; i++)
        if (b == csr_col[i])
            return 1;
    return 0;
}

uint32_t partitioner(int N, int chunks, int currentChunk) {
    int y = sqrt(currentChunk * currentChunk + N * N / 8);
    return y;
}

uint32_t triangleCountV3OpenMP(int N, uint32_t *csr_row, uint32_t *csr_col, uint32_t *c3) {

    uint32_t triangles = 0;
    int chunks         = 8;

#pragma omp parallel for schedule(dynamic) reduction(+ : triangles)
    for (int i = 0; i < N - 2; i++) {

        int tempTriangles = 0;

        int colStartPtr = csr_row[i];
        int colEndPrt   = csr_row[i + 1];

        for (int j = colStartPtr; j < colEndPrt; j++) {
            int a = csr_col[j];
            if (a > i)
                for (int k = j + 1; k < colEndPrt; k++) {
                    int b = csr_col[k];
                    if (hasEdge(a, b, csr_row, csr_col)) {
                        tempTriangles++;
                        // c3[i]++;
                        // c3[a]++;
                        // c3[b]++;
                    }
                }
        }
        triangles += tempTriangles;
    }
    return triangles;
}