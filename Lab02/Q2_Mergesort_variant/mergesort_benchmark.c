#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// 2-Way Merge Sort
void merge2(int arr[], int lo, int mid, int hi, int tmp[]) {
    int i = lo, j = mid + 1, k = lo;
    while (i <= mid && j <= hi) {
        if (arr[i] <= arr[j]) tmp[k++] = arr[i++];
        else tmp[k++] = arr[j++];
    }
    while (i <= mid) tmp[k++] = arr[i++];
    while (j <= hi) tmp[k++] = arr[j++];
    for (int t = lo; t <= hi; t++) arr[t] = tmp[t];
}

void mergesort2(int arr[], int lo, int hi, int tmp[]) {
    if (lo >= hi) return;
    int mid = lo + (hi - lo) / 2;
    mergesort2(arr, lo, mid, tmp);
    mergesort2(arr, mid + 1, hi, tmp);
    merge2(arr, lo, mid, hi, tmp);
}

// 3-Way Merge Sort
void merge3(int arr[], int lo, int m1, int m2, int hi, int tmp[]) {
    int i = lo, j = m1 + 1, k = m2 + 1, t = lo;
    while (i <= m1 && j <= m2 && k <= hi) {
        if (arr[i] <= arr[j] && arr[i] <= arr[k]) tmp[t++] = arr[i++];
        else if (arr[j] <= arr[i] && arr[j] <= arr[k]) tmp[t++] = arr[j++];
        else tmp[t++] = arr[k++];
    }
    while (i <= m1 && j <= m2) tmp[t++] = (arr[i] <= arr[j]) ? arr[i++] : arr[j++];
    while (j <= m2 && k <= hi) tmp[t++] = (arr[j] <= arr[k]) ? arr[j++] : arr[k++];
    while (i <= m1 && k <= hi) tmp[t++] = (arr[i] <= arr[k]) ? arr[i++] : arr[k++];
    while (i <= m1) tmp[t++] = arr[i++];
    while (j <= m2) tmp[t++] = arr[j++];
    while (k <= hi) tmp[t++] = arr[k++];
    for (int u = lo; u <= hi; u++) arr[u] = tmp[u];
}

void mergesort3(int arr[], int lo, int hi, int tmp[]) {
    if (lo >= hi) return;
    int len = hi - lo + 1;
    if (len == 2) {
        if (arr[lo] > arr[hi]) { int temp = arr[lo]; arr[lo] = arr[hi]; arr[hi] = temp; }
        return;
    }
    int third = len / 3;
    int m1 = lo + third - 1;
    int m2 = lo + 2 * third - 1;
    mergesort3(arr, lo, m1, tmp);
    mergesort3(arr, m1 + 1, m2, tmp);
    mergesort3(arr, m2 + 1, hi, tmp);
    merge3(arr, lo, m1, m2, hi, tmp);
}

int main() {
    int sizes[] = {1000, 5000, 10000, 50000, 100000, 200000};
    int num_sizes = 6;
    
    FILE *fp = fopen("results_q2.csv", "w");
    fprintf(fp, "n,mergesort2_ms,mergesort3_ms\n");

    srand(42);

    for (int s = 0; s < num_sizes; s++) {
        int n = sizes[s];
        double total2 = 0, total3 = 0;

        for (int t = 0; t < 3; t++) {
            int *a1 = (int*)malloc(sizeof(int) * n);
            int *a2 = (int*)malloc(sizeof(int) * n);
            int *tmp = (int*)malloc(sizeof(int) * n);

            for (int i = 0; i < n; i++) {
                a1[i] = rand() % 100000;
                a2[i] = a1[i];
            }

            clock_t start = clock();
            mergesort2(a1, 0, n - 1, tmp);
            clock_t end = clock();
            total2 += (double)(end - start) / CLOCKS_PER_SEC * 1000;

            start = clock();
            mergesort3(a2, 0, n - 1, tmp);
            end = clock();
            total3 += (double)(end - start) / CLOCKS_PER_SEC * 1000;

            free(a1); free(a2); free(tmp);
        }

        fprintf(fp, "%d,%f,%f\n", n, total2 / 3, total3 / 3);
        printf("n = %d done\n", n);
    }

    fclose(fp);
    printf("Results written to results_q2.csv\n");
    return 0;
}