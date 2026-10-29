/*
 * Q9: Hu-Tucker algorithm (optimal alphabetic binary tree)
 * --------------------------------------------------------
 * Input : n and the weights w1 .. wn (in their fixed left-to-right order).
 * Goal  : a binary tree whose leaves, read left to right, are w1 .. wn, that
 *         minimises  sum w_i * depth(i).
 *
 * Phase 1 - Combination (greedy):
 *   Keep a working sequence of nodes. Original leaves are "square" nodes,
 *   combined nodes are "circle" nodes. Two nodes are COMPATIBLE if no square
 *   node lies strictly between them (circles are transparent).
 *   Repeat n-1 times: among all compatible pairs (i < j) choose the one with
 *   the smallest w_i + w_j (ties: smallest i, then smallest j); replace node i
 *   by a circle of weight w_i + w_j and delete node j.
 *   This tree is NOT alphabetic, but its leaf depths l_i are the optimal
 *   levels.
 *
 * Phase 2 - Levels: l_i = depth of leaf i in the phase-1 tree.
 *
 * Phase 3 - Recombination:
 *   Start from the sequence of leaves with their levels. Repeatedly take the
 *   maximum level L and join the leftmost adjacent pair that both have level
 *   L into one node of level L-1. The result is an alphabetic tree in which
 *   every leaf i has depth l_i, so its cost is optimal.
 *
 * Validation: O(n^3) DP for the optimal alphabetic tree,
 *   C[i][i] = 0,  C[i][j] = min_{i<=k<j} ( C[i][k] + C[k+1][j] ) + W(i..j)
 *
 * Complexity (this straightforward implementation):
 *   Phase 1 : n-1 rounds, each scans O(n^2) pairs -> O(n^3)
 *             (O(n^2) with a smarter scan; O(n log n) with mergeable heaps)
 *   Phase 2 : O(n)
 *   Phase 3 : O(n^2)
 *   DP check: O(n^3) time, O(n^2) space
 */
#include <stdio.h>
#include <stdlib.h>

#define MAXN 200

typedef struct {
    long long w;
    int left, right;   /* children, -1 for a leaf */
    int leaf;          /* leaf index (0-based) or -1 */
} Node;

static Node t1[2 * MAXN], t2[2 * MAXN];   /* phase-1 tree and final tree */
static int level[MAXN];
static long long w[MAXN];
static int n;

static void leafDepths(int v, int d) {
    if (t1[v].left == -1) { level[t1[v].leaf] = d; return; }
    leafDepths(t1[v].left, d + 1);
    leafDepths(t1[v].right, d + 1);
}

static void printTree(int v) {
    if (t2[v].left == -1) { printf("w%d", t2[v].leaf + 1); return; }
    printf("(");
    printTree(t2[v].left);
    printf(" ");
    printTree(t2[v].right);
    printf(")");
}

static void finalDepths(int v, int d, int *depth) {
    if (t2[v].left == -1) { depth[t2[v].leaf] = d; return; }
    finalDepths(t2[v].left, d + 1, depth);
    finalDepths(t2[v].right, d + 1, depth);
}

static long long dpOptimal(void) {
    long long **C = malloc(n * sizeof(long long *));
    long long *pre = malloc((n + 1) * sizeof(long long));
    pre[0] = 0;
    for (int i = 0; i < n; i++) { C[i] = calloc(n, sizeof(long long)); pre[i + 1] = pre[i] + w[i]; }
    for (int len = 2; len <= n; len++)
        for (int i = 0; i + len - 1 < n; i++) {
            int j = i + len - 1;
            C[i][j] = -1;
            for (int k = i; k < j; k++) {
                long long c = C[i][k] + C[k + 1][j];
                if (C[i][j] < 0 || c < C[i][j]) C[i][j] = c;
            }
            C[i][j] += pre[j + 1] - pre[i];
        }
    long long ans = C[0][n - 1];
    for (int i = 0; i < n; i++) free(C[i]);
    free(C);
    free(pre);
    return ans;
}

int main(void) {
    printf("Enter number of weights n (1..%d): ", MAXN);
    if (scanf("%d", &n) != 1 || n < 1 || n > MAXN) { printf("Invalid input\n"); return 1; }
    printf("Enter %d weights in order: ", n);
    for (int i = 0; i < n; i++) scanf("%lld", &w[i]);

    if (n == 1) {
        printf("\nSingle leaf: tree = w1, cost = 0\n");
        return 0;
    }

    /* ---------- Phase 1: combination ---------- */
    int seq[MAXN], square[MAXN], len = n, cnt = n;
    for (int i = 0; i < n; i++) {
        t1[i] = (Node){w[i], -1, -1, i};
        seq[i] = i;
        square[i] = 1;
    }
    printf("\nPhase 1 (combination):\n");
    while (len > 1) {
        int bi = -1, bj = -1;
        long long best = 0;
        for (int i = 0; i < len; i++) {
            for (int j = i + 1; j < len; j++) {
                long long s = t1[seq[i]].w + t1[seq[j]].w;
                if (bi == -1 || s < best) { best = s; bi = i; bj = j; }
                if (square[j]) break;   /* cannot look past a square node */
            }
        }
        t1[cnt] = (Node){best, seq[bi], seq[bj], -1};
        printf("  combine weights %lld + %lld = %lld\n", t1[seq[bi]].w, t1[seq[bj]].w, best);
        seq[bi] = cnt++;
        square[bi] = 0;
        for (int k = bj; k < len - 1; k++) { seq[k] = seq[k + 1]; square[k] = square[k + 1]; }
        len--;
    }

    /* ---------- Phase 2: levels ---------- */
    leafDepths(seq[0], 0);
    printf("\nPhase 2 (levels):\n  ");
    for (int i = 0; i < n; i++) printf("w%d=%lld:L%d  ", i + 1, w[i], level[i]);
    printf("\n");

    /* ---------- Phase 3: recombination ---------- */
    int id[MAXN], lv[MAXN], m = n, total = n;
    for (int i = 0; i < n; i++) {
        t2[i] = (Node){w[i], -1, -1, i};
        id[i] = i;
        lv[i] = level[i];
    }
    while (m > 1) {
        int L = 0, pos = -1;
        for (int i = 0; i < m; i++) if (lv[i] > L) L = lv[i];
        for (int i = 0; i + 1 < m; i++) if (lv[i] == L && lv[i + 1] == L) { pos = i; break; }
        if (pos == -1) { printf("Recombination failed\n"); return 1; }
        t2[total] = (Node){t2[id[pos]].w + t2[id[pos + 1]].w, id[pos], id[pos + 1], -1};
        id[pos] = total++;
        lv[pos] = L - 1;
        for (int k = pos + 1; k < m - 1; k++) { id[k] = id[k + 1]; lv[k] = lv[k + 1]; }
        m--;
    }

    int depth[MAXN];
    finalDepths(id[0], 0, depth);
    long long cost = 0;
    int same = 1;
    for (int i = 0; i < n; i++) {
        cost += w[i] * depth[i];
        if (depth[i] != level[i]) same = 0;
    }

    printf("\nPhase 3 (optimal alphabetic tree, leaves in order):\n  ");
    printTree(id[0]);
    printf("\n\nDepths match phase-2 levels : %s\n", same ? "YES" : "NO");
    printf("Minimum cost sum w_i*depth_i : %lld\n", cost);

    long long dp = dpOptimal();
    printf("O(n^3) DP check              : %lld  (%s)\n", dp, dp == cost ? "MATCH" : "MISMATCH");
    return 0;
}
