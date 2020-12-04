#include <stdio.h>
#include <stdint.h>

uint32_t multiplyColRow(uint32_t row,
                        uint32_t col,
                        uint32_t *c3,
                        uint32_t *csr_row_ptr,
                        uint32_t *csr_col,
                        uint32_t *csc_col_ptr,
                        uint32_t *csc_row) {

    uint32_t i = csr_row_ptr[row];
    uint32_t j = csc_col_ptr[col];

    uint32_t ans = 0;

    for (; i < csr_row_ptr[row + 1] && j < csc_col_ptr[col + 1];) {
        if (csr_col[i] == csc_row[j]) {
            // printf("Equality: %d %d %d %d\n", i, j, csr_col[i], csr_col[j]);
            // c3[row]++;
            // c3[col]++;
            // c3[csr_col[i]]++;
            ans++;
            i++;
            j++;
        } else if (csr_col[i] < csc_row[j]) {
            i++;
        } else {
            j++;
        }
    }

    return ans;
}

void triangleCountV4(
    uint32_t N, uint32_t *c3, uint32_t *csr_row_ptr, uint32_t *csr_col, uint32_t *csc_col_ptr, uint32_t *csc_row) {
    for (int i = 0; i < N; i++) {
        uint32_t ans = 0;
        for (int j = csr_row_ptr[i]; j < csr_row_ptr[i + 1]; j++) {
            // if (csr_col[j] >= i)
            ans += multiplyColRow(i, csr_col[j], c3, csr_row_ptr, csr_col, csc_col_ptr, csc_row);
        }
        c3[i] = ans;
    }
}