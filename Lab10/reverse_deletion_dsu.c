/*
 * Q4: Reverse Time-Step DSU (vertex deletions)
 * --------------------------------------------
 * Input : V, the non-negative vertex values val_0 .. val_{V-1}, E and the
 *         edges "u v", then the deletion order p_1 .. p_V (a permutation).
 * Output: after each deletion, the maximum sum of values over the remaining
 *         connected components (0 when no vertex remains).
 *
 * Idea: DSU cannot split components, but it can merge them. So process time
 * BACKWARDS: start with the empty graph and ADD the vertices in the order
 * p_V, p_{V-1}, ..., p_1.
 *   ans[V] = 0 (everything deleted)
 *   for i = V down to 1:
 *       ans[i] = best      (state after deleting p_1..p_i = vertices p_{i+1}..p_V)
 *       activate p_i; for every neighbour already active, union them
 *       best = max(best, sum of p_i's component)
 *   (ans[i] is recorded before p_i is added back.)
 * With non-negative values a merged component's sum is at least the sum of
 * each part, so a running maximum is the maximum over current components.
 *
 * Validation: for every step, BFS over the remaining vertices from scratch.
 *
 * Complexity:
 *   Building adjacency : O(V + E)
 *   Reverse process    : each edge examined twice, O((V + E) alpha(V))
 *   Total              : O(V + E alpha(V)) time, O(V + E) space
 *   Validation         : O(V (V + E))
 */
#include <stdio.h>
#include <stdlib.h>

static int *parent, *rnk;
static long long *sum;

static int find(int x) {
    while (parent[x] != x) { parent[x] = parent[parent[x]]; x = parent[x]; }
    return x;
}

static void unite(int a, int b) {
    a = find(a); b = find(b);
    if (a == b) return;
    if (rnk[a] < rnk[b]) { int t = a; a = b; b = t; }
    parent[b] = a;
    sum[a] += sum[b];
    if (rnk[a] == rnk[b]) rnk[a]++;
}

int main(void) {
    int V, E;
    printf("Enter number of vertices V: ");
    if (scanf("%d", &V) != 1 || V < 1) { printf("Invalid input\n"); return 1; }
    long long *val = malloc(V * sizeof(long long));
    printf("Enter %d non-negative vertex values: ", V);
    for (int i = 0; i < V; i++) {
        scanf("%lld", &val[i]);
        if (val[i] < 0) { printf("Values must be non-negative\n"); return 1; }
    }
    printf("Enter number of edges E: ");
    if (scanf("%d", &E) != 1 || E < 0) { printf("Invalid input\n"); return 1; }

    int *eu = malloc((E + 1) * sizeof(int)), *ev = malloc((E + 1) * sizeof(int));
    int *deg = calloc(V + 1, sizeof(int)), *start = calloc(V + 1, sizeof(int));
    printf("Enter %d edges (u v), vertices 0..%d:\n", E, V - 1);
    for (int i = 0; i < E; i++) {
        scanf("%d %d", &eu[i], &ev[i]);
        deg[eu[i]]++;
        deg[ev[i]]++;
    }
    for (int i = 0; i < V; i++) start[i + 1] = start[i] + deg[i];
    int *pos = malloc(V * sizeof(int)), *adj = malloc((2 * E + 1) * sizeof(int));
    for (int i = 0; i < V; i++) pos[i] = start[i];
    for (int i = 0; i < E; i++) { adj[pos[eu[i]]++] = ev[i]; adj[pos[ev[i]]++] = eu[i]; }

    int *P = malloc((V + 1) * sizeof(int));
    printf("Enter the deletion order (%d vertices): ", V);
    for (int i = 1; i <= V; i++) scanf("%d", &P[i]);

    /* ---- reverse process ---- */
    parent = malloc(V * sizeof(int));
    rnk = calloc(V, sizeof(int));
    sum = malloc(V * sizeof(long long));
    char *active = calloc(V, 1);
    long long *ans = malloc((V + 1) * sizeof(long long)), best = 0;

    for (int i = V; i >= 1; i--) {
        ans[i] = best;
        int x = P[i];
        active[x] = 1;
        parent[x] = x;
        sum[x] = val[x];
        for (int k = start[x]; k < start[x + 1]; k++)
            if (active[adj[k]]) unite(x, adj[k]);
        long long s = sum[find(x)];
        if (s > best) best = s;
    }

    /* ---- validation: BFS on the remaining graph after each deletion ---- */
    char *alive = malloc(V), *seen = malloc(V);
    int *queue = malloc(V * sizeof(int)), mismatches = 0;
    for (int i = 0; i < V; i++) alive[i] = 1;

    printf("\n%6s %10s %12s %10s\n", "step", "deleted", "max sum", "BFS");
    for (int i = 1; i <= V; i++) {
        alive[P[i]] = 0;
        long long bf = 0;
        for (int v = 0; v < V; v++) seen[v] = 0;
        for (int s = 0; s < V; s++) {
            if (!alive[s] || seen[s]) continue;
            int h = 0, t = 0;
            long long total = 0;
            seen[s] = 1;
            queue[t++] = s;
            while (h < t) {
                int x = queue[h++];
                total += val[x];
                for (int k = start[x]; k < start[x + 1]; k++) {
                    int y = adj[k];
                    if (alive[y] && !seen[y]) { seen[y] = 1; queue[t++] = y; }
                }
            }
            if (total > bf) bf = total;
        }
        if (bf != ans[i]) mismatches++;
        printf("%6d %10d %12lld %10lld\n", i, P[i], ans[i], bf);
    }
    printf("\nBFS validation: %s\n", mismatches == 0 ? "all answers MATCH" : "MISMATCH found");

    free(val); free(eu); free(ev); free(deg); free(start); free(pos); free(adj); free(P);
    free(parent); free(rnk); free(sum); free(active); free(ans); free(alive); free(seen); free(queue);
    return 0;
}
