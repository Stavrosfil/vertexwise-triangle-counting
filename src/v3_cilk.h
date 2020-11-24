#include <cilk/cilk.h>
#include <stdint.h>
#include <stdio.h>

uint32_t
sameInRange(int i, uint32_t si1, uint32_t si2, uint32_t sj1, uint32_t sj2, uint32_t *csc_col, uint32_t *csc_row) {

    uint32_t cnt = 0;

    // Find common subset
    for (int k = si1; k < si2; k++) {
        int c1 = 0;
        for (int l = sj1; l < sj2; l++) {
            if (i == csc_row[l] || csc_row[k] == csc_row[l]) {
                c1++;
            }
            if (c1 == 2) {
                cnt++;
                // c3[i]++;
                // c3[j]++;
                // c3[csc_row[k]]++;
                break;
            }
        }
    }

    return cnt;
}

uint32_t triangleCountV3Cilk(int N, uint32_t *csc_col, uint32_t *csc_row, uint32_t *c3) {
    // uint32_t triangles = 0;
    cilk::reducer_opadd<uint32_t> triangles;
    cilk_for(int i = 1; i < N - 1; i++) {
        for (int j = i + 1; j < N; j++) {
            int si1, si2, sj1, sj2;
            si1 = csc_col[i];
            si2 = csc_col[i + 1];
            sj1 = csc_col[j];
            sj2 = csc_col[j + 1];

            triangles += sameInRange(i, si1, si2, sj1, sj2, csc_col, csc_row);
        }
    }
    return triangles.get_value();
}