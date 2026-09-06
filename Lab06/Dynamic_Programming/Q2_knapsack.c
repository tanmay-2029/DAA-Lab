/*
 * Q2: 0/1 Knapsack Problem using Dynamic Programming
 *
 * Given n items each with a weight and a profit, and a knapsack of
 * capacity W, determine the maximum profit obtainable such that the
 * total weight of chosen items does not exceed W. Each item can be
 * picked at most once (0/1 property).
 *
 * Time Complexity  : O(n * W)
 * Space Complexity : O(n * W)  -- can be reduced to O(W) using a 1-D
 *                                 rolling array processed right-to-left.
 */
#include <stdio.h>
#include <stdlib.h>

int maxInt(int a, int b) { return (a > b) ? a : b; }

int knapsack(int n, int W, int weight[], int profit[], int **dp) {
    for (int i = 0; i <= n; i++) {
        for (int w = 0; w <= W; w++) {
            if (i == 0 || w == 0) {
                dp[i][w] = 0;
            } else if (weight[i - 1] <= w) {
                dp[i][w] = maxInt(profit[i - 1] + dp[i - 1][w - weight[i - 1]], dp[i - 1][w]);
            } else {
                dp[i][w] = dp[i - 1][w];
            }
        }
    }
    return dp[n][W];
}

void printSelectedItems(int n, int W, int weight[], int profit[], int **dp) {
    int w = W;
    printf("Items included in the optimal knapsack:\n");
    for (int i = n; i > 0 && w >= 0; i--) {
        if (dp[i][w] != dp[i - 1][w]) {
            printf("  Item %d  (weight = %d, profit = %d)\n", i, weight[i - 1], profit[i - 1]);
            w -= weight[i - 1];
        }
    }
}

int main(void) {
    int n, W;
    printf("=== 0/1 Knapsack Problem (Dynamic Programming) ===\n");
    printf("Enter number of items (n): ");
    scanf("%d", &n);
    printf("Enter capacity of knapsack (W): ");
    scanf("%d", &W);

    int *weight = malloc(n * sizeof(int));
    int *profit = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        printf("Enter weight and profit of item %d (space separated): ", i + 1);
        scanf("%d %d", &weight[i], &profit[i]);
    }

    int **dp = malloc((n + 1) * sizeof(int *));
    for (int i = 0; i <= n; i++) dp[i] = malloc((W + 1) * sizeof(int));

    int result = knapsack(n, W, weight, profit, dp);

    printf("\nMaximum profit obtainable = %d\n", result);
    printSelectedItems(n, W, weight, profit, dp);

    printf("\nTime Complexity  : O(n * W)\n");
    printf("Space Complexity : O(n * W)  [O(W) achievable with a 1-D rolling array]\n");

    for (int i = 0; i <= n; i++) free(dp[i]);
    free(dp);
    free(weight);
    free(profit);
    return 0;
}
