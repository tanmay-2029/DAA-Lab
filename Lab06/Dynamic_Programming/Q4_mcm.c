/*
 * Q4: Matrix Chain Multiplication using Dynamic Programming
 *
 * Given the dimensions of N-1 matrices in an array arr[] of size N
 * (matrix i has dimensions arr[i-1] x arr[i]), determine the minimum
 * number of scalar multiplications needed to multiply the complete
 * chain, and the optimal parenthesization.
 *
 * Sample from the lab sheet:
 *   Input : N = 4, arr[] = {10, 30, 5, 60}
 *   Output: 4500
 *
 * Time Complexity  : O(N^3)
 * Space Complexity : O(N^2)
 */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int **splitTable;

void printOptimalParens(int i, int j) {
    if (i == j) {
        printf("M%d", i);
        return;
    }
    printf("(");
    printOptimalParens(i, splitTable[i][j]);
    printOptimalParens(splitTable[i][j] + 1, j);
    printf(")");
}

int main(void) {
    int N;
    printf("=== Matrix Chain Multiplication (Dynamic Programming) ===\n");
    printf("Enter N (number of dimension values, i.e. number of matrices + 1): ");
    scanf("%d", &N);

    int *arr = malloc(N * sizeof(int));
    printf("Enter %d dimension values arr[0..%d]:\n", N, N - 1);
    for (int i = 0; i < N; i++) {
        printf("  arr[%d] = ", i);
        scanf("%d", &arr[i]);
    }

    int n = N;
    int **dp = malloc(n * sizeof(int *));
    splitTable = malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
        dp[i] = calloc(n, sizeof(int));
        splitTable[i] = calloc(n, sizeof(int));
    }

    for (int len = 2; len < n; len++) {
        for (int i = 1; i < n - len + 1; i++) {
            int j = i + len - 1;
            dp[i][j] = INT_MAX;
            for (int k = i; k < j; k++) {
                int cost = dp[i][k] + dp[k + 1][j] + arr[i - 1] * arr[k] * arr[j];
                if (cost < dp[i][j]) {
                    dp[i][j] = cost;
                    splitTable[i][j] = k;
                }
            }
        }
    }

    printf("\nMinimum number of scalar multiplications = %d\n", dp[1][n - 1]);
    printf("Optimal parenthesization                 = ");
    printOptimalParens(1, n - 1);
    printf("\n");
    printf("\nTime Complexity  : O(N^3)\n");
    printf("Space Complexity : O(N^2)\n");

    for (int i = 0; i < n; i++) { free(dp[i]); free(splitTable[i]); }
    free(dp);
    free(splitTable);
    free(arr);
    return 0;
}
