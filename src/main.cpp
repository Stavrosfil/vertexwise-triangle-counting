#include <cstdio>
#include <iostream>
#include <stdlib.h>
#include <vector>

#define DEBUG
// #define FILEPATH "data/as-Skitter.mtx"
#define FILEPATH "data/belgium_osm.mtx"
// #define FILEPATH "data/ca-AstroPh.mtx"
// #define FILEPATH "data/ca-GrQc.mtx"
#define FILEPATH "data/com-Youtube.mtx"
// #define FILEPATH "data/dblp-2010.mtx"
// #define FILEPATH "data/delaunay_n19.mtx"
// #define FILEPATH "data/roadNet-CA.mtx"
// #define FILEPATH "data/smalltest.mtx"
#define FILEPATH "data/testmatrix.mtx"

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

int compare(const void *a, const void *b) {
    int int_a = *((int *)a);
    int int_b = *((int *)b);

    if (int_a == int_b)
        return 0;
    else if (int_a < int_b)
        return -1;
    else
        return 1;
}

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

    // nz *= 2;

    uint32_t *I           = (uint32_t *)malloc(nz * sizeof(uint32_t));
    uint32_t *J           = (uint32_t *)malloc(nz * sizeof(uint32_t));
    uint32_t *csr_col     = (uint32_t *)malloc(nz * sizeof(uint32_t));
    uint32_t *csr_row_ptr = (uint32_t *)malloc((N + 1) * sizeof(uint32_t));
    uint32_t *c3          = (uint32_t *)calloc(N, sizeof(uint32_t));
    uint32_t isOneBased   = 1;

    uint32_t *csc_row     = (uint32_t *)malloc(nz * sizeof(uint32_t));
    uint32_t *csc_col_ptr = (uint32_t *)malloc((N + 1) * sizeof(uint32_t));

    for (int i = 0; i < nz; i++) {
        fscanf(f, "%d %d\n", &I[i], &J[i]);
        // I[i + 1] = J[i];
        // J[i + 1] = I[i];
        // i++;
    }

    coo2csc(csr_col, csr_row_ptr, I, J, nz, N, isOneBased);
    coo2csc(csc_row, csc_col_ptr, J, I, nz, N, isOneBased);

    // for (int i = 0; i < N; i++) {
    // qsort((csr_col + csr_row_ptr[i]), csr_row_ptr[i + 1] - csr_row_ptr[i], sizeof(uint32_t), compare);
    // qsort((csc_row + csc_col_ptr[i]), csc_col_ptr[i + 1] - csc_col_ptr[i], sizeof(uint32_t), compare);
    // }
    // int ccc = 0;
    // for (int i = 0; i < N; i++) {
    //     printf("%d -> ", i);
    //     for (int j = csr_row_ptr[i]; j < csr_row_ptr[i + 1]; j++) {
    //         // if (csr_col[j] < i) {
    //         printf("%d ", csr_col[j]);
    //         ccc++;
    //         // }
    //     }
    //     printf("\n");
    // }
    // printf("%d\n", ccc);

    printMatrixH(I, nz, (char *)"I");
    printMatrixH(J, nz, (char *)"J");
    printMatrixH(csr_col, nz, (char *)"csr_col");
    printMatrixH(csr_row_ptr, N + 1, (char *)"csr_row_ptr");

    printMatrixH(csc_row, nz, (char *)"csc_row");
    printMatrixH(csc_col_ptr, N + 1, (char *)"csc_col_ptr");

    uint32_t triangles = 0;

    timerStart();
    // triangles = triangleCountV3OpenMP(N, csr_row_ptr, csr_col, c3);
    triangleCountV4(N, c3, csr_row_ptr, csr_col, csc_col_ptr, csc_row);
    timerEnd();
    timerPrint((char *)"v3-openmp");

    for (int i = 0; i < N; i++) {
        triangles += c3[i];
    }

    printf("Triangles: %u\n", triangles);
    printMatrixV(c3, N, (char *)"c3");

    return 0;
}