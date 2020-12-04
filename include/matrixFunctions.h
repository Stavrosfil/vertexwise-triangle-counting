#include <stdint.h>

bool hasEdge(uint32_t a, uint32_t b, uint32_t *csr_row_ptr, uint32_t *csr_col) {

    uint32_t lo  = csr_row_ptr[a];
    uint32_t hi  = csr_row_ptr[a + 1];
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