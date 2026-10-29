/*
 * Q10: Shortest Common Superstring - the Greedy Superstring Conjecture
 * --------------------------------------------------------------------
 * Problem: given strings s1..sn, find the shortest string containing every
 * si as a substring. The problem is NP-hard.
 *
 * Preprocessing: drop duplicates and strings that are substrings of others
 * (they are covered automatically).
 *
 * Greedy algorithm:
 *   ov(a, b) = longest suffix of a that is a prefix of b.
 *   While more than one string remains, merge the pair (a, b) with the
 *   largest overlap into a + b[ov..] and replace both by the merged string.
 *
 * Exact algorithm (for checking, small n): maximise the total overlap over
 * all orders with a DP over subsets (like TSP):
 *   best[mask][last] = max overlap of an order of the strings in mask that
 *                      ends with string last
 *   |optimal| = sum |si| - max over last of best[full][last]
 *
 * Conjecture: greedy length <= 2 * optimal length. The lab sheet notes a
 * September 2026 arXiv paper claiming a disproof (ratio at least
 * (9k+2)/(4k+4) -> 2.25 for even string length k >= 10); that claim was not
 * yet validated.
 *
 * Demo: the family { c(ab)^k, (ba)^k, (ab)^k c } shows greedy can be close
 * to 2 times worse: greedy first merges c(ab)^k and (ab)^k c (overlap 2k)
 * and then finds no overlap with (ba)^k, giving length 4k+2, while the best
 * order c(ab)^k -> (ba)^k -> (ab)^k c has length 2k+4. Ratio -> 2.
 *
 * Complexity (n strings, total length N, max length m):
 *   Overlap table : O(n^2 * m^2) naive (O(n^2 m) with KMP)
 *   Greedy        : n-1 merges, each recomputes overlaps -> O(n^3 * m^2) naive
 *   Exact DP      : O(2^n * n^2) time, O(2^n * n) space (n <= 16 here)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXS 20
#define MAXL 4096

static int overlap(const char *a, const char *b) {
    int la = strlen(a), lb = strlen(b);
    int maxk = la < lb ? la : lb;
    for (int k = maxk; k > 0; k--)
        if (strncmp(a + la - k, b, k) == 0) return k;
    return 0;
}

/* Remove duplicates and substrings; returns new count. */
static int reduce(char s[][MAXL], int n) {
    int keep[MAXS], m = 0;
    for (int i = 0; i < n; i++) {
        keep[i] = 1;
        for (int j = 0; j < n && keep[i]; j++) {
            if (i == j) continue;
            if (strcmp(s[i], s[j]) == 0 ? j < i : strstr(s[j], s[i]) != NULL) keep[i] = 0;
        }
    }
    for (int i = 0; i < n; i++) if (keep[i]) { if (m != i) strcpy(s[m], s[i]); m++; }
    return m;
}

static void greedy(char in[][MAXL], int n, char *out) {
    static char s[MAXS][MAXL];
    for (int i = 0; i < n; i++) strcpy(s[i], in[i]);
    n = reduce(s, n);
    while (n > 1) {
        int bi = 0, bj = 1, best = -1;
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++) {
                if (i == j) continue;
                int o = overlap(s[i], s[j]);
                if (o > best) { best = o; bi = i; bj = j; }
            }
        strcat(s[bi], s[bj] + best);
        if (bj != n - 1) strcpy(s[bj], s[n - 1]);
        n--;
        n = reduce(s, n);
    }
    strcpy(out, s[0]);
}

static void exact(char in[][MAXL], int n, char *out) {
    static char s[MAXS][MAXL];
    for (int i = 0; i < n; i++) strcpy(s[i], in[i]);
    n = reduce(s, n);

    int ov[MAXS][MAXS];
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) ov[i][j] = (i == j) ? 0 : overlap(s[i], s[j]);

    int full = (1 << n) - 1;
    int *best = malloc((size_t)(full + 1) * n * sizeof(int));
    int *par = malloc((size_t)(full + 1) * n * sizeof(int));
    for (int i = 0; i < (full + 1) * n; i++) best[i] = -1;
    for (int i = 0; i < n; i++) { best[(1 << i) * n + i] = 0; par[(1 << i) * n + i] = -1; }

    for (int mask = 1; mask <= full; mask++)
        for (int last = 0; last < n; last++) {
            int cur = best[mask * n + last];
            if (cur < 0) continue;
            for (int nx = 0; nx < n; nx++) {
                if (mask & (1 << nx)) continue;
                int nm = mask | (1 << nx), v = cur + ov[last][nx];
                if (v > best[nm * n + nx]) { best[nm * n + nx] = v; par[nm * n + nx] = last; }
            }
        }

    int last = 0;
    for (int i = 1; i < n; i++) if (best[full * n + i] > best[full * n + last]) last = i;

    int order[MAXS], k = n, mask = full;
    while (last != -1) {
        order[--k] = last;
        int p = par[mask * n + last];
        mask ^= 1 << last;
        last = p;
    }
    strcpy(out, s[order[0]]);
    for (int i = 1; i < n; i++) strcat(out, s[order[i]] + ov[order[i - 1]][order[i]]);

    free(best);
    free(par);
}

static int containsAll(const char *sup, char s[][MAXL], int n) {
    for (int i = 0; i < n; i++) if (!strstr(sup, s[i])) return 0;
    return 1;
}

int main(void) {
    static char s[MAXS][MAXL], g[MAXS * MAXL], e[MAXS * MAXL];

    printf("Demo: family { c(ab)^k, (ba)^k, (ab)^k c }\n");
    printf("%4s %8s %8s %8s\n", "k", "greedy", "optimal", "ratio");
    for (int k = 1; k <= 10; k++) {
        strcpy(s[0], "c"); s[1][0] = '\0'; s[2][0] = '\0';
        for (int i = 0; i < k; i++) { strcat(s[0], "ab"); strcat(s[1], "ba"); strcat(s[2], "ab"); }
        strcat(s[2], "c");
        greedy(s, 3, g);
        exact(s, 3, e);
        printf("%4d %8d %8d %8.3f\n", k, (int)strlen(g), (int)strlen(e), (double)strlen(g) / strlen(e));
    }

    int n;
    printf("\nEnter number of strings (1..16): ");
    if (scanf("%d", &n) != 1 || n < 1 || n > 16) { printf("Invalid input\n"); return 1; }
    printf("Enter %d strings (no spaces):\n", n);
    for (int i = 0; i < n; i++) scanf("%200s", s[i]);

    greedy(s, n, g);
    exact(s, n, e);
    printf("\nGreedy superstring  (length %d): %s\n", (int)strlen(g), g);
    printf("Optimal superstring (length %d): %s\n", (int)strlen(e), e);
    printf("Ratio greedy/optimal = %.4f\n", (double)strlen(g) / strlen(e));
    printf("Both contain every input string: %s\n",
           containsAll(g, s, n) && containsAll(e, s, n) ? "YES" : "NO");
    return 0;
}
