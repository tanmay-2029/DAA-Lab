/*
 * Q1: Nth Fibonacci number using Dynamic Programming (bottom-up tabulation)
 *
 * Time Complexity  : O(n)
 * Space Complexity : O(n)  -- can be reduced to O(1) by keeping only the
 *                             last two values instead of the full table.
 */
#include <stdio.h>
#include <stdlib.h>

long long fibonacciDP(int n, long long *additions) {
    if (n == 0) return 0;
    long long *dp = (long long *) malloc((n + 1) * sizeof(long long));
    dp[0] = 0;
    dp[1] = 1;
    for (int i = 2; i <= n; i++) {
        dp[i] = dp[i - 1] + dp[i - 2];
        (*additions)++;
    }
    long long result = dp[n];
    free(dp);
    return result;
}

int main(void) {
    int n;
    printf("=== Nth Fibonacci Number (Dynamic Programming) ===\n");
    printf("Enter n: ");
    if (scanf("%d", &n) != 1 || n < 0) {
        printf("Invalid input. n must be a non-negative integer.\n");
        return 1;
    }

    long long additions = 0;
    long long result = fibonacciDP(n, &additions);

    printf("\nFibonacci(%d) = %lld\n", n, result);
    printf("Additions performed : %lld\n", additions);
    printf("Time Complexity     : O(n)\n");
    printf("Space Complexity    : O(n)  [O(1) possible with two rolling variables]\n");
    return 0;
}
