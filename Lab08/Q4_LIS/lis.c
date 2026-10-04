/*
 * Q4: Longest Increasing Subsequence (strictly increasing)
 * Lab-08, DAA
 *
 * lis[i] = length of the LIS that ENDS at index i
 *   lis[i] = 1 + max(lis[j])  for all j < i with a[j] < a[i]
 * (if no such j exists then lis[i] = 1)
 * Answer = max of all lis[i]
 *
 * prev[i] stores the previous index so we can print the subsequence too.
 *
 * Time  : O(n^2)   (two nested loops)
 * Space : O(n)
 */
#include <stdio.h>

#define MAX 10000

int main()
{
    int n, i, j;
    int a[MAX], lis[MAX], prev[MAX];

    printf("Enter size of array (1 to %d): ", MAX);
    scanf("%d", &n);
    if (n < 1 || n > MAX) {
        printf("Invalid size!\n");
        return 1;
    }

    printf("Enter %d integers: ", n);
    for (i = 0; i < n; i++)
        scanf("%d", &a[i]);

    for (i = 0; i < n; i++) {
        lis[i] = 1;
        prev[i] = -1;
        for (j = 0; j < i; j++) {
            if (a[j] < a[i] && lis[j] + 1 > lis[i]) {
                lis[i] = lis[j] + 1;
                prev[i] = j;
            }
        }
    }

    // find the best ending index
    int best = 0;
    for (i = 1; i < n; i++)
        if (lis[i] > lis[best])
            best = i;

    printf("\nLength of LIS = %d\n", lis[best]);

    // collect the elements going backwards, then print in right order
    int seq[MAX], count = 0;
    for (i = best; i != -1; i = prev[i])
        seq[count++] = a[i];

    printf("One LIS: ");
    for (i = count - 1; i >= 0; i--)
        printf("%d ", seq[i]);
    printf("\n");

    return 0;
}
