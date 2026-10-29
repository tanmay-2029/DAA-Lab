/*
 * Q4: Minimum Cost to Connect Sticks
 * ----------------------------------
 * Input : n and the lengths L1 .. Ln.
 *
 * Greedy (same as Huffman / optimal merge pattern):
 *   Keep all sticks in a min-heap. Repeatedly remove the two shortest sticks
 *   x and y, pay x + y, and insert the new stick x + y. Stop at one stick.
 *   Why optimal: each original stick is paid for once per merge it takes
 *   part in, i.e. cost = sum L_i * depth_i in the merge tree, which is
 *   exactly the Huffman cost, minimised by merging the two smallest first.
 *
 * Validation: for n <= 8, try every possible pair at every step (brute force).
 *
 * Complexity:
 *   Greedy      : O(n log n) time (n-1 merges, each O(log n)), O(n) space
 *   Brute force : O( prod_{k=2..n} k(k-1)/2 ) = O(n! (n-1)! / 2^(n-1))
 */
#include <stdio.h>
#include <stdlib.h>

static long long *heap;
static int heapSize = 0;

static void push(long long v) {
    int i = heapSize++;
    heap[i] = v;
    while (i > 0 && heap[(i - 1) / 2] > heap[i]) {
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
        if (l < heapSize && heap[l] < heap[m]) m = l;
        if (r < heapSize && heap[r] < heap[m]) m = r;
        if (m == i) break;
        long long t = heap[i]; heap[i] = heap[m]; heap[m] = t; i = m;
    }
    return top;
}

static long long bruteForce(long long *a, int n) {
    if (n <= 1) return 0;
    long long best = -1;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            long long b[16];
            int m = 0;
            for (int k = 0; k < n; k++) if (k != i && k != j) b[m++] = a[k];
            b[m++] = a[i] + a[j];
            long long c = a[i] + a[j] + bruteForce(b, m);
            if (best < 0 || c < best) best = c;
        }
    }
    return best;
}

int main(void) {
    int n;
    printf("Enter number of sticks: ");
    if (scanf("%d", &n) != 1 || n < 1) { printf("Invalid input\n"); return 1; }
    long long *L = malloc(n * sizeof(long long));
    heap = malloc(n * sizeof(long long));
    printf("Enter %d stick lengths: ", n);
    for (int i = 0; i < n; i++) { scanf("%lld", &L[i]); push(L[i]); }

    long long total = 0;
    int step = 0;
    printf("\nMerges:\n");
    while (heapSize > 1) {
        long long x = pop(), y = pop();
        total += x + y;
        printf("  %2d: %lld + %lld = %lld   (total so far %lld)\n", ++step, x, y, x + y, total);
        push(x + y);
    }

    printf("\nMinimum total cost = %lld\n", total);

    if (n <= 8) {
        long long bf = bruteForce(L, n);
        printf("Brute-force check  = %lld  (%s)\n", bf, bf == total ? "MATCH" : "MISMATCH");
    }

    free(L);
    free(heap);
    return 0;
}
