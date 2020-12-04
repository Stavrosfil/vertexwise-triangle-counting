#include <stdio.h>
#include <stdint.h>

void multiplyColRow(uint32_t row,
                    uint32_t col,
                    uint32_t *c3,
                    uint32_t *csr_row_ptr,
                    uint32_t *csr_col,
                    uint32_t *csc_col_ptr,
                    uint32_t *csc_row) {

    uint32_t i = csr_row_ptr[row];
    uint32_t j = csc_col_ptr[col];

    for (; i < csr_row_ptr[row + 1] && j < csc_col_ptr[col + 1];) {
        if (csr_col[i] == csc_row[j]) {
            c3[row]++;
            i++;
            j++;
        } else if (csr_col[i] < csc_row[j]) {
            i++;
        } else {
            j++;
        }
    }
}

void triangleCountV4(
    uint32_t N, uint32_t *c3, uint32_t *csr_row_ptr, uint32_t *csr_col, uint32_t *csc_col_ptr, uint32_t *csc_row) {
    for (int i = 0; i < N; i++) {
        for (int j = csr_row_ptr[i]; j < csr_row_ptr[i + 1]; j++)
            multiplyColRow(i, csr_col[j], c3, csr_row_ptr, csr_col, csc_col_ptr, csc_row);
        c3[i] /= 2;
    }
}