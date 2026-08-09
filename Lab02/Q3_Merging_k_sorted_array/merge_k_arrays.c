#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Helper to merge two sorted arrays
void merge_two(int *a, int na, int *b, int nb, int *out) {
    int i = 0, j = 0, k = 0;
    while (i < na && j < nb) {
        if (a[i] <= b[j]) out[k++] = a[i++];
        else out[k++] = b[j++];
    }
    while (i < na) out[k++] = a[i++];
    while (j < nb) out[k++] = b[j++];
}

// Method 1: Sequential Merge
int* merge_k_sequential(int **arrays, int k, int n) {
    int cur_len = n;
    int *result = (int*)malloc(sizeof(int) * n);
    for (int i = 0; i < n; i++) result[i] = arrays[0][i];

    for (int idx = 1; idx < k; idx++) {
        int new_len = cur_len + n;
        int *merged = (int*)malloc(sizeof(int) * new_len);
        merge_two(result, cur_len, arrays[idx], n, merged);
        free(result);
        result = merged;
        cur_len = new_len;
    }
    return result;
}

// Method 2: Pairwise / Tournament Merge
int* merge_k_pairwise(int **arrays, int k, int n) {
    int **current = (int**)malloc(sizeof(int*) * k);
    int *lens = (int*)malloc(sizeof(int) * k);
    int count = k;

    for (int i = 0; i < k; i++) {
        current[i] = (int*)malloc(sizeof(int) * n);
        for (int j = 0; j < n; j++) current[i][j] = arrays[i][j];
        lens[i] = n;
    }

    while (count > 1) {
        int new_count = (count + 1) / 2;
        int **next = (int**)malloc(sizeof(int*) * new_count);
        int *next_lens = (int*)malloc(sizeof(int) * new_count);

        int p = 0;
        for (int i = 0; i < count; i += 2, p++) {
            if (i + 1 < count) {
                int m_len = lens[i] + lens[i + 1];
                int *merged = (int*)malloc(sizeof(int) * m_len);
                merge_two(current[i], lens[i], current[i + 1], lens[i + 1], merged);
                free(current[i]); free(current[i + 1]);
                next[p] = merged;
                next_lens[p] = m_len;
            } else {
                next[p] = current[i];
                next_lens[p] = lens[i];
            }
        }
        free(current); free(lens);
        current = next; lens = next_lens; count = new_count;
    }

    int *result = current[0];
    free(current); free(lens);
    return result;
}

// Qsort comparison function
int cmp(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

// Create k sorted arrays
int** make_k_arrays(int k, int n) {
    int **arrays = (int**)malloc(sizeof(int*) * k);
    for (int i = 0; i < k; i++) {
        arrays[i] = (int*)malloc(sizeof(int) * n);
        for (int j = 0; j < n; j++) arrays[i][j] = rand() % 100000;
        qsort(arrays[i], n, sizeof(int), cmp);
    }
    return arrays;
}

void free_k_arrays(int **arrays, int k) {
    for (int i = 0; i < k; i++) free(arrays[i]);
    free(arrays);
}

int main() {
    srand(42);
    clock_t start, end;

    // --- Experiment A: Fix n, vary k ---
    int n_fixed = 500;
    int ks[] = {2, 4, 8, 16, 32, 64, 128};
    int num_k = 7;

    FILE *fp1 = fopen("results_q3_vary_k.csv", "w");
    fprintf(fp1, "n,k,sequential_ms,pairwise_ms\n");

    for (int idx = 0; idx < num_k; idx++) {
        int k = ks[idx];
        int **arrays = make_k_arrays(k, n_fixed);

        start = clock();
        int *r1 = merge_k_sequential(arrays, k, n_fixed);
        end = clock();
        double t_seq = (double)(end - start) / CLOCKS_PER_SEC * 1000;
        free(r1);

        start = clock();
        int *r2 = merge_k_pairwise(arrays, k, n_fixed);
        end = clock();
        double t_pair = (double)(end - start) / CLOCKS_PER_SEC * 1000;
        free(r2);

        fprintf(fp1, "%d,%d,%f,%f\n", n_fixed, k, t_seq, t_pair);
        printf("n=%d k=%d done\n", n_fixed, k);
        free_k_arrays(arrays, k);
    }
    fclose(fp1);

    // --- Experiment B: Fix k, vary n ---
    int k_fixed = 16;
    int ns[] = {200, 400, 800, 1600, 3200, 6400};
    int num_n = 6;

    FILE *fp2 = fopen("results_q3_vary_n.csv", "w");
    fprintf(fp2, "k,n,sequential_ms,pairwise_ms\n");

    for (int idx = 0; idx < num_n; idx++) {
        int n = ns[idx];
        int **arrays = make_k_arrays(k_fixed, n);

        start = clock();
        int *r1 = merge_k_sequential(arrays, k_fixed, n);
        end = clock();
        double t_seq = (double)(end - start) / CLOCKS_PER_SEC * 1000;
        free(r1);

        start = clock();
        int *r2 = merge_k_pairwise(arrays, k_fixed, n);
        end = clock();
        double t_pair = (double)(end - start) / CLOCKS_PER_SEC * 1000;
        free(r2);

        fprintf(fp2, "%d,%d,%f,%f\n", k_fixed, n, t_seq, t_pair);
        printf("k=%d n=%d done\n", k_fixed, n);
        free_k_arrays(arrays, k_fixed);
    }
    fclose(fp2);

    printf("Results written to CSV files\n");
    return 0;
}
