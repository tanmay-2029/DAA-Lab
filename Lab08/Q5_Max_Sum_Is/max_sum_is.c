/*
 * Q5: Maximum Sum Increasing Subsequence
 * Lab-08, DAA
 *
 * Same idea as LIS but instead of length we store the SUM.
 *   msis[i] = a[i] + max(msis[j])  for j < i and a[j] < a[i]
 *   (if no such j, msis[i] = a[i])
 * Answer = max of all msis[i]
 *
 * Time  : O(n^2)
 * Space : O(n)
 */
#include <stdio.h>

#define MAX 10000

int main()
{
    int n, i, j;
    int a[MAX], prev[MAX];
    long long msis[MAX];

    printf("Enter size of array (1 to %d): ", MAX);
    scanf("%d", &n);
    if (n < 1 || n > MAX) {
        printf("Invalid size!\n");
        return 1;
    }

    printf("Enter %d positive integers: ", n);
    for (i = 0; i < n; i++) {
        scanf("%d", &a[i]);
        if (a[i] <= 0) {
            printf("Only positive integers are allowed!\n");
            return 1;
        }
    }

    for (i = 0; i < n; i++) {
        msis[i] = a[i];
        prev[i] = -1;
        for (j = 0; j < i; j++) {
            if (a[j] < a[i] && msis[j] + a[i] > msis[i]) {
                msis[i] = msis[j] + a[i];
                prev[i] = j;
            }
        }
    }

    int best = 0;
    for (i = 1; i < n; i++)
        if (msis[i] > msis[best])
            best = i;

    printf("\nMaximum sum of increasing subsequence = %lld\n", msis[best]);

    int seq[MAX], count = 0;
    for (i = best; i != -1; i = prev[i])
        seq[count++] = a[i];

    printf("Elements: ");
    for (i = count - 1; i >= 0; i--)
        printf("%d ", seq[i]);
    printf("\n");

    return 0;
}
