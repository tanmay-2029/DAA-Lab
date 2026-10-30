/*
 * DAA Lab-11, Q1: Shortest path on parity-constrained graph (0-1 BFS)
 *
 * Idea (see docs/analysis.md for the full write-up):
 *   Since every weight is 0 or 1, W(P) == k1(P). So the cost of a path only
 *   depends on how many weight-1 edges it uses:
 *        k1 even -> cost = k1
 *        k1 odd  -> cost = k1 + C
 *   For a fixed parity, a smaller k1 is always better, so we only need the
 *   minimum k1 for each (vertex, parity) pair. That is a shortest path
 *   problem on a graph with 2*V states, where
 *        weight-0 edge: (u,p) -> (v,p)     cost 0
 *        weight-1 edge: (u,p) -> (v,1-p)   cost 1
 *   -> run 0-1 BFS (deque) from (S,0).
 *   Answer(v) = min( d[v][0], d[v][1] + C ).
 *
 * Assumption: a "path" may repeat vertices (a walk). This matters because
 * sometimes going around a cycle is the only way to fix the parity.
 *
 * Input format:
 *   n m S C
 *   u v w      (m lines, vertices are 1..n, w is 0 or 1, edge is u -> v)
 *
 * Output: one line per vertex  ->  "v: cost"  or  "v: UNREACHABLE"
 */
#include <stdio.h>
#include <stdlib.h>

#define INF 4000000000000000000LL

int main()
{
    int n, m, S;
    long long C;

    printf("Enter number of vertices, edges, source and C: ");
    scanf("%d %d %d %lld", &n, &m, &S, &C);

    if (n <= 0 || m < 0 || S < 1 || S > n || C < 0) {
        printf("Invalid input\n");
        return 1;
    }

    int *eu = malloc(sizeof(int) * (m > 0 ? m : 1));
    int *ev = malloc(sizeof(int) * (m > 0 ? m : 1));
    int *ew = malloc(sizeof(int) * (m > 0 ? m : 1));
    int *start = calloc(n + 2, sizeof(int));

    printf("Enter %d edges (u v w):\n", m);

    for (int i = 0; i < m; i++) {
        scanf("%d %d %d", &eu[i], &ev[i], &ew[i]);

        if (eu[i] < 1 || eu[i] > n || ev[i] < 1 || ev[i] > n ||
            (ew[i] != 0 && ew[i] != 1)) {
            printf("Invalid edge\n");
            return 1;
        }

        start[eu[i] + 1]++;
    }

    for (int i = 1; i <= n + 1; i++)
        start[i] += start[i - 1];

    int *to = malloc(sizeof(int) * (m > 0 ? m : 1));
    int *wt = malloc(sizeof(int) * (m > 0 ? m : 1));
    int *fill = malloc(sizeof(int) * (n + 2));

    for (int i = 0; i <= n + 1; i++)
        fill[i] = start[i];

    for (int i = 0; i < m; i++) {
        int pos = fill[eu[i]]++;
        to[pos] = ev[i];
        wt[pos] = ew[i];
    }

    int states = 2 * (n + 1);

    long long *d = malloc(sizeof(long long) * states);
    char *done = calloc(states, 1);

    for (int i = 0; i < states; i++)
        d[i] = INF;

    int cap = 2 * (2 * m + 2) + 2;
    int *dq = malloc(sizeof(int) * cap);

    int head = cap / 2;
    int tail = cap / 2;

    d[S * 2] = 0;
    dq[tail++] = S * 2;

    while (head < tail) {
        int cur = dq[head++];

        if (done[cur])
            continue;

        done[cur] = 1;

        int u = cur / 2;
        int p = cur % 2;

        for (int e = start[u]; e < start[u + 1]; e++) {
            int v = to[e];
            int w = wt[e];

            int np = p ^ w;
            int nxt = v * 2 + np;

            if (d[cur] + w < d[nxt]) {
                d[nxt] = d[cur] + w;

                if (w == 0)
                    dq[--head] = nxt;
                else
                    dq[tail++] = nxt;
            }
        }
    }

    printf("\nShortest path costs:\n");

    for (int v = 1; v <= n; v++) {
        long long even = d[v * 2];
        long long odd = d[v * 2 + 1];
        long long best = INF;

        if (even < INF)
            best = even;

        if (odd < INF && odd + C < best)
            best = odd + C;

        if (best >= INF)
            printf("%d: UNREACHABLE\n", v);
        else
            printf("%d: %lld\n", v, best);
    }

    free(eu);
    free(ev);
    free(ew);
    free(start);
    free(to);
    free(wt);
    free(fill);
    free(d);
    free(done);
    free(dq);

    return 0;
}