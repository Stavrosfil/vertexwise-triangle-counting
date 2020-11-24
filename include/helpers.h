#include <stdlib.h>
#include <time.h>

#include "defs.h"

void printMatrixH(uint32_t *a, uint32_t size, char *text) {
    DEBUG_PRINT(("%s: ", text));
    for (int i = 0; i < size; i++)
        DEBUG_PRINT(("%d ", a[i]));
    DEBUG_PRINT(("\n"));
}

void printMatrixV(uint32_t *a, uint32_t size, char *text) {
    DEBUG_PRINT(("------- %s: \n", text));
    for (int i = 0; i < size; i++)
        DEBUG_PRINT(("%d: %d\n", i, a[i]));
    DEBUG_PRINT(("\n"));
}

void printMatrix2Dims(void *arr, int n, int m) {
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