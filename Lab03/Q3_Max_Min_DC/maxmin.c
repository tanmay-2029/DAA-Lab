#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static long comparisons = 0;

typedef struct { int mx, mn; } Pair;

Pair maxMin(int arr[], int lo, int hi) {
    int n = hi - lo + 1;
    Pair result;

    if (n == 1) {
        result.mx = result.mn = arr[lo];
        return result;
    }
    if (n == 2) {
        comparisons++;
        if (arr[lo] > arr[hi]) { result.mx = arr[lo]; result.mn = arr[hi]; }
        else                   { result.mx = arr[hi]; result.mn = arr[lo]; }
        return result;
    }

    int mid = lo + (hi - lo) / 2;
    Pair left  = maxMin(arr, lo, mid);
    Pair right = maxMin(arr, mid + 1, hi);

    comparisons++;
    result.mx = (left.mx > right.mx) ? left.mx : right.mx;
    comparisons++;
    result.mn = (left.mn < right.mn) ? left.mn : right.mn;
    return result;
}

static void fillRandom(int *arr, int n) {
    for (int i = 0; i < n; i++) arr[i] = rand() % 1000000;
}

int main(int argc, char *argv[]) {
    srand(42);

    if (argc >= 3 && argv[1][0] == '-' && argv[1][1] == '-' && argv[1][2] == 'b') {
        int maxN = atoi(argv[2]);
        printf("n,comparisons,bound_3n_over_2\n");
        for (int n = 2; n <= maxN; n *= 2) {
            int *arr = malloc(sizeof(int) * n);
            fillRandom(arr, n);
            comparisons = 0;
            maxMin(arr, 0, n - 1);
            printf("%d,%ld,%.1f\n", n, comparisons, 1.5 * n);
            free(arr);
        }
        return 0;
    }

    int n;
    printf("Enter number of elements: ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Invalid input\n");
        return 1;
    }
    int *arr = malloc(sizeof(int) * n);
    printf("Enter %d elements: ", n);
    for (int i = 0; i < n; i++) scanf("%d", &arr[i]);

    comparisons = 0;
    Pair p = maxMin(arr, 0, n - 1);

    printf("\nMaximum = %d\n", p.mx);
    printf("Minimum = %d\n", p.mn);
    printf("Comparisons used = %ld  (bound: 3n/2 = %.1f)\n", comparisons, 1.5 * n);

    free(arr);
    return 0;
}
