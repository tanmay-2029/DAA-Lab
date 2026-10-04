/*
 * Q2: Coin Change - Total number of ways
 * Lab-08, DAA
 *
 * Idea: ways[j] = number of combinations that give sum j.
 * We take coins ONE BY ONE in the outer loop. This way 1+2 and 2+1
 * are not counted twice (order does not matter).
 *
 *   for each coin c:
 *       for j = c to V:
 *           ways[j] += ways[j - c]
 *
 * Time  : O(n * V)
 * Space : O(V)
 */
#include <stdio.h>

#define MAX_COINS 50
#define MAX_V 100000

unsigned long long ways[MAX_V + 1];

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

    printf("Enter the %d DISTINCT positive coin values: ", n);
    for (i = 0; i < n; i++) {
        scanf("%d", &coins[i]);
        if (coins[i] <= 0) {
            printf("Coin value must be positive!\n");
            return 1;
        }
    }

    // check distinct (simple O(n^2) check, n is small)
    for (i = 0; i < n; i++)
        for (j = i + 1; j < n; j++)
            if (coins[i] == coins[j]) {
                printf("Coins must be distinct! %d is repeated.\n", coins[i]);
                return 1;
            }

    printf("Enter target amount V (0 to %d): ", MAX_V);
    scanf("%d", &V);
    if (V < 0 || V > MAX_V) {
        printf("Invalid amount!\n");
        return 1;
    }

    ways[0] = 1;   // one way to make 0: take nothing

    for (i = 0; i < n; i++) {
        for (j = coins[i]; j <= V; j++) {
            ways[j] += ways[j - coins[i]];
        }
    }

    printf("\nTotal number of ways to make %d = %llu\n", V, ways[V]);
    printf("(note: value is stored in unsigned long long, very large V may overflow)\n");

    return 0;
}
