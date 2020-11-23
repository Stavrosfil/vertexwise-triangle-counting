#include <cstdio>
#include <iostream>
#include <stdlib.h>
#include <vector>

// #define DEBUG
#define FILEPATH "../data/ca-GrQc.mtx"
// #define FILEPATH "../data/as-Skitter.mtx"
// #define FILEPATH "../data/bus.mtx"
// #define FILEPATH "../data/smalltest.mtx"
// #define FILEPATH "../data/testmatrix.mtx"

#include "defs.h"

#include "mmio.h"
#include "coo2csc.h"
#include "helpers.h"
#include "timerHelpers.h"

#include "v1.h"
#include "v2.h"
#include "v3.h"

using namespace std;

bool A[RAND_N][RAND_N];
FILE *f;

int main() {

    int M, N, nz;
    int triangles = 0;

    FILE *f;

    if ((f = fopen(FILEPATH, "r")) == NULL)
        exit(1);
    if (mm_read_mtx_crd_size(f, &M, &N, &nz) != 0)
        exit(1);
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

    printMatrixH(I, nz, "I");
    printMatrixH(J, nz, "J");
    printMatrixH(csc_row, nz, "csc_row");
    printMatrixH(csc_col, N + 1, "csc_col");

    /* ----------------------- Neighbourhoods of vertices ----------------------- */

    timerStart();
    for (int i = 1; i < N - 1; i++) {
        for (int j = i + 1; j < N; j++) {
            int si1, si2, sj1, sj2;
            si1 = csc_col[i];
            si2 = csc_col[i + 1];
            sj1 = csc_col[j];
            sj2 = csc_col[j + 1];

            // Find common subset
            for (int k = si1; k < si2; k++) {
                int c1 = 0;
                for (int l = sj1; l < sj2; l++) {
                    if (i == csc_row[l] || csc_row[k] == csc_row[l]) {
                        c1++;
                    }
                    if (c1 == 2) {
                        triangles++;
                        c3[i]++;
                        c3[j]++;
                        c3[csc_row[k]]++;
                        break;
                    }
                }
            }
        }
    }
    timerEnd();
    timerPrint("v3");
    // DEBUG_PRINT(("Triangles: %d\n", triangles));
    printf("Triangles: %d\n", triangles);

    /* --------------------------------- random --------------------------------- */

    printMatrixV(c3, N, "c3");

    initRandomGraph(A, RAND_N, RAND_N);

    timerStart();
    triangleCountV2(A, RAND_N, RAND_N, c3);
    timerEnd();
    timerPrint("v2");

    timerStart();
    triangleCountV1(A, RAND_N, RAND_N, c3);
    timerEnd();
    timerPrint("v1");

    printMatrix2Dims(A, RAND_N, RAND_N);

    return 0;
}