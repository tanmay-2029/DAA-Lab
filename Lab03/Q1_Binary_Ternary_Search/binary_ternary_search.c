#include <stdio.h>
#include <stdlib.h>

/* ---------- Binary Search ---------- */
int binarySearch(int arr[], int low, int high, int x, long *comparisons) {
    while (low <= high) {
        int mid = low + (high - low) / 2;
        (*comparisons)++;
        if (arr[mid] == x) return mid;
        (*comparisons)++;
        if (arr[mid] < x)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return -1;
}

/* ---------- Ternary Search ---------- */
int ternarySearch(int arr[], int low, int high, int x, long *comparisons) {
    while (low <= high) {
        int mid1 = low + (high - low) / 3;
        int mid2 = high - (high - low) / 3;

        (*comparisons)++;
        if (arr[mid1] == x) return mid1;
        (*comparisons)++;
        if (arr[mid2] == x) return mid2;

        (*comparisons)++;
        if (x < arr[mid1]) {
            high = mid1 - 1;
        } else {
            (*comparisons)++;
            if (x > arr[mid2]) {
                low = mid2 + 1;
            } else {
                low = mid1 + 1;
                high = mid2 - 1;
            }
        }
    }
    return -1;
}

static void fillSorted(int *arr, int n) {
    for (int i = 0; i < n; i++) arr[i] = i * 2; /* 0,2,4,... */
}

/* Runs both searches for every element in the array and returns the
 * average number of comparisons per successful search. */
static void benchmark(int n, double *avgBinary, double *avgTernary) {
    int *arr = malloc(sizeof(int) * n);
    fillSorted(arr, n);

    long totalBinary = 0, totalTernary = 0;
    for (int i = 0; i < n; i++) {
        long c = 0;
        binarySearch(arr, 0, n - 1, arr[i], &c);
        totalBinary += c;

        c = 0;
        ternarySearch(arr, 0, n - 1, arr[i], &c);
        totalTernary += c;
    }
    *avgBinary = (double) totalBinary / n;
    *avgTernary = (double) totalTernary / n;
    free(arr);
}

int main(int argc, char *argv[]) {
    if (argc >= 3 && (
        (argv[1][0]=='-' && argv[1][1]=='-' && argv[1][2]=='b') )) {
        int maxN = atoi(argv[2]);
        printf("n,binary_comparisons,ternary_comparisons\n");
        for (int n = 10; n <= maxN; n *= 2) {
            double avgB, avgT;
            benchmark(n, &avgB, &avgT);
            printf("%d,%.4f,%.4f\n", n, avgB, avgT);
        }
        return 0;
    }

    /* Interactive / demo mode */
    int n;
    printf("Enter number of elements: ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Invalid size\n");
        return 1;
    }
    int *arr = malloc(sizeof(int) * n);
    fillSorted(arr, n);

    printf("Sorted array generated (0, 2, 4, ... %d)\n", arr[n - 1]);
    int x;
    printf("Enter element to search: ");
    if (scanf("%d", &x) != 1) return 1;

    long compB = 0, compT = 0;
    int idxB = binarySearch(arr, 0, n - 1, x, &compB);
    int idxT = ternarySearch(arr, 0, n - 1, x, &compT);

    printf("\n--- Binary Search ---\n");
    if (idxB != -1) printf("Found %d at index %d\n", x, idxB);
    else printf("%d not found\n", x);
    printf("Comparisons used: %ld\n", compB);

    printf("\n--- Ternary Search ---\n");
    if (idxT != -1) printf("Found %d at index %d\n", x, idxT);
    else printf("%d not found\n", x);
    printf("Comparisons used: %ld\n", compT);

    printf("\nConclusion: Binary search used %s comparisons than ternary search.\n",
           compB < compT ? "fewer" : (compB > compT ? "more" : "the same number of"));

    free(arr);
    return 0;
}
