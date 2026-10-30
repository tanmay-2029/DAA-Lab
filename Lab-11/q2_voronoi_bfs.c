/*
 * DAA Lab-11, Q2: Multi-source BFS for radius-constrained Voronoi cells
 *
 * Idea:
 *   Put ALL hubs in the queue at distance 0 and run one BFS. Every vertex gets
 *   the distance to its nearest hub. For the owner:
 *     - first time a vertex is discovered it copies the owner of the parent;
 *     - if another parent on the SAME level also reaches it later, we keep the
 *       smaller hub index.
 *   This is safe because BFS finishes a whole level before it starts popping
 *   the next one, so by the time a vertex is popped, all of its parents (one
 *   level up) have already been processed and its owner is final.
 *   We stop expanding at depth R, anything not reached has owner -1.
 *
 * Input format:
 *   n m K R
 *   h_1 h_2 ... h_K        (hub vertices, hub i has index i)
 *   u v                    (m lines, undirected edges, vertices 1..n)
 *
 * Output: n lines  ->  "vertex owner dist"   (owner = -1, dist = -1 if outside)
 */
#include <stdio.h>
#include <stdlib.h>

int main()
{
    int n, m, K, R;

    printf("Enter number of vertices, edges, hubs and radius: ");
    scanf("%d %d %d %d", &n, &m, &K, &R);

    if (n <= 0 || m < 0 || K < 0 || R < 0) {
        printf("Invalid input\n");
        return 1;
    }

    int *hubs = malloc(sizeof(int) * (K > 0 ? K : 1));

    printf("Enter %d hub vertices:\n", K);
    for (int i = 0; i < K; i++) {
        scanf("%d", &hubs[i]);

        if (hubs[i] < 1 || hubs[i] > n) {
            printf("Invalid hub\n");
            return 1;
        }
    }

    int *eu = malloc(sizeof(int) * (m > 0 ? m : 1));
    int *ev = malloc(sizeof(int) * (m > 0 ? m : 1));

    int *start = calloc(n + 2, sizeof(int));

    printf("Enter %d edges (u v):\n", m);
    for (int i = 0; i < m; i++) {
        scanf("%d %d", &eu[i], &ev[i]);

        if (eu[i] < 1 || eu[i] > n || ev[i] < 1 || ev[i] > n) {
            printf("Invalid edge\n");
            return 1;
        }

        start[eu[i] + 1]++;
        start[ev[i] + 1]++;
    }

    for (int i = 1; i <= n + 1; i++)
        start[i] += start[i - 1];

    int *adj = malloc(sizeof(int) * (2 * m > 0 ? 2 * m : 1));
    int *fill = malloc(sizeof(int) * (n + 2));

    for (int i = 0; i <= n + 1; i++)
        fill[i] = start[i];

    for (int i = 0; i < m; i++) {
        adj[fill[eu[i]]++] = ev[i];
        adj[fill[ev[i]]++] = eu[i];
    }

    int *dist = malloc(sizeof(int) * (n + 1));
    int *owner = malloc(sizeof(int) * (n + 1));
    int *queue = malloc(sizeof(int) * n);

    for (int v = 0; v <= n; v++) {
        dist[v] = -1;
        owner[v] = -1;
    }

    int qh = 0, qt = 0;

    for (int i = 0; i < K; i++) {
        int h = hubs[i];

        if (dist[h] == -1) {
            dist[h] = 0;
            owner[h] = i + 1;
            queue[qt++] = h;
        }
    }

    while (qh < qt) {
        int u = queue[qh++];

        if (dist[u] == R)
            continue;

        for (int e = start[u]; e < start[u + 1]; e++) {
            int v = adj[e];

            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                owner[v] = owner[u];
                queue[qt++] = v;
            }
            else if (dist[v] == dist[u] + 1 && owner[u] < owner[v]) {
                owner[v] = owner[u];
            }
        }
    }

    printf("\nVertex  Owner  Distance\n");

    for (int v = 1; v <= n; v++)
        printf("%d        %d      %d\n", v, owner[v], dist[v]);

    free(hubs);
    free(eu);
    free(ev);
    free(start);
    free(adj);
    free(fill);
    free(dist);
    free(owner);
    free(queue);

    return 0;
}