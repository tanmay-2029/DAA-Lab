/*
 * DAA LAB-5 : Problem 3
 * Implement Quick Sort of N random elements stored in a file.
 *
 * Workflow:
 *   1. Generate N random integers and write them to "data/input.txt".
 *   2. Read the N integers back from "data/input.txt" into an array.
 *   3. Quick Sort the array in-place.
 *   4. Write the sorted array to "data/output.txt" and print it.
 *
 * Complexity Analysis (see README.md for full derivation)
 * ---------------------------------------------------------
 * - Best / Average case : O(N log N)
 * - Worst case           : O(N^2)   (e.g. already sorted input with a
 *                           naive last-element pivot)
 * - Space Complexity     : O(log N) average (recursion stack), O(N) worst
 *                           case recursion depth.
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

void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

/* Generates N random numbers in [0, 9999] and writes them to a file */
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

    /* Step 1: generate N random numbers into a file */
    generateRandomFile(INPUT_FILE, n);
    printf("Generated %d random numbers into %s\n", n, INPUT_FILE);

    /* Step 2: read them back from the file */
    int *arr = readArrayFromFile(INPUT_FILE, n);

    printf("Unsorted (from file): ");
    printArray(arr, n);

    /* Step 3: quick sort */
    quickSort(arr, 0, n - 1);

    printf("Sorted: ");
    printArray(arr, n);

    /* Step 4: write sorted result to file */
    writeArrayToFile(OUTPUT_FILE, arr, n);
    printf("Sorted numbers written to %s\n", OUTPUT_FILE);

    free(arr);
    return 0;
}
