/*
 * Q5: Lexicographically Smallest Permutation using allowed swaps
 * --------------------------------------------------------------
 * Input : n, the permutation A[1..n], m, then m allowed swaps "u v"
 *         (u, v are POSITIONS 1..n; the swap may be used any number of times).
 *
 * Key fact: within one connected component of the swap graph, repeated
 * swaps can produce ANY arrangement of the values sitting in that component
 * (adjacent transpositions along a spanning tree generate every permutation).
 * Values can never move between components.
 *
 * Algorithm:
 *   1. DSU over positions; union(u, v) for every allowed swap.
 *   2. Group positions by their root.
 *   3. In each group, sort the positions and sort the values, then put the
 *      smallest value at the smallest position, and so on.
 *   This is the lexicographically smallest result: at each position, from
 *   left to right, it holds the smallest value still available to it.
 *
 * Validation: for n <= 8, BFS over every permutation reachable by the swaps
 * and pick the lexicographically smallest one.
 *
 * Complexity:
 *   DSU          : O((n + m) alpha(n))
 *   Grouping     : O(n)  (counting sort by root)
 *   Sorting      : O(n log n) over all groups
 *   Total        : O(n log n + m alpha(n)) time, O(n) space
 *   Brute force  : O(n! * m * n^2)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int *parent, *rnk;

static int find(int x) {
    while (parent[x] != x) { parent[x] = parent[parent[x]]; x = parent[x]; }
    return x;
}

static void unite(int a, int b) {
    a = find(a); b = find(b);
    if (a == b) return;
    if (rnk[a] < rnk[b]) { int t = a; a = b; b = t; }
    parent[b] = a;
    if (rnk[a] == rnk[b]) rnk[a]++;
}

static int cmpInt(const void *a, const void *b) {
    return *(const int *)a - *(const int *)b;
}

/* ---- brute force: BFS over permutations, each ranked to 0..n!-1 ---- */
static int n, m;
static int *su, *sv;

/* Lehmer-code rank of p[1..n]. */
static int rankPerm(const int *p) {
    int r = 0;
    for (int i = 1; i <= n; i++) {
        int smaller = 0;
        for (int j = i + 1; j <= n; j++) if (p[j] < p[i]) smaller++;
        r = r * (n - i + 1) + smaller;
    }
    return r;
}

static void bruteForce(const int *A, int *best) {
    int cap = 1;
    for (int i = 2; i <= n; i++) cap *= i;
    int (*q)[9] = malloc((size_t)cap * sizeof *q);
    char *seen = calloc(cap, 1);
    int head = 0, tail = 0;

    memcpy(q[tail++], A, (n + 1) * sizeof(int));
    seen[rankPerm(A)] = 1;
    memcpy(best, A, (n + 1) * sizeof(int));

    while (head < tail) {
        int cur[9];
        memcpy(cur, q[head++], (n + 1) * sizeof(int));
        int smaller = 0;
        for (int i = 1; i <= n; i++) {
            if (cur[i] != best[i]) { smaller = cur[i] < best[i]; break; }
        }
        if (smaller) memcpy(best, cur, (n + 1) * sizeof(int));

        for (int e = 0; e < m; e++) {
            int nx[9];
            memcpy(nx, cur, (n + 1) * sizeof(int));
            int t = nx[su[e]]; nx[su[e]] = nx[sv[e]]; nx[sv[e]] = t;
            int r = rankPerm(nx);
            if (!seen[r]) {
                seen[r] = 1;
                memcpy(q[tail++], nx, (n + 1) * sizeof(int));
            }
        }
    }
    free(q);
    free(seen);
}

int main(void) {
    printf("Enter n: ");
    if (scanf("%d", &n) != 1 || n < 1) { printf("Invalid input\n"); return 1; }
    int *A = malloc((n + 1) * sizeof(int));
    printf("Enter the permutation of 1..%d: ", n);
    for (int i = 1; i <= n; i++) scanf("%d", &A[i]);
    printf("Enter number of allowed swaps m: ");
    if (scanf("%d", &m) != 1 || m < 0) { printf("Invalid input\n"); return 1; }

    su = malloc((m + 1) * sizeof(int));
    sv = malloc((m + 1) * sizeof(int));
    parent = malloc((n + 1) * sizeof(int));
    rnk = calloc(n + 1, sizeof(int));
    for (int i = 1; i <= n; i++) parent[i] = i;
    printf("Enter %d swaps (positions u v, 1-based):\n", m);
    for (int e = 0; e < m; e++) {
        scanf("%d %d", &su[e], &sv[e]);
        unite(su[e], sv[e]);
    }

    /* group positions by root (counting sort keeps positions increasing) */
    int *cnt = calloc(n + 2, sizeof(int)), *order = malloc(n * sizeof(int));
    for (int i = 1; i <= n; i++) cnt[find(i) + 1]++;
    for (int r = 1; r <= n + 1; r++) cnt[r] += cnt[r - 1];
    int *fill = malloc((n + 2) * sizeof(int));
    memcpy(fill, cnt, (n + 2) * sizeof(int));
    for (int i = 1; i <= n; i++) order[fill[find(i)]++] = i;

    int *result = malloc((n + 1) * sizeof(int)), *vals = malloc(n * sizeof(int));
    printf("\nComponents (positions : values):\n");
    for (int r = 1; r <= n; r++) {
        int lo = cnt[r], hi = cnt[r + 1];
        if (lo == hi) continue;
        int k = hi - lo;
        for (int t = 0; t < k; t++) vals[t] = A[order[lo + t]];
        qsort(vals, k, sizeof(int), cmpInt);
        printf("  {");
        for (int t = 0; t < k; t++) printf("%d%s", order[lo + t], t + 1 < k ? "," : "");
        printf("} : ");
        for (int t = 0; t < k; t++) {
            result[order[lo + t]] = vals[t];
            printf("%d ", vals[t]);
        }
        printf("\n");
    }

    printf("\nOriginal     : ");
    for (int i = 1; i <= n; i++) printf("%d ", A[i]);
    printf("\nLex smallest : ");
    for (int i = 1; i <= n; i++) printf("%d ", result[i]);
    printf("\n");

    if (n <= 8) {
        int best[9];
        bruteForce(A, best);
        int same = 1;
        for (int i = 1; i <= n; i++) if (best[i] != result[i]) same = 0;
        printf("Brute-force BFS check: %s\n", same ? "MATCH" : "MISMATCH");
    }

    free(A); free(su); free(sv); free(parent); free(rnk);
    free(cnt); free(order); free(fill); free(result); free(vals);
    return 0;
}
