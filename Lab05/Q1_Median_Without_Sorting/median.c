/*
 * DAA LAB-5 : Problem 1
 * Find the median of a list of N numbers WITHOUT sorting the list.
 *
 * Approach : Randomized Quickselect
 * ----------------------------------
 * The median is just the (N/2)-th order statistic (or the average of the
 * two middle order statistics when N is even). Instead of sorting the
 * whole array (O(N log N)) we can find the k-th smallest element directly
 * using the Quickselect algorithm, which is a variant of Quicksort that
 * only recurses into the partition that contains the element we want.
 *
 * Complexity Analysis
 * --------------------
 * Let T(N) be the expected running time.
 * - Partitioning the array around a pivot takes O(N) time.
 * - With a *random* pivot, the expected split is balanced enough that,
 *   on average, T(N) = T(N/2) + O(N)  =>  T(N) = O(N)   (expected case)
 * - Worst case (always picking the smallest/largest element as pivot,
 *   e.g. an already sorted array with a bad pivot rule):
 *       T(N) = T(N-1) + O(N)  =>  T(N) = O(N^2)
 * - Using the "median of medians" pivot selection strategy guarantees
 *   O(N) worst case, at the cost of a larger constant factor. This file
 *   implements the simpler randomized version; a deterministic O(N)
 *   worst-case version (median-of-medians) is described in the README.
 *
 * Space Complexity : O(1) extra (in-place partitioning, ignoring the
 *                    O(log N) average recursion stack).
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

/* Lomuto partition scheme around arr[high] */
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

/* Choose a random pivot, swap it to the end, then partition */
int randomizedPartition(int arr[], int low, int high) {
    int randomIndex = low + rand() % (high - low + 1);
    swap(&arr[randomIndex], &arr[high]);
    return partition(arr, low, high);
}

/*
 * quickSelect returns the element that WOULD be at index k
 * if the array were sorted (0-indexed), without fully sorting it.
 */
int quickSelect(int arr[], int low, int high, int k) {
    if (low == high) return arr[low];

    int pivotIndex = randomizedPartition(arr, low, high);

    if (k == pivotIndex) {
        return arr[k];
    } else if (k < pivotIndex) {
        return quickSelect(arr, low, pivotIndex - 1, k);
    } else {
        return quickSelect(arr, pivotIndex + 1, high, k);
    }
}

double findMedian(int arr[], int n) {
    if (n % 2 == 1) {
        return (double) quickSelect(arr, 0, n - 1, n / 2);
    } else {
        /* Average of the two middle elements: (n/2 - 1) and (n/2) */
        int a = quickSelect(arr, 0, n - 1, n / 2 - 1);
        int b = quickSelect(arr, 0, n - 1, n / 2);
        return (a + b) / 2.0;
    }
}

int main() {
    int n;
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

    double median = findMedian(arr, n);
    printf("Median = %.2lf\n", median);

    free(arr);
    return 0;
}
