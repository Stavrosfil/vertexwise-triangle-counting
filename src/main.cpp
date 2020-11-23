#include <cstdio>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <vector>

#include "mmio.h"
#include "coo2csc.h"
#include "randomGraphs.h"

#include "v1.h"
#include "v2.h"
#include "v3.h"

#define DEBUG

#include "defs.h"

using namespace std;

/* -------------------------------- VARIABLES ------------------------------- */
bool A[RAND_N][RAND_N];
struct timespec ts_start;
struct timespec ts_end;
FILE *f;

/* ------------------------------------ - ----------------------------------- */

double totalTime(timespec start, timespec end) {
    double time_taken;
    time_taken = (end.tv_sec - start.tv_sec) * 1e9;
    time_taken = (time_taken + (end.tv_nsec - start.tv_nsec)) * 1e-9;
    return time_taken;
}

void timerStart() { clock_gettime(CLOCK_MONOTONIC, &ts_start); }
void timerEnd() { clock_gettime(CLOCK_MONOTONIC, &ts_end); }
void timerPrint(char *argv) { printf("Time to run %s: [%lf]s\n", argv, totalTime(ts_start, ts_end)); }

void printMatrixH(uint32_t *a, uint32_t size, char *text) {
    DEBUG_PRINT(("%s: ", text));
    for (int i = 0; i < size; i++)
        DEBUG_PRINT(("%d ", a[i]));
}

int main() {

    // char FILEPATH[] = {"../data/ca-GrQc.mtx"};
    char FILEPATH[] = {"../data/smalltest.mtx"};
    // char FILEPATH[] = {"../data/testmatrix.mtx"};
    int M, N, nz;

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

    printMatrixH(I, nz, "I");
    printMatrixH(J, nz, "J");

    coo2csc(csc_row, csc_col, J, I, nz, N, isOneBased);

    DEBUG_PRINT(("\ncsc_row: "));
    for (int i = 0; i < nz; i++)
        DEBUG_PRINT(("%d ", csc_row[i]));
    DEBUG_PRINT(("\ncsc_col: "));
    for (int i = 0; i < N + 1; i++)
        DEBUG_PRINT(("%d ", csc_col[i]));
    DEBUG_PRINT(("\n"));

    /* ----------------------- Neighbourhoods of vertices ----------------------- */

    int cnt = 0;

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
                        cnt++;
                        c3[i]++;
                        c3[j]++;
                        c3[csc_row[k]]++;
                        // printf("%d %d %d %d %d %d\n", i, j, csc_row[k], csc_row[l], k, l);
                        break;
                    }
                }
            }
        }
    }
    timerEnd();
    timerPrint("v3");
    DEBUG_PRINT(("Triangles: %d\n", cnt));

    /* --------------------------------- random --------------------------------- */

    // for (int i = 0; i < N; i++) {
    //     DEBUG_PRINT(("%d: %d\n", i, c3[i]));
    // }

    initRandomGraph(A);

    // timerStart();
    // triangleCountV2(A, RAND_N, RAND_N, c3);
    // timerEnd();
    // timerPrint("v2");

    // timerStart();
    // triangleCountV1(A, RAND_N, RAND_N, c3);
    // timerEnd();
    // timerPrint("v1");

    prtarr(A);

    // DEBUG_PRINT(("\n"));
    // for (int i = 0; i < n; i++) {
    //     DEBUG_PRINT(("%d ", c3[i]));
    // }

    return 0;
}