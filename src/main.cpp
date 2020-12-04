#include <cstdio>
#include <iostream>
#include <stdlib.h>
#include <vector>

// #define DEBUG
// #define filepath "data/as-Skitter.mtx"
// #define filepath "data/belgium_osm.mtx"
// #define filepath "data/ca-AstroPh.mtx"
// #define filepath "data/ca-GrQc.mtx"
// #define filepath "data/com-Youtube.mtx"
// #define filepath "data/dblp-2010.mtx"
// #define filepath "data/delaunay_n19.mtx"
// #define filepath "data/roadNet-CA.mtx"
// #define filepath "data/smalltest.mtx"
// #define filepath "data/testmatrix.mtx"

#include "../include/defs.h"

#include "../include/mmio.h"
#include "../include/coo2csc.h"
#include "../include/helpers.h"
#include "../include/timerHelpers.h"

// #include "../include/v1.h"
// #include "../include/v2.h"

#define openmp

#ifdef serial
#include "../include/v3.h"
#include "../include/v4.h"
#endif

#ifdef openmp
#include "../include/v3_openmp.h"
#include "../include/v4_openmp.h"
#endif

#ifdef cilk
#include "../include/v3_cilk.h"
#include "../include/v4_cilk.h"
#endif

#ifdef pthreads
#include "../include/v4_pthreads.h"
#endif

using namespace std;

int main(int argc, char *argv[]) {

    int M, N, nz, num_threads = 8;
    FILE *f;
    char *filepath;

    if (argc == 2) {
        filepath = argv[1];
    } else if (argc == 3) {
        filepath    = argv[1];
        num_threads = atoi(argv[2]);
    }

    if ((f = fopen(filepath, "r")) == NULL) {
        printf("File does not exist.\nExiting...");
        exit(1);
    }
    if (mm_read_mtx_crd_size(f, &M, &N, &nz) != 0) {
        printf("Failed to read matrix properties.\nExiting...");
        exit(1);
    }
    DEBUG_PRINT(("M: %d, N: %d, nz: %d\n", M, N, nz));

    nz *= 2;

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
        I[i + 1] = J[i];
        J[i + 1] = I[i];
        i++;
    }

    coo2csc(csr_col, csr_row_ptr, I, J, nz, N, isOneBased);
    coo2csc(csc_row, csc_col_ptr, J, I, nz, N, isOneBased);

    printMatrixH(I, nz, (char *)"I");
    printMatrixH(J, nz, (char *)"J");
    printMatrixH(csr_col, nz, (char *)"csr_col");
    printMatrixH(csr_row_ptr, N + 1, (char *)"csr_row_ptr");

    printMatrixH(csc_row, nz, (char *)"csc_row");
    printMatrixH(csc_col_ptr, N + 1, (char *)"csc_col_ptr");

    uint32_t triangles = 0;
    // printMatrixV(c3, N, (char *)"c3");

    timerStart();
    triangleCountV3(N, c3, csr_row_ptr, csr_col);
    timerEnd();
    timerPrint((char *)"v3");

    for (int i = 0; i < N; i++)
        triangles += c3[i];
    printf("Triangles: %u\n", triangles / 3);

    timerStart();
    triangleCountV4(N, c3, csr_row_ptr, csr_col, num_threads);
    timerEnd();
    timerPrint((char *)"v4");

    for (int i = 0; i < N; i++)
        triangles += c3[i];
    printf("Triangles: %u\n", triangles / 3);

    return 0;
}