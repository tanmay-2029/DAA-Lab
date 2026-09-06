/*
 * Q3: Longest Common Subsequence (LCS) using Dynamic Programming
 *
 * Given two strings, find the length of their longest common
 * subsequence and reconstruct the subsequence itself.
 *
 * Time Complexity  : O(m * n)
 * Space Complexity : O(m * n)  -- can be reduced to O(min(m, n)) if only
 *                                 the length (not the actual subsequence)
 *                                 is required.
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int maxInt(int a, int b) { return a > b ? a : b; }

int main(void) {
    char X[1000], Y[1000];
    printf("=== Longest Common Subsequence (Dynamic Programming) ===\n");
    printf("Enter first string  : ");
    scanf("%999s", X);
    printf("Enter second string : ");
    scanf("%999s", Y);

    int m = (int) strlen(X), n = (int) strlen(Y);

    int **dp = malloc((m + 1) * sizeof(int *));
    for (int i = 0; i <= m; i++) dp[i] = calloc(n + 1, sizeof(int));

    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (X[i - 1] == Y[j - 1])
                dp[i][j] = dp[i - 1][j - 1] + 1;
            else
                dp[i][j] = maxInt(dp[i - 1][j], dp[i][j - 1]);
        }
    }

    int lcsLen = dp[m][n];
    char *lcs = malloc((lcsLen + 1) * sizeof(char));
    lcs[lcsLen] = '\0';

    int i = m, j = n, idx = lcsLen - 1;
    while (i > 0 && j > 0) {
        if (X[i - 1] == Y[j - 1]) {
            lcs[idx--] = X[i - 1];
            i--; j--;
        } else if (dp[i - 1][j] >= dp[i][j - 1]) {
            i--;
        } else {
            j--;
        }
    }

    printf("\nLength of LCS : %d\n", lcsLen);
    printf("LCS string    : %s\n", lcs);
    printf("\nTime Complexity  : O(m * n)\n");
    printf("Space Complexity : O(m * n)\n");

    for (int k = 0; k <= m; k++) free(dp[k]);
    free(dp);
    free(lcs);
    return 0;
}
