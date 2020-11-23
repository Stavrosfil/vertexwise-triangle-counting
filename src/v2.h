void triangleCountV2(void *A, int n, int m, uint32_t *c3) {
    bool *a = (bool *)A;
    for (int i = 0; i < n - 2; i++)
        for (int j = i + 1; j < n - 1; j++)
            for (int k = j + 1; k < n; k++)
                if (a[i * m + j] == 1 && a[i * m + j] == a[j * m + k] && a[i * m + j] == a[i * m + k]) {
                    c3[i]++;
                    c3[j]++;
                    c3[k]++;
                }
}