#include <stdint.h>

int hasEdge(uint32_t a, uint32_t b, uint32_t *csr_row_ptr, uint32_t *csr_col) {

    uint32_t lo  = csr_row_ptr[a];
    uint32_t hi  = csr_row_ptr[a + 1] - 1;
    uint32_t mid = (lo + hi) / 2;

    while (lo <= hi) {
        if (csr_col[mid] == b)
            return 1;
        else if (csr_col[mid] < b)
            lo = mid + 1;
        else
            hi = mid - 1;
        mid = (lo + hi) / 2;
    }
    return 0;
}

void multiplyRowCol(uint32_t row, uint32_t col, uint32_t *c, uint32_t *csr_row_ptr, uint32_t *csr_col) {

    uint32_t i = csr_row_ptr[row];
    uint32_t j = csr_row_ptr[col];

    uint32_t ans = 0;

    for (; i < csr_row_ptr[row + 1] && j < csr_row_ptr[col + 1];) {
        if (csr_col[i] == csr_col[j]) {
            c[row]++;
            i++;
            j++;
        } else if (csr_col[i] < csr_col[j])
            i++;
        else
            j++;
    }
}