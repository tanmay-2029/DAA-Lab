/*
 * Q7: Rod Cutting with Reconstruction
 * Lab-08, DAA
 *
 * r[j] = max revenue for a rod of length j
 *   r[j] = max( p[i] + r[j - i] )   for i = 1..j
 * cut[j] remembers the first piece length i that gave the best answer,
 * so we can print the pieces at the end.
 *
 * Time  : O(n^2)
 * Space : O(n)
 */
#include <stdio.h>

#define MAX 10000

int main()
{
    int n, i, j;
    int p[MAX + 1], cut[MAX + 1];
    long long r[MAX + 1];

    printf("Enter rod length n (1 to %d): ", MAX);
    scanf("%d", &n);
    if (n < 1 || n > MAX) {
        printf("Invalid length!\n");
        return 1;
    }

    printf("Enter prices of pieces of length 1 to %d:\n", n);
    for (i = 1; i <= n; i++) {
        printf("  price of length %d: ", i);
        scanf("%d", &p[i]);
    }

    r[0] = 0;
    for (j = 1; j <= n; j++) {
        r[j] = -1;
        for (i = 1; i <= j; i++) {
            if (p[i] + r[j - i] > r[j]) {
                r[j] = p[i] + r[j - i];
                cut[j] = i;
            }
        }
    }

    printf("\nMaximum revenue = %lld\n", r[n]);
    printf("Pieces: ");
    int rem = n;
    int pieces = 0;
    while (rem > 0) {
        printf("%d ", cut[rem]);
        rem = rem - cut[rem];
        pieces++;
    }
    printf("\n(%d piece(s), total length = %d)\n", pieces, n);

    return 0;
}
