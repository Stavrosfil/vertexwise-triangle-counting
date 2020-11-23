#include <stdlib.h>
#include <time.h>

#include "defs.h"

void prtarr(void *arr, int n, int m) {
    bool *a = (bool *)arr;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++)
            DEBUG_PRINT(("%d ", a[i * m + j]));
        DEBUG_PRINT(("\n"));
    }
}

void initRandomGraph(void *arr, int n, int m) {
    bool *a = (bool *)arr;
    srandom(time(NULL));
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < m; j++) {
            // Check if an IF is faster (if random() & 1 => A[i][j] = 1 etc)
            bool r       = random() & 1;
            a[i * m + j] = r;
            a[j * m + i] = r;
        }
    }
}