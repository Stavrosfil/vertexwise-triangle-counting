void triangleCountV1(void *A, int n, int m, uint32_t *c3) {
    bool *a = (bool *)A;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            for (int k = 0; k < n; k++)
                if (a[i * m + j] == 1 && a[i * m + j] == a[j * m + k] && a[i * m + j] == a[i * m + k]) {
                    c3[i]++;
                    c3[j]++;
                    c3[k]++;
                }
}