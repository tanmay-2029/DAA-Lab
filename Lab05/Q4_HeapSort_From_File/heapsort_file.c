/*
 * DAA LAB-5 : Problem 4
 * Implement Heap Sort to sort N randomly generated elements stored in a
 * file. Do the complexity analysis of your algorithm.
 *
 * Workflow:
 *   1. Generate N random integers and write them to "data/input.txt".
 *   2. Read the N integers back from "data/input.txt" into an array.
 *   3. Build a max-heap from the array, then repeatedly extract the max
 *      element to produce a sorted array (classic in-place Heap Sort).
 *   4. Write the sorted array to "data/output.txt" and print it.
 *
 * Complexity Analysis
 * --------------------
 * Let N be the number of elements.
 *
 * 1) Build-Heap phase (heapify all internal nodes bottom-up):
 *      Although a single heapify call costs O(log N), summing the cost
 *      over all N/2 internal nodes (most of which are near the bottom
 *      and need very few swaps) gives a TIGHT bound of O(N), not
 *      O(N log N). This is a classic amortized-analysis result.
 *
 * 2) Extraction phase:
 *      We perform N extract-max operations. Each extraction:
 *        - swaps the root with the last element            -> O(1)
 *        - reduces heap size by 1
 *        - sifts the new root down                          -> O(log N)
 *      Total for this phase = O(N log N).
 *
 * Overall Time Complexity : O(N) + O(N log N) = O(N log N)
 *   - This holds for the BEST, AVERAGE, and WORST case alike -- unlike
 *     Quick Sort, Heap Sort has no O(N^2) worst case.
 *
 * Space Complexity : O(1) auxiliary -- Heap Sort sorts in-place using
 *   only the input array (the "heap" IS the array), plus O(log N) for
 *   the recursion stack in the recursive heapify (or O(1) if heapify is
 *   implemented iteratively).
 *
 * Note: Heap Sort is NOT a stable sort.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define INPUT_FILE  "data/input.txt"
#define OUTPUT_FILE "data/output.txt"

void swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

/* Sift the node at index i down so the subtree rooted at i satisfies
 * the max-heap property. 'heapSize' is the current usable size of the
 * heap within arr[]. */
void heapify(int arr[], int heapSize, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < heapSize && arr[left] > arr[largest])
        largest = left;

    if (right < heapSize && arr[right] > arr[largest])
        largest = right;

    if (largest != i) {
        swap(&arr[i], &arr[largest]);
        heapify(arr, heapSize, largest); /* recurse into affected subtree */
    }
}

void heapSort(int arr[], int n) {
    /* Build max heap: start from the last internal (non-leaf) node */
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i);
    }

    /* Repeatedly move current root (max) to the end, shrink heap, heapify */
    for (int i = n - 1; i > 0; i--) {
        swap(&arr[0], &arr[i]);
        heapify(arr, i, 0);
    }
}

void generateRandomFile(const char *filename, int n) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printf("Could not open %s for writing.\n", filename);
        exit(1);
    }
    for (int i = 0; i < n; i++) {
        fprintf(fp, "%d\n", rand() % 10000);
    }
    fclose(fp);
}

int *readArrayFromFile(const char *filename, int n) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("Could not open %s for reading.\n", filename);
        exit(1);
    }
    int *arr = (int *) malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        if (fscanf(fp, "%d", &arr[i]) != 1) {
            printf("File does not contain enough numbers.\n");
            exit(1);
        }
    }
    fclose(fp);
    return arr;
}

void writeArrayToFile(const char *filename, int arr[], int n) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printf("Could not open %s for writing.\n", filename);
        exit(1);
    }
    for (int i = 0; i < n; i++) {
        fprintf(fp, "%d\n", arr[i]);
    }
    fclose(fp);
}

void printArray(int arr[], int n) {
    for (int i = 0; i < n; i++) printf("%d ", arr[i]);
    printf("\n");
}

int main() {
    int n;
    srand((unsigned int) time(NULL));

    printf("Enter number of random elements to generate (N): ");
    scanf("%d", &n);

    if (n <= 0) {
        printf("N must be positive.\n");
        return 1;
    }

    generateRandomFile(INPUT_FILE, n);
    printf("Generated %d random numbers into %s\n", n, INPUT_FILE);

    int *arr = readArrayFromFile(INPUT_FILE, n);

    printf("Unsorted (from file): ");
    printArray(arr, n);

    heapSort(arr, n);

    printf("Sorted: ");
    printArray(arr, n);

    writeArrayToFile(OUTPUT_FILE, arr, n);
    printf("Sorted numbers written to %s\n", OUTPUT_FILE);

    free(arr);
    return 0;
}
