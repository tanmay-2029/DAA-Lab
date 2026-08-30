/*
 * DAA LAB-5 : Problem 2
 * Find the K'th smallest element in a given list of N numbers
 * WITHOUT sorting the list.
 *
 * Approach : Randomized Quickselect (same idea as Q1, generalized to an
 * arbitrary rank K instead of only the median).
 *
 * Complexity Analysis
 * --------------------
 * - Average / Expected case : O(N)
 *      Each partition step costs O(N), and with a random pivot the
 *      expected size of the sub-array we recurse into shrinks by a
 *      constant factor each time, giving the recurrence
 *      T(N) = T(N/2) + O(N)  =>  T(N) = O(N).
 * - Worst case : O(N^2)
 *      Happens if the pivot chosen is always the smallest or largest
 *      remaining element (extremely unlikely with random pivots, but
 *      possible in the worst case).
 * - Space Complexity : O(1) auxiliary (in-place), O(log N) expected
 *      recursion stack depth.
 *
 * Note: This is asymptotically better than sorting (O(N log N)) followed
 * by a direct index lookup, whenever only a single order statistic (like
 * the K'th smallest) is needed.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

int randomizedPartition(int arr[], int low, int high) {
    int randomIndex = low + rand() % (high - low + 1);
    swap(&arr[randomIndex], &arr[high]);
    return partition(arr, low, high);
}

/* Returns the k-th smallest element (1-indexed: k=1 means smallest) */
int quickSelect(int arr[], int low, int high, int k) {
    if (low == high) return arr[low];

    int pivotIndex = randomizedPartition(arr, low, high);
    int rank = pivotIndex - low + 1; /* rank of pivot within arr[low..high] */

    if (rank == k) {
        return arr[pivotIndex];
    } else if (k < rank) {
        return quickSelect(arr, low, pivotIndex - 1, k);
    } else {
        return quickSelect(arr, pivotIndex + 1, high, k - rank);
    }
}

int main() {
    int n, k;
    srand((unsigned int) time(NULL));

    printf("Enter number of elements (N): ");
    scanf("%d", &n);

    if (n <= 0) {
        printf("N must be positive.\n");
        return 1;
    }

    int *arr = (int *) malloc(n * sizeof(int));
    if (!arr) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    printf("Enter %d numbers:\n", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    printf("Enter K (1 <= K <= N): ");
    scanf("%d", &k);

    if (k < 1 || k > n) {
        printf("Invalid K.\n");
        free(arr);
        return 1;
    }

    int result = quickSelect(arr, 0, n - 1, k);
    printf("%d-th smallest element = %d\n", k, result);

    free(arr);
    return 0;
}
