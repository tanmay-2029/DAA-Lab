#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// 1: optimized bubble sort with swap flag
long bubblesortoptimized(int arr[], int n) {
    long comparisons = 0;
    int swapped;
    
    for (int i = 0; i < n - 1; i++) {
        swapped = 0;
        for (int j = 0; j < n - 1 - i; j++) {
            comparisons++;
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                swapped = 1;
            }
        }
        if (swapped == 0) {
            break;
        }
    }
    return comparisons;
}

// 2: naive bubble sort without flag
long bubblesortnaive(int arr[], int n) {
    long comparisons = 0;
    
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            comparisons++;
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
    return comparisons;
}

int main() {
    srand(time(NULL));

    FILE *fp = fopen("bubble_sort_comparisons.csv", "w");
    fprintf(fp, "n,optimised_comparisons,naive_comparisons\n");

    printf("Running bubble sort analysis...\n");
    printf("n\tOptimized\tNaive\n");

    for (int n = 10; n <= 2000; n = n + 10) {
        int *arr1 = (int*)malloc(n * sizeof(int));
        int *arr2 = (int*)malloc(n * sizeof(int));

        //fill array with random numbers
        for (int i = 0; i < n; i++) {
            int val = rand() % 100000;
            arr1[i] = val;
            arr2[i] = val; //copy same numbers
        }

        long c1 = bubblesortoptimized(arr1, n);
        long c2 = bubblesortnaive(arr2, n);

        fprintf(fp, "%d,%ld,%ld\n", n, c1, c2);

        if (n % 200 == 0) {
            printf("%d\t%ld\t\t%ld\n", n, c1, c2);
        }

        free(arr1);
        free(arr2);
    }
    fclose(fp);

    printf("\nTesting Best Case (already sorted array of size 1000):\n");
    int size = 1000;
    int *sorted1 = (int*)malloc(size * sizeof(int));
    int *sorted2 = (int*)malloc(size * sizeof(int));

    for (int i = 0; i < size; i++) {
        sorted1[i] = i;
        sorted2[i] = i;
    }

    long best1 = bubblesortoptimized(sorted1, size);
    long best2 = bubblesortnaive(sorted2, size);

    printf("Optimized comparisons: %ld\n", best1);
    printf("Naive comparisons: %ld\n", best2);

    free(sorted1);
    free(sorted2);

    printf("\nDone! CSV output saved to bubble_sort_comparisons.csv\n");
    return 0;
}
