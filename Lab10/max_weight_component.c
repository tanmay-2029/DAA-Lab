/*
 * Q3: Maximum Weight Component with a Size Constraint
 * ---------------------------------------------------
 * Input : V, K, the vertex weights w_0 .. w_{V-1}, then E and the edges
 *         "u v weight" in the order they are added.
 * Query : after each edge, the maximum total vertex weight of a connected
 *         component having at most K vertices.
 * (The edge weight does not change component sums; it is only reported.
 *  If the edges are first sorted by weight this becomes the Kruskal-style
 *  "using only edges up to weight t" version.)
 *
 * Representation:
 *   DSU with size[root] and sum[root] (union by size, path compression).
 *   A max-heap of candidate components (sum, root, version) with LAZY
 *   deletion: whenever a component changes, its version is increased and a
 *   new entry is pushed. When querying, entries are discarded from the top
 *   while they are outdated (root no longer a root, version changed) or the
 *   component is now larger than K. Components only grow, so a component
 *   that exceeds K never becomes valid again - discarding is permanent.
 *
 * Validation: after every edge the answer is recomputed with a BFS over all
 * components of the edges added so far.
 *
 * Complexity:
 *   Each union        : O(alpha(V)) + one heap push O(log(V + E))
 *   Each query        : amortised O(log(V + E)) (each entry popped once)
 *   Total             : O((V + E) log(V + E)) time, O(V + E) space
 *   Validation        : O(E (V + E))
 */
#include <stdio.h>
#include <stdlib.h>

typedef struct { long long sum; int root, version; } Entry;

static int *parent, *size, *version;
static long long *sum;
static Entry *heap;
static int heapSize = 0;

static void push(Entry e) {
    int i = heapSize++;
    heap[i] = e;
    while (i > 0 && heap[(i - 1) / 2].sum < heap[i].sum) {
        Entry t = heap[i]; heap[i] = heap[(i - 1) / 2]; heap[(i - 1) / 2] = t;
        i = (i - 1) / 2;
    }
}

static void popTop(void) {
    heap[0] = heap[--heapSize];
    int i = 0;
    for (;;) {
        int l = 2 * i + 1, r = l + 1, m = i;
        if (l < heapSize && heap[l].sum > heap[m].sum) m = l;
        if (r < heapSize && heap[r].sum > heap[m].sum) m = r;
        if (m == i) break;
        Entry t = heap[i]; heap[i] = heap[m]; heap[m] = t; i = m;
    }
}

static int find(int x) {
    while (parent[x] != x) { parent[x] = parent[parent[x]]; x = parent[x]; }
    return x;
}

static void unite(int a, int b, int K) {
    a = find(a); b = find(b);
    if (a == b) return;
    if (size[a] < size[b]) { int t = a; a = b; b = t; }
    parent[b] = a;
    size[a] += size[b];
    sum[a] += sum[b];
    version[a]++;
    version[b]++;
    if (size[a] <= K) push((Entry){sum[a], a, version[a]});
}

/* Returns 1 and sets *best if some component has size <= K. */
static int query(int K, long long *best) {
    while (heapSize > 0) {
        Entry e = heap[0];
        if (parent[e.root] == e.root && version[e.root] == e.version && size[e.root] <= K) {
            *best = e.sum;
            return 1;
        }
        popTop();
    }
    return 0;
}

/* ---- brute force with BFS ---- */
static int bruteForce(int V, int K, const long long *w, int m, const int *eu, const int *ev, long long *best) {
    int *deg = calloc(V + 1, sizeof(int)), *start = calloc(V + 1, sizeof(int));
    int *pos = malloc(V * sizeof(int)), *adj = malloc((2 * m + 1) * sizeof(int));
    for (int i = 0; i < m; i++) { deg[eu[i]]++; deg[ev[i]]++; }
    for (int i = 0; i < V; i++) start[i + 1] = start[i] + deg[i];
    for (int i = 0; i < V; i++) pos[i] = start[i];
    for (int i = 0; i < m; i++) { adj[pos[eu[i]]++] = ev[i]; adj[pos[ev[i]]++] = eu[i]; }

    char *seen = calloc(V, 1);
    int *queue = malloc(V * sizeof(int)), found = 0;
    for (int s = 0; s < V; s++) {
        if (seen[s]) continue;
        int h = 0, t = 0;
        long long total = 0;
        seen[s] = 1;
        queue[t++] = s;
        while (h < t) {
            int x = queue[h++];
            total += w[x];
            for (int k = start[x]; k < start[x + 1]; k++)
                if (!seen[adj[k]]) { seen[adj[k]] = 1; queue[t++] = adj[k]; }
        }
        if (t <= K && (!found || total > *best)) { *best = total; found = 1; }
    }
    free(deg); free(start); free(pos); free(adj); free(seen); free(queue);
    return found;
}

int main(void) {
    int V, K, E;
    printf("Enter number of vertices V and size limit K: ");
    if (scanf("%d %d", &V, &K) != 2 || V < 1) { printf("Invalid input\n"); return 1; }

    long long *w = malloc(V * sizeof(long long));
    printf("Enter %d vertex weights: ", V);
    for (int i = 0; i < V; i++) scanf("%lld", &w[i]);
    printf("Enter number of edges E: ");
    if (scanf("%d", &E) != 1 || E < 0) { printf("Invalid input\n"); return 1; }

    parent = malloc(V * sizeof(int));
    size = malloc(V * sizeof(int));
    version = calloc(V, sizeof(int));
    sum = malloc(V * sizeof(long long));
    heap = malloc((V + E + 1) * sizeof(Entry));
    for (int i = 0; i < V; i++) {
        parent[i] = i;
        size[i] = 1;
        sum[i] = w[i];
        if (K >= 1) push((Entry){sum[i], i, 0});
    }

    int *eu = malloc((E + 1) * sizeof(int)), *ev = malloc((E + 1) * sizeof(int));
    int mismatches = 0;
    long long best = 0, bf = 0;

    int ok = query(K, &best);
    printf("\nInitially: answer = ");
    if (ok) printf("%lld\n", best); else printf("none\n");

    printf("Enter %d edges (u v weight), vertices 0..%d:\n", E, V - 1);
    for (int i = 0; i < E; i++) {
        int u, v;
        long long ew;
        scanf("%d %d %lld", &u, &v, &ew);
        eu[i] = u;
        ev[i] = v;
        unite(u, v, K);

        ok = query(K, &best);
        int okB = bruteForce(V, K, w, i + 1, eu, ev, &bf);
        if (ok != okB || (ok && best != bf)) mismatches++;

        printf("  edge %2d (%d, %d, w=%lld): max weight of a component with size <= %d = ", i + 1, u, v, ew, K);
        if (ok) printf("%lld\n", best); else printf("none\n");
    }
    printf("\nBFS validation: %s\n", mismatches == 0 ? "all answers MATCH" : "MISMATCH found");

    free(w); free(parent); free(size); free(version); free(sum); free(heap); free(eu); free(ev);
    return 0;
}
