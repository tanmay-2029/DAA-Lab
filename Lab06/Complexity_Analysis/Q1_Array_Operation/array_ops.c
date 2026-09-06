/*
 * Q1: 1D Array Operations and their Complexities
 *
 * Given an unsorted array of n integers, implement and analyse:
 *   (i)    Maximum element                      -> O(n)
 *   (ii)   First and second largest elements     -> O(n)
 *   (iii)  Mean                                  -> O(n)
 *   (iv)   Median                                -> O(n log n)  (sort-based)
 *   (v)    Standard deviation                    -> O(n)
 *   (vi)   Mode                                  -> O(n log n)  (sort-based)
 *   (vii)  Remove all duplicates                 -> O(n log n)  (sort-based)
 *   (viii) Reverse the array                     -> O(n)
 *   (ix)   Partition around a random pivot        -> O(n)
 *          (all elements >= pivot placed before all elements < pivot,
 *           as required by the problem statement)
 *
 * See ANALYSIS.md in this folder for the full complexity discussion,
 * including how (iv), (vi) and (vii) could be pushed to O(n) using
 * quickselect / hashing at the cost of worst-case guarantees.
 */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <time.h>

void printArray(int arr[], int n) {
    for (int i = 0; i < n; i++) printf("%d ", arr[i]);
    printf("\n");
}

/* (i) Maximum element : O(n) */
int findMax(int arr[], int n) {
    int mx = arr[0];
    for (int i = 1; i < n; i++)
        if (arr[i] > mx) mx = arr[i];
    return mx;
}

/* (ii) First and second largest : O(n) */
void firstSecondLargest(int arr[], int n, int *first, int *second) {
    *first = *second = INT_MIN;
    for (int i = 0; i < n; i++) {
        if (arr[i] > *first) {
            *second = *first;
            *first = arr[i];
        } else if (arr[i] > *second && arr[i] != *first) {
            *second = arr[i];
        }
    }
}

/* (iii) Mean : O(n) */
double findMean(int arr[], int n) {
    long long sum = 0;
    for (int i = 0; i < n; i++) sum += arr[i];
    return (double) sum / n;
}

int cmpInt(const void *a, const void *b) { return (*(int *) a - *(int *) b); }

/* (iv) Median : O(n log n) via sorting (O(n) average possible via quickselect) */
double findMedian(int arr[], int n) {
    int *copy = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) copy[i] = arr[i];
    qsort(copy, n, sizeof(int), cmpInt);
    double median = (n % 2 == 0) ? (copy[n / 2 - 1] + copy[n / 2]) / 2.0 : copy[n / 2];
    free(copy);
    return median;
}

/* (v) Standard deviation : O(n) */
double findStdDev(int arr[], int n) {
    double mean = findMean(arr, n);
    double sumSq = 0;
    for (int i = 0; i < n; i++) sumSq += (arr[i] - mean) * (arr[i] - mean);
    return sqrt(sumSq / n);
}

/* (vi) Mode : O(n log n) via sorting (O(n) possible with hashing) */
int findMode(int arr[], int n) {
    int *copy = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) copy[i] = arr[i];
    qsort(copy, n, sizeof(int), cmpInt);

    int mode = copy[0], maxCount = 1, curCount = 1;
    for (int i = 1; i < n; i++) {
        curCount = (copy[i] == copy[i - 1]) ? curCount + 1 : 1;
        if (curCount > maxCount) { maxCount = curCount; mode = copy[i]; }
    }
    free(copy);
    return mode;
}

/* (vii) Remove duplicates : O(n log n) via sorting (O(n) possible with hashing) */
int removeDuplicates(int arr[], int n, int result[]) {
    int *copy = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) copy[i] = arr[i];
    qsort(copy, n, sizeof(int), cmpInt);
    int k = 0;
    for (int i = 0; i < n; i++)
        if (i == 0 || copy[i] != copy[i - 1]) result[k++] = copy[i];
    free(copy);
    return k;
}

/* (viii) Reverse : O(n) */
void reverseArray(int arr[], int n) {
    int l = 0, r = n - 1;
    while (l < r) {
        int t = arr[l]; arr[l] = arr[r]; arr[r] = t;
        l++; r--;
    }
}

/* (ix) Partition around a random pivot : O(n)
 * Rearranges so that all elements >= pivot come first, followed by all
 * elements < pivot, as specified in the problem statement.
 */
int partitionAroundPivot(int arr[], int n) {
    int pivotIndex = rand() % n;
    int pivot = arr[pivotIndex];

    int i = 0, j = n - 1;
    while (i <= j) {
        while (i <= j && arr[i] >= pivot) i++;
        while (i <= j && arr[j] < pivot) j--;
        if (i < j) {
            int t = arr[i]; arr[i] = arr[j]; arr[j] = t;
            i++; j--;
        }
    }
    return pivot;
}

int main(void) {
    srand((unsigned) time(NULL));

    int n;
    printf("=== 1D Array Operations and their Complexities ===\n");
    printf("Enter number of elements (n): ");
    scanf("%d", &n);

    int *arr = malloc(n * sizeof(int));
    printf("Enter %d integer elements:\n", n);
    for (int i = 0; i < n; i++) scanf("%d", &arr[i]);

    int choice;
    do {
        printf("\n----------------- MENU -----------------\n");
        printf(" 1. Maximum element                (O(n))\n");
        printf(" 2. First & second largest         (O(n))\n");
        printf(" 3. Mean                           (O(n))\n");
        printf(" 4. Median                         (O(n log n))\n");
        printf(" 5. Standard deviation             (O(n))\n");
        printf(" 6. Mode                           (O(n log n))\n");
        printf(" 7. Remove duplicates              (O(n log n))\n");
        printf(" 8. Reverse array                  (O(n))\n");
        printf(" 9. Partition around random pivot  (O(n))\n");
        printf("10. Print current array\n");
        printf(" 0. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Maximum element = %d\n", findMax(arr, n));
                break;
            case 2: {
                int f, s;
                firstSecondLargest(arr, n, &f, &s);
                printf("First largest = %d, Second largest = %d\n", f, s);
                break;
            }
            case 3:
                printf("Mean = %.4f\n", findMean(arr, n));
                break;
            case 4:
                printf("Median = %.4f\n", findMedian(arr, n));
                break;
            case 5:
                printf("Standard Deviation = %.4f\n", findStdDev(arr, n));
                break;
            case 6:
                printf("Mode = %d\n", findMode(arr, n));
                break;
            case 7: {
                int *result = malloc(n * sizeof(int));
                int k = removeDuplicates(arr, n, result);
                printf("Array after removing duplicates (%d elements): ", k);
                printArray(result, k);
                free(result);
                break;
            }
            case 8:
                reverseArray(arr, n);
                printf("Array reversed: ");
                printArray(arr, n);
                break;
            case 9: {
                int pivot = partitionAroundPivot(arr, n);
                printf("Pivot chosen = %d\n", pivot);
                printf("Array after partitioning (>= pivot, then < pivot): ");
                printArray(arr, n);
                break;
            }
            case 10:
                printf("Current array: ");
                printArray(arr, n);
                break;
            case 0:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice.\n");
        }
    } while (choice != 0);

    free(arr);
    return 0;
}
