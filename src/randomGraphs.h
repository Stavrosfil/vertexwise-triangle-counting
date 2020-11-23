#include <stdlib.h>
#include <time.h>

#include "defs.h"

void prtarr(bool arr[RAND_N][RAND_N]) {
    for (int i = 0; i < RAND_N; i++) {
        for (int j = 0; j < RAND_N; j++)
            DEBUG_PRINT(("%d ", arr[i][j]));
        DEBUG_PRINT(("\n"));
    }
}
void initRandomGraph(bool A[RAND_N][RAND_N]) {
    srandom(time(NULL));
    for (int i = 0; i < RAND_N; i++) {
        for (int j = i + 1; j < RAND_N; j++) {
            // Check if an IF is faster (if random() & 1 => A[i][j] = 1 etc)
            bool r  = random() & 1;
            A[i][j] = r;
            A[j][i] = r;
        }
    }
}