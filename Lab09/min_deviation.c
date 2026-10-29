/*
 * Q7: Minimise Deviation in Array (two-way greedy with a max-heap)
 * ----------------------------------------------------------------
 * Input : n and n positive integers.
 * Allowed: odd x -> 2x, even x -> x/2 (any number of times).
 *
 * Observation: every element can only take values in a fixed chain:
 *   odd x  : {x, 2x}
 *   even x : {x, x/2, x/4, ..., odd part of x}
 * So first push every element to its MAXIMUM (double the odd ones); after
 * that the only useful move is halving.
 *
 * Greedy:
 *   1. Double all odd numbers; insert all into a max-heap; track the minimum.
 *   2. Repeat: take the maximum M. deviation = min(deviation, M - minimum).
 *      If M is odd, stop (the maximum can no longer decrease).
 *      Otherwise replace M by M/2, update the minimum, push it back.
 *   Halving the maximum is the only move that can lower max - min; halving
 *   anything else only lowers the minimum and increases the deviation.
 *
 * Validation (exact, different method): list every reachable (value, index)
 * pair, sort by value, and find the smallest window that contains at least
 * one value of every index (sliding window).
 *
 * Complexity (V = largest value):
 *   Each element is halved at most log2(V) times, so at most n log V heap
 *   operations: O(n log n log V) time, O(n) space.
 *   Validation: O(n log V log(n log V)) time, O(n log V) space.
 */
#include <stdio.h>
#include <stdlib.h>

static long long *heap;
static int heapSize = 0;

static void push(long long v) {
    int i = heapSize++;
    heap[i] = v;
    while (i > 0 && heap[(i - 1) / 2] < heap[i]) {
        long long t = heap[i]; heap[i] = heap[(i - 1) / 2]; heap[(i - 1) / 2] = t;
        i = (i - 1) / 2;
    }
}

static long long pop(void) {
    long long top = heap[0];
    heap[0] = heap[--heapSize];
    int i = 0;
    for (;;) {
        int l = 2 * i + 1, r = l + 1, m = i;
        if (l < heapSize && heap[l] > heap[m]) m = l;
        if (r < heapSize && heap[r] > heap[m]) m = r;
        if (m == i) break;
        long long t = heap[i]; heap[i] = heap[m]; heap[m] = t; i = m;
    }
    return top;
}

typedef struct { long long value; int idx; } Pair;

static int byValue(const void *a, const void *b) {
    const Pair *x = a, *y = b;
    return (x->value > y->value) - (x->value < y->value);
}

static long long slidingWindowCheck(const long long *A, int n) {
    int m = 0;
    Pair *p = malloc((size_t)n * 64 * sizeof(Pair));
    for (int i = 0; i < n; i++) {
        long long x = A[i] % 2 ? A[i] * 2 : A[i];
        for (;;) {
            p[m].value = x; p[m].idx = i; m++;
            if (x % 2) break;
            x /= 2;
        }
    }
    qsort(p, m, sizeof(Pair), byValue);

    int *have = calloc(n, sizeof(int));
    int covered = 0, lo = 0;
    long long best = -1;
    for (int hi = 0; hi < m; hi++) {
        if (have[p[hi].idx]++ == 0) covered++;
        while (covered == n) {
            long long d = p[hi].value - p[lo].value;
            if (best < 0 || d < best) best = d;
            if (--have[p[lo].idx] == 0) covered--;
            lo++;
        }
    }
    free(p);
    free(have);
    return best;
}

int main(void) {
    int n;
    printf("Enter number of elements: ");
    if (scanf("%d", &n) != 1 || n < 1) { printf("Invalid input\n"); return 1; }
    long long *A = malloc(n * sizeof(long long));
    heap = malloc(n * sizeof(long long));
    printf("Enter %d positive integers: ", n);
    for (int i = 0; i < n; i++) scanf("%lld", &A[i]);

    long long minimum = -1;
    for (int i = 0; i < n; i++) {
        long long x = A[i] % 2 ? A[i] * 2 : A[i];
        push(x);
        if (minimum < 0 || x < minimum) minimum = x;
    }

    long long best = -1, bestMax = 0, bestMin = 0;
    int steps = 0;
    for (;;) {
        long long mx = pop();
        if (best < 0 || mx - minimum < best) { best = mx - minimum; bestMax = mx; bestMin = minimum; }
        if (mx % 2) break;
        mx /= 2;
        steps++;
        if (mx < minimum) minimum = mx;
        push(mx);
    }

    printf("\nMinimum deviation = %lld  (max %lld - min %lld, after %d halvings)\n",
           best, bestMax, bestMin, steps);

    long long check = slidingWindowCheck(A, n);
    printf("Sliding-window check = %lld  (%s)\n", check, check == best ? "MATCH" : "MISMATCH");

    free(A);
    free(heap);
    return 0;
}
