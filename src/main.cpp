#include <cstdio>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <set>
#include <vector>

// #include "convert.h"
// #include "readMatrix.h"

#include "v1.h"
#include "v2.h"
#include "v3.h"

#include "mmio.h"
#include "coo2csc.h"

#define DEBUG
// #define n 1000
// #define n 10

#include "defs.h"

using namespace std;

/* -------------------------------- VARIABLES ------------------------------- */

// bool A[n][n];
struct timespec ts_start;
struct timespec ts_end;
FILE *f;

/* ------------------------------------ - ----------------------------------- */

// void prtarr(bool arr[n][n]) {
//     for (int i = 0; i < n; i++) {
//         for (int j = 0; j < n; j++)
//             DEBUG_PRINT(("%d ", arr[i][j]));
//         DEBUG_PRINT(("\n"));
//     }
// }
// void initRandomGraph(bool A[n][n]) {
//     srandom(time(NULL));
//     for (int i = 0; i < n; i++) {
//         for (int j = i + 1; j < n; j++) {
//             // Check if an IF is faster (if random() & 1 => A[i][j] = 1 etc)
//             bool r  = random() & 1;
//             A[i][j] = r;
//             A[j][i] = r;
//         }
//     }
// }
double totalTime(timespec start, timespec end) {
    double time_taken;
    time_taken = (end.tv_sec - start.tv_sec) * 1e9;
    time_taken = (time_taken + (end.tv_nsec - start.tv_nsec)) * 1e-9;
    return time_taken;
}

// https://medium.com/swlh/an-in-depth-introduction-to-sparse-matrix-a5972d7e8c86
int main() {

    char FILEPATH[] = {"../data/ca-GrQc.mtx"};
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

    DEBUG_PRINT(("I: "));
    for (int i = 0; i < nz; i++)
        DEBUG_PRINT(("%d ", I[i]));
    DEBUG_PRINT(("\nJ: "));
    for (int i = 0; i < nz; i++)
        DEBUG_PRINT(("%d ", J[i]));

    coo2csc(csc_row, csc_col, J, I, nz, N, isOneBased);

    DEBUG_PRINT(("\ncsc_row: "));
    for (int i = 0; i < nz; i++)
        DEBUG_PRINT(("%d ", csc_row[i]));
    DEBUG_PRINT(("\ncsc_col: "));
    for (int i = 0; i < N + 1; i++)
        DEBUG_PRINT(("%d ", csc_col[i]));
    DEBUG_PRINT(("\n"));

    /* ----------------------- Neighbourhoods of vertices ----------------------- */
    set<int>::iterator itr;
    vector<set<int>> nbh(N);

    clock_gettime(CLOCK_MONOTONIC, &ts_start);
    for (int i = 0; i < N; i++) {
        int s1 = csc_col[i];
        int s2 = csc_col[i + 1];
        // cout << "Slice: [" << s1 << ":" << s2 << "]" << endl;
        nbh[i] = set<int>(csc_row + s1, csc_row + s2);
    }
    clock_gettime(CLOCK_MONOTONIC, &ts_end);
    printf("\nTime to run: %lfs", totalTime(ts_start, ts_end));

    int cnt = 0;

    clock_gettime(CLOCK_MONOTONIC, &ts_start);
    for (int i = 1; i < N - 1; i++) {
        set<int> i_nodes = nbh[i];
        itr              = i_nodes.begin();

        for (itr = i_nodes.begin(); itr != i_nodes.end(); ++itr) {
            for (int j = i + 1; j < N; j++) {
                const bool is_in  = nbh[j].find(*itr) != nbh[j].end();
                const bool is_in2 = nbh[j].find(i) != nbh[j].end();
                if (is_in && is_in2)
                    cnt++;
                // printf("triangle\n");
                // c3[i]++;
                // c3[j]++;
                // c3[*itr]++;
            }
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &ts_end);

    printf("\n%d", cnt);

    // Create list of sets
    // for (int i = N - 1; i >= 0; i--) {
    //     int s1 = csc_col[i];
    //     int s2 = csc_col[i + 1];
    //     cout << "Slice: [" << s1 << ":" << s2 << "]" << endl;
    //     // nbh[i] = set<int>(csc_row + s1, csc_row + s2);
    //     for (int j = s1; j < s2; j++) {
    //         int node = csc_row[j];
    //         cout << "Node: " << node << endl;
    //         for (int k = csc_col[node]; k < csc_col[node + 1]; k++) {
    //             int node2 = csc_row[k];
    //             cout << "Node2: " << node2 << endl;
    //             if (node == node2) {
    //                 printf("Triangle");
    //                 // c3[i]++;
    //                 // c3[j]++;
    //                 // c3[k]++;
    //             }
    //         }
    //     }
    // }

    // for (int i = 0; i < N; i++) {
    // DEBUG_PRINT(("%d: %d\n", i, c3[i]));
    // }

    /* -------------------------------- print set ------------------------------- */

    // for (int i = 0; i < N; i++) {
    //     set<int> s = nbh[i];
    //     if (s.empty())
    //         cout << "Empty set";
    //     else
    //         for (itr = s.begin(); itr != s.end(); ++itr)
    //             cout << *itr << ',';
    //     cout << endl;
    // }

    /* --------------------------------- random --------------------------------- */

    // initRandomGraph();

    // clock_gettime(CLOCK_MONOTONIC, &ts_start);
    // triangleCountV2(A, n, n, c3);
    // clock_gettime(CLOCK_MONOTONIC, &ts_end);

    // prtarr(A);
    // DEBUG_PRINT(("\n"));
    // for (int i = 0; i < n; i++) {
    //     DEBUG_PRINT(("%d ", c3[i]));
    // }

    printf("\nTime to run: %lfs", totalTime(ts_start, ts_end));

    return 0;
}