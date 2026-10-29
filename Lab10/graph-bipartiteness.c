/*
 * Q1: Online Graph-Bipartiteness Testing (DSU with parity)
 * --------------------------------------------------------
 * Input : V (vertices 0..V-1), E, then E edges "u v" arriving one by one.
 *
 * Representation: Disjoint Set Union where every vertex also stores
 *   parity[x] = colour of x relative to its parent (0 = same, 1 = different).
 *   find(x) returns the root and, via path compression, the parity of x
 *   relative to the root (colour of x = parity, colour of root = 0).
 *
 * Adding edge (u, v):
 *   ru = find(u), pu = colour of u;  rv = find(v), pv = colour of v.
 *   - ru != rv : different components -> join them so u and v get different
 *                colours: parity[rv] = pu ^ pv ^ 1 (union by rank).
 *   - ru == rv and pu != pv : edge joins opposite colours -> still bipartite.
 *   - ru == rv and pu == pv : u and v already have the same colour, so the
 *                path u..v has even length and the edge closes an ODD cycle
 *                -> reject the edge.
 *
 * Validation (brute force): for every edge, BFS 2-colouring of the accepted
 * edges plus this edge must succeed exactly when the DSU accepted it.
 *
 * Complexity:
 *   Per edge  : O(alpha(V)) amortised (path compression + union by rank)
 *   Total     : O(V + E alpha(V)) time, O(V) space
 *   Validation: O(E (V + E))
 */
#include <stdio.h>
#include <stdlib.h>

static int *parent, *rnk, *parity;

/* Returns the root of x; *col receives the colour of x relative to the root. */
static int find(int x, int *col) {
    if (parent[x] == x) { *col = 0; return x; }
    int pc;
    int root = find(parent[x], &pc);
    parity[x] ^= pc;          /* parity now relative to the root */
    parent[x] = root;
    *col = parity[x];
    return root;
}

/* Returns 1 if the edge is added, 0 if it would create an odd cycle. */
static int addEdge(int u, int v) {
    int pu, pv;
    int ru = find(u, &pu), rv = find(v, &pv);
    if (ru == rv) return pu != pv;
    if (rnk[ru] < rnk[rv]) { int t = ru; ru = rv; rv = t; }
    parent[rv] = ru;
    parity[rv] = pu ^ pv ^ 1;
    if (rnk[ru] == rnk[rv]) rnk[ru]++;
    return 1;
}

/* ---- brute force: BFS 2-colouring of the first m edges in the list ---- */
static int bipartiteBFS(int V, int m, const int *eu, const int *ev) {
    int *deg = calloc(V + 1, sizeof(int)), *adj = malloc(2 * m * sizeof(int) + 1);
    int *start = calloc(V + 1, sizeof(int)), *pos = malloc((V + 1) * sizeof(int));
    for (int i = 0; i < m; i++) { deg[eu[i]]++; deg[ev[i]]++; }
    for (int i = 0; i < V; i++) start[i + 1] = start[i] + deg[i];
    for (int i = 0; i < V; i++) pos[i] = start[i];
    for (int i = 0; i < m; i++) { adj[pos[eu[i]]++] = ev[i]; adj[pos[ev[i]]++] = eu[i]; }

    int *color = malloc(V * sizeof(int)), *queue = malloc(V * sizeof(int)), ok = 1;
    for (int i = 0; i < V; i++) color[i] = -1;
    for (int s = 0; s < V && ok; s++) {
        if (color[s] != -1) continue;
        int h = 0, t = 0;
        color[s] = 0;
        queue[t++] = s;
        while (h < t && ok) {
            int x = queue[h++];
            for (int k = start[x]; k < start[x + 1]; k++) {
                int y = adj[k];
                if (color[y] == -1) { color[y] = color[x] ^ 1; queue[t++] = y; }
                else if (color[y] == color[x]) { ok = 0; break; }
            }
        }
    }
    free(deg); free(adj); free(start); free(pos); free(color); free(queue);
    return ok;
}

int main(void) {
    int V, E;
    printf("Enter number of vertices V and edges E: ");
    if (scanf("%d %d", &V, &E) != 2 || V < 1 || E < 0) { printf("Invalid input\n"); return 1; }

    parent = malloc(V * sizeof(int));
    rnk = calloc(V, sizeof(int));
    parity = calloc(V, sizeof(int));
    for (int i = 0; i < V; i++) parent[i] = i;

    int *eu = malloc((E + 1) * sizeof(int)), *ev = malloc((E + 1) * sizeof(int));
    int accepted = 0, mismatches = 0;

    printf("Enter %d edges (u v), vertices 0..%d:\n", E, V - 1);
    for (int i = 0; i < E; i++) {
        int u, v;
        scanf("%d %d", &u, &v);
        int ok = addEdge(u, v);
        printf("  edge %2d (%d, %d): %s\n", i + 1, u, v,
               ok ? "added (graph still bipartite)" : "REJECTED - creates an odd cycle");

        /* validation: accepted edges so far + this edge */
        eu[accepted] = u;
        ev[accepted] = v;
        int bfs = bipartiteBFS(V, accepted + 1, eu, ev);
        if (bfs != ok) mismatches++;
        if (ok) accepted++;
    }

    printf("\nEdges accepted = %d, rejected = %d\n", accepted, E - accepted);

    int pc;
    printf("Final 2-colouring: ");
    for (int i = 0; i < V; i++) { find(i, &pc); printf("%d:%c ", i, pc ? 'B' : 'A'); }
    printf("\nBFS validation: %s\n", mismatches == 0 ? "all decisions MATCH" : "MISMATCH found");

    free(parent); free(rnk); free(parity); free(eu); free(ev);
    return 0;
}
