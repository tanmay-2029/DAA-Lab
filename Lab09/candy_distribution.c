/*
 * Q5: Candy Distribution (bi-directional slope greedy)
 * ----------------------------------------------------
 * Input : n and the ratings r[0..n-1].
 *
 * Algorithm:
 *   1. Give every child 1 candy.
 *   2. Left-to-right pass: if r[i] > r[i-1] then left[i] = left[i-1] + 1
 *      (handles every increasing slope).
 *   3. Right-to-left pass: if r[i] > r[i+1] then right[i] = right[i+1] + 1
 *      (handles every decreasing slope).
 *   4. candy[i] = max(left[i], right[i]); answer = sum of candy[i].
 *   Why minimal: left[i] is the length of the increasing run ending at i,
 *   and any valid distribution must give child i at least that many (the
 *   same holds for right[i]), so candy[i] = max(left, right) is a lower
 *   bound for every child and it is also valid.
 *
 * Validation: for n <= 7 every assignment with candies in 1..n is tried.
 *
 * Complexity:
 *   Greedy      : Theta(n) time (two passes), Theta(n) space
 *   Brute force : O(n^n * n)
 */
#include <stdio.h>
#include <stdlib.h>

static int n;
static int *r;

static int valid(const int *c) {
    for (int i = 0; i < n; i++) {
        if (c[i] < 1) return 0;
        if (i > 0 && r[i] > r[i - 1] && c[i] <= c[i - 1]) return 0;
        if (i < n - 1 && r[i] > r[i + 1] && c[i] <= c[i + 1]) return 0;
    }
    return 1;
}

static long long bruteBest;
static int bf[8];

static void brute(int i, long long sum) {
    if (sum >= bruteBest) return;
    if (i == n) {
        if (valid(bf)) bruteBest = sum;
        return;
    }
    for (int c = 1; c <= n; c++) {
        bf[i] = c;
        brute(i + 1, sum + c);
    }
}

int main(void) {
    printf("Enter number of children: ");
    if (scanf("%d", &n) != 1 || n < 1) { printf("Invalid input\n"); return 1; }
    r = malloc(n * sizeof(int));
    printf("Enter %d ratings: ", n);
    for (int i = 0; i < n; i++) scanf("%d", &r[i]);

    int *left = malloc(n * sizeof(int));
    int *right = malloc(n * sizeof(int));
    int *candy = malloc(n * sizeof(int));

    left[0] = 1;
    for (int i = 1; i < n; i++) left[i] = (r[i] > r[i - 1]) ? left[i - 1] + 1 : 1;
    right[n - 1] = 1;
    for (int i = n - 2; i >= 0; i--) right[i] = (r[i] > r[i + 1]) ? right[i + 1] + 1 : 1;

    long long total = 0;
    for (int i = 0; i < n; i++) {
        candy[i] = left[i] > right[i] ? left[i] : right[i];
        total += candy[i];
    }

    printf("\n%8s %8s %8s %8s\n", "rating", "L->R", "R->L", "candies");
    for (int i = 0; i < n; i++)
        printf("%8d %8d %8d %8d\n", r[i], left[i], right[i], candy[i]);

    printf("\nMinimum total candies = %lld\n", total);
    printf("Constraints satisfied = %s\n", valid(candy) ? "YES" : "NO");

    if (n <= 7) {
        bruteBest = (long long)n * n + 1;
        brute(0, 0);
        printf("Brute-force check     = %lld  (%s)\n", bruteBest, bruteBest == total ? "MATCH" : "MISMATCH");
    }

    free(r); free(left); free(right); free(candy);
    return 0;
}
