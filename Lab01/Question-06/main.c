/*
 * Name: Alex Chen
 * Roll No: 10240391
 * Lab Assignment 6: Element Uniqueness Analysis
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Comparator for qsort
int compareInts(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

// Brute force check: compares every pair
int checkBruteForce(int arr[], int n, long *count) {
    *count = 0;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            (*count)++;
            if (arr[i] == arr[j]) {
                return 1; // found duplicate
            }
        }
    }
    return 0; // all unique
}

// Sort then check adjacent elements
int checkSorted(int arr[], int n) {
    int *temp = (int*)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        temp[i] = arr[i];
    }

    // sort array
    qsort(temp, n, sizeof(int), compareInts);

    int foundDuplicate = 0;
    for (int i = 0; i < n - 1; i++) {
        if (temp[i] == temp[i + 1]) {
            foundDuplicate = 1;
            break;
        }
    }

    free(temp);
    return foundDuplicate;
}

// Fill array with unique numbers and shuffle
void createUniqueArray(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = i;
    }
    // simple shuffle
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

int main() {
    srand(time(NULL));

    printf("=== Lab 6: Element Uniqueness Analysis ===\n\n");

    // Demo with small array
    int demo[] = {4, 2, 7, 1, 9, 4, 6};
    int demoSize = 7;
    long demoComps = 0;

    int isDup = checkBruteForce(demo, demoSize, &demoComps);
    if (isDup == 1) {
        printf("Demo array has duplicates! (%ld comparisons)\n\n", demoComps);
    } else {
        printf("Demo array has no duplicates! (%ld comparisons)\n\n", demoComps);
    }

    FILE *fp = fopen("uniqueness_timing.csv", "w");
    fprintf(fp, "n,bruteforce_comparisons,bruteforce_time_sec,sorted_time_sec\n");

    printf("Running performance test...\n");
    printf("n\tBrute Comparisons\tBrute Time(s)\tSorted Time(s)\n");

    for (int n = 500; n <= 8000; n = n + 500) {
        int *A = (int*)malloc(n * sizeof(int));
        createUniqueArray(A, n);

        long compCount = 0;

        clock_t start1 = clock();
        checkBruteForce(A, n, &compCount);
        clock_t end1 = clock();

        checkSorted(A, n);
        clock_t end2 = clock();

        double timeBrute = (double)(end1 - start1) / CLOCKS_PER_SEC;
        double timeSorted = (double)(end2 - end1) / CLOCKS_PER_SEC;

        fprintf(fp, "%d,%ld,%.6f,%.6f\n", n, compCount, timeBrute, timeSorted);
        printf("%d\t%ld\t\t%.6f\t%.6f\n", n, compCount, timeBrute, timeSorted);

        free(A);
    }

    fclose(fp);
    printf("\nData saved to uniqueness_timing.csv successfully!\n");

    return 0;
}
