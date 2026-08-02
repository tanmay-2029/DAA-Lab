/*
 * Name: Alex Chen
 * Roll No: 10240391
 * Lab Assignment 5: Partition Point Search (Linear vs Binary Search)
 */

#include <stdio.h>
#include <stdlib.h>

// Linear scan method: O(n)
int linearSearch(int arr[], int n, long *count) {
    *count = 0;
    for (int i = 0; i < n; i++) {
        (*count)++;
        if (arr[i] == 1) {
            return i; // found first 1
        }
    }
    return n; // no 1 found
}

// Binary search method: O(log n)
int binarySearch(int arr[], int n, long *count) {
    *count = 0;
    int low = 0;
    int high = n;
    
    while (low < high) {
        int mid = low + (high - low) / 2;
        (*count)++;
        
        if (arr[mid] == 1) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }
    return low;
}

// function to create array of 0s then 1s
void makeArray(int arr[], int n, int k) {
    for (int i = 0; i < n; i++) {
        if (i < k) {
            arr[i] = 0;
        } else {
            arr[i] = 1;
        }
    }
}

int main() {
    printf("=== Lab 5: Partition Point Search ===\n\n");

    // Demo example
    int demoArr[] = {0, 0, 0, 0, 0, 1, 1, 1};
    int demoSize = 8;
    long cLinear, cBinary;

    int pos1 = linearSearch(demoArr, demoSize, &cLinear);
    int pos2 = binarySearch(demoArr, demoSize, &cBinary);

    printf("Demo Array Result:\n");
    printf("Linear search found index %d using %ld comparisons\n", pos1, cLinear);
    printf("Binary search found index %d using %ld comparisons\n\n", pos2, cBinary);

    // Save comparison data to CSV file
    FILE *fp = fopen("partition_point_comparisons.csv", "w");
    fprintf(fp, "n,linear_comparisons,binary_comparisons\n");

    printf("Running size experiments...\n");
    printf("n\tLinear\tBinary\n");

    for (int n = 100; n <= 100000; n = n * 2) {
        int *A = (int*)malloc(n * sizeof(int));
        int k = n / 2; // place 1s starting at the middle
        makeArray(A, n, k);

        long lc, bc;
        linearSearch(A, n, &lc);
        binarySearch(A, n, &bc);

        fprintf(fp, "%d,%ld,%ld\n", n, lc, bc);
        printf("%d\t%ld\t%ld\n", n, lc, bc);

        free(A);
    }

    fclose(fp);
    printf("\nData saved to partition_point_comparisons.csv successfully!\n");

    return 0;
}
