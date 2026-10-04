/*
 * Q1: Minimum Coin Change
 * Lab-08, DAA
 *
 * Idea: dp[i] = minimum coins needed to make amount i
 *       dp[i] = min( dp[i - c] + 1 )  for every coin c <= i
 *
 * Time  : O(n * V)
 * Space : O(V)
 */
#include <stdio.h>

#define MAX_COINS 50
#define MAX_V 100000
#define INF 1000000000   // means "not possible"

int dp[MAX_V + 1];
int lastCoin[MAX_V + 1];   // coin used to reach amount i (for printing the coins)

int main()
{
    int n, V, i, j;
    int coins[MAX_COINS];

    printf("Enter number of coin denominations (1 to %d): ", MAX_COINS);
    scanf("%d", &n);
    if (n < 1 || n > MAX_COINS) {
        printf("Invalid number of coins!\n");
        return 1;
    }

    printf("Enter the %d coin values: ", n);
    for (i = 0; i < n; i++) {
        scanf("%d", &coins[i]);
        if (coins[i] <= 0) {
            printf("Coin value must be positive!\n");
            return 1;
        }
    }

    printf("Enter target amount V (0 to %d): ", MAX_V);
    scanf("%d", &V);
    if (V < 0 || V > MAX_V) {
        printf("Invalid amount!\n");
        return 1;
    }

    // base case: 0 coins needed for amount 0
    dp[0] = 0;
    lastCoin[0] = -1;

    for (i = 1; i <= V; i++) {
        dp[i] = INF;
        lastCoin[i] = -1;
        for (j = 0; j < n; j++) {
            if (coins[j] <= i && dp[i - coins[j]] != INF && dp[i - coins[j]] + 1 < dp[i]) {
                dp[i] = dp[i - coins[j]] + 1;
                lastCoin[i] = coins[j];
            }
        }
    }

    if (dp[V] == INF) {
        printf("\nAnswer = -1 (amount %d cannot be made)\n", V);
    } else {
        printf("\nMinimum number of coins = %d\n", dp[V]);
        printf("Coins used: ");
        int cur = V;
        while (cur > 0) {
            printf("%d ", lastCoin[cur]);
            cur = cur - lastCoin[cur];
        }
        printf("\n");
    }

    return 0;
}
