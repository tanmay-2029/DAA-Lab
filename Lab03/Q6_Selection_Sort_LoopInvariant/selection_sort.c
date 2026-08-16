#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static long comparisons = 0;
static long swaps = 0;

/* Checks that A[0..i-1] is sorted -- used to validate the loop
 * invariant at runtime (only compiled in when INVARIANT_CHECK is
 * defined, to avoid slowing down benchmarking). */
#ifdef INVARIANT_CHECK
static void assertInvariant(int A[], int i) {
    for (int k = 1; k < i; k++) {
        if (A[k - 1] > A[k]) {
            fprintf(stderr, "Loop invariant VIOLATED before iteration i=%d!\n", i);
            exit(1);
        }
    }
}
#endif

void selectionSort(int A[], int n) {
    for (int i = 0; i < n - 1; i++) {
#ifdef INVARIANT_CHECK
        assertInvariant(A, i); /* A[0..i-1] sorted & the i smallest so far */
#endif
        int min = i;
        for (int j = i + 1; j < n; j++) {
            comparisons++;
            if (A[j] < A[min]) min = j;
        }
        if (min != i) {
            int tmp = A[i]; A[i] = A[min]; A[min] = tmp;
            swaps++;
        }
    }
#ifdef INVARIANT_CHECK
    assertInvariant(A, n); /* final check: whole array sorted */
#endif
}

static void printArray(int A[], int n) {
    for (int i = 0; i < n; i++) printf("%d ", A[i]);
    printf("\n");
}

static void fillRandom(int *arr, int n) { for (int i = 0; i < n; i++) arr[i] = rand() % 1000000; }
static void fillSorted(int *arr, int n) { for (int i = 0; i < n; i++) arr[i] = i; }
static void fillReverse(int *arr, int n) { for (int i = 0; i < n; i++) arr[i] = n - i; }

int main(int argc, char *argv[]) {
    srand((unsigned) time(NULL));

    if (argc >= 3 && argv[1][0] == '-' && argv[1][1] == '-' && argv[1][2] == 'b') {
        int maxN = atoi(argv[2]);
        printf("n,comparisons_random,comparisons_sorted,comparisons_reverse\n");
        for (int n = 100; n <= maxN; n *= 2) {
            int *a1 = malloc(sizeof(int) * n);
            int *a2 = malloc(sizeof(int) * n);
            int *a3 = malloc(sizeof(int) * n);
            fillRandom(a1, n);
            fillSorted(a2, n);
            fillReverse(a3, n);

            comparisons = 0; selectionSort(a1, n); long cRand = comparisons;
            comparisons = 0; selectionSort(a2, n); long cSorted = comparisons;
            comparisons = 0; selectionSort(a3, n); long cRev = comparisons;

            printf("%d,%ld,%ld,%ld\n", n, cRand, cSorted, cRev);

            free(a1); free(a2); free(a3);
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

    printf("\nBefore sorting: ");
    printArray(arr, n);

    comparisons = 0; swaps = 0;
    selectionSort(arr, n);

    printf("After sorting:  ");
    printArray(arr, n);
    printf("\nComparisons = %ld (expected n(n-1)/2 = %ld)\n", comparisons, (long) n * (n - 1) / 2);
    printf("Swaps       = %ld\n", swaps);

    free(arr);
    return 0;
}
