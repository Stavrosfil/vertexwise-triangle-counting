#include <cstdio>
#include <iostream>
#include <stdlib.h>
#include <vector>

// #define DEBUG
#define FILEPATH "data/ca-GrQc.mtx"
// #define FILEPATH "data/as-Skitter.mtx"
// #define FILEPATH "data/smalltest.mtx"
// #define FILEPATH "data/testmatrix.mtx"

#include "../include/defs.h"

#include "../include/mmio.h"
#include "../include/coo2csc.h"
#include "../include/helpers.h"
#include "../include/timerHelpers.h"

#include "../include/v1.h"
#include "../include/v2.h"
#include "../include/v3.h"

using namespace std;

bool A[RAND_N][RAND_N];

int main() {

    int M, N, nz;
    int triangles = 0;

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

    uint32_t *I         = (uint32_t *)malloc(nz * sizeof(uint32_t));
    uint32_t *J         = (uint32_t *)malloc(nz * sizeof(uint32_t));
    uint32_t *csc_row   = (uint32_t *)malloc(nz * sizeof(uint32_t));
    uint32_t *csc_col   = (uint32_t *)malloc((N + 1) * sizeof(uint32_t));
    uint32_t *c3        = (uint32_t *)calloc(N, sizeof(uint32_t));
    uint32_t isOneBased = 0;

    for (int i = 0; i < nz; i++) {
        fscanf(f, "%d %d\n", &I[i], &J[i]);
        I[i]--;
        J[i]--;
    }

    coo2csc(csc_row, csc_col, J, I, nz, N, isOneBased);

    printMatrixH(I, nz, (char *)"I");
    printMatrixH(J, nz, (char *)"J");
    printMatrixH(csc_row, nz, (char *)"csc_row");
    printMatrixH(csc_col, N + 1, (char *)"csc_col");

    /* ----------------------- Neighbourhoods of vertices ----------------------- */

    timerStart();
    triangles = triangleCountV3(N, csc_col, csc_row, c3);
    timerEnd();
    timerPrint((char *)"v3");
    printf("Triangles: %d\n", triangles);
    printMatrixV(c3, N, (char *)"c3");

    /* --------------------------------- random --------------------------------- */

    // initRandomGraph(A, RAND_N, RAND_N);

    // timerStart();
    // triangleCountV2(A, RAND_N, RAND_N, c3);
    // timerEnd();
    // timerPrint("v2");

    // timerStart();
    // triangleCountV1(A, RAND_N, RAND_N, c3);
    // timerEnd();
    // timerPrint("v1");

    // printMatrix2Dims(A, RAND_N, RAND_N);

    return 0;
}