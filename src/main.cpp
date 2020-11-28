#include <cstdio>
#include <iostream>
#include <stdlib.h>
#include <vector>

#define DEBUG
// #define FILEPATH "data/as-Skitter.mtx"
#define FILEPATH "data/ca-AstroPh.mtx"
// #define FILEPATH "data/ca-GrQc.mtx"
// #define FILEPATH "data/dblp-2010.mtx"
// #define FILEPATH "data/delaunay_n19.mtx"
// #define FILEPATH "data/roadNet-CA.mtx"
#define FILEPATH "data/smalltest.mtx"
// #define FILEPATH "data/testmatrix.mtx"

#include "../include/defs.h"

#include "../include/mmio.h"
#include "../include/coo2csc.h"
#include "../include/helpers.h"
#include "../include/timerHelpers.h"

#include "../include/v1.h"
#include "../include/v2.h"
#include "../include/v3_openmp.h"
// #include "../include/v3.h"
// #include "../include/v3_cilk.h"

#include "multiplication.h"

using namespace std;

int main() {

    int M, N, nz;

    FILE *f;

    if ((f = fopen(FILEPATH, "r")) == NULL) {
        printf("File does not exist.\nExiting...");
        exit(1);
    }
    if (mm_read_mtx_crd_size(f, &M, &N, &nz) != 0) {
        printf("Failed to read matrix properties.\nExiting...");
        exit(1);
    }
    DEBUG_PRINT(("M: %d, N: %d, nz: %d\n", M, N, nz));

    uint32_t *I           = (uint32_t *)malloc(nz * sizeof(uint32_t));
    uint32_t *J           = (uint32_t *)malloc(nz * sizeof(uint32_t));
    uint32_t *csr_col     = (uint32_t *)malloc(nz * sizeof(uint32_t));
    uint32_t *csr_row_ptr = (uint32_t *)malloc((N + 1) * sizeof(uint32_t));
    uint32_t *c3          = (uint32_t *)calloc(N, sizeof(uint32_t));
    uint32_t isOneBased   = 1;

    // uint32_t *csc_row     = (uint32_t *)malloc(nz * sizeof(uint32_t));
    // uint32_t *csc_col_ptr = (uint32_t *)malloc((N + 1) * sizeof(uint32_t));

    for (int i = 0; i < nz; i++)
        fscanf(f, "%d %d\n", &I[i], &J[i]);

    coo2csc(csr_col, csr_row_ptr, I, J, nz, N, isOneBased);
    // coo2csc(csc_row, csc_col_ptr, J, I, nz, N, isOneBased);

    printMatrixH(I, nz, (char *)"I");
    printMatrixH(J, nz, (char *)"J");
    printMatrixH(csr_col, nz, (char *)"csr_col");
    printMatrixH(csr_row_ptr, N + 1, (char *)"csr_row_ptr");
    // printMatrixH(csc_row, nz, (char *)"csc_row");
    // printMatrixH(csc_col_ptr, N + 1, (char *)"csc_col_ptr");

    uint32_t triangles = 0;

    bool A[4][4] = {0};
    initRandomGraph(A, 4, 4);
    uint32_t V[4]   = {0};
    uint32_t out[4] = {0};
    for (int i = 0; i < 4; i++) {
        V[i] = random() & 1;
    }

    printMatrix2Dims(A, 4, 4);

    multiplyMatrixVector(A, V, out);

    multiplyMatrixVectorCSR(V, out, csr_row_ptr, csr_col);

    printMatrixH(V, 4, "V");
    printMatrixH(out, 4, "out");
    // for (int i = 0; i < 4; i++) {
    // printf("%u ", out[i]);
    // }
    printf("\n");

    timerStart();
    triangles = triangleCountV3OpenMP(N, csr_row_ptr, csr_col, c3);
    timerEnd();
    timerPrint((char *)"v3-openmp");

    printf("Triangles: %u\n", triangles);
    // printMatrixV(c3, N, (char *)"c3");

    return 0;
}