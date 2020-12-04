#include <stdint.h>
#include <stdio.h>

bool hasEdge(uint32_t a, uint32_t b, uint32_t csr_row_ptr[], uint32_t csr_col[]) {

    uint32_t lo  = csr_row_ptr[a];
    uint32_t hi  = csr_row_ptr[a + 1] - 1;
    uint32_t mid = (lo + hi) / 2;

    while (lo <= hi) {
        if (csr_col[mid] == b)
            return true;
        else if (csr_col[mid] < b)
            lo = mid + 1;
        else
            hi = mid - 1;
        mid = (lo + hi) / 2;
    }
    return false;
}

void triangleCountV3(int N, uint32_t *c3, uint32_t *csr_row_ptr, uint32_t *csr_col) {

    for (int i = 0; i < N - 2; i++) {

        int colStartPtr = csr_row_ptr[i];
        int colEndPrt   = csr_row_ptr[i + 1];

        for (int j = colStartPtr; j < colEndPrt; j++) {
            int a = csr_col[j];
            for (int k = j + 1; k < colEndPrt; k++) {
                int b = csr_col[k];
                if (hasEdge(a, b, csr_row_ptr, csr_col)) {
                    c3[i]++;
                }
            }
        }
    }
}