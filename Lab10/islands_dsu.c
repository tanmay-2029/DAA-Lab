/*
 * Q2: Dynamic Connected Components in a Grid (Number of Islands II)
 * -----------------------------------------------------------------
 * Input : R C, k, then k positions "r c" (0-based) turned from water to land.
 *
 * Representation: coordinate flattening, cell (r, c) -> id = r*C + c, so the
 * grid becomes a DSU over R*C elements. land[id] marks land cells.
 *
 * Algorithm, for each new land cell x:
 *   - If x is already land, the count does not change.
 *   - Otherwise mark it land, count++ (a new island of one cell).
 *   - For each of the 4 neighbours y that is land: if find(x) != find(y),
 *     union them and count-- (two islands merged into one).
 *   - Output count.
 *
 * Validation: after every operation the islands are counted again from
 * scratch with a BFS flood fill and compared.
 *
 * Complexity:
 *   Initialisation : O(R*C)
 *   Each operation : O(alpha(R*C)) amortised (at most 4 unions)
 *   Total          : O(R*C + k alpha(R*C)) time, O(R*C) space
 *   Validation     : O(k * R*C)
 */
#include <stdio.h>
#include <stdlib.h>

static int *parent, *rnk;
static char *land;
static int R, C;

static int find(int x) {
    while (parent[x] != x) {
        parent[x] = parent[parent[x]];   /* path halving */
        x = parent[x];
    }
    return x;
}

static int unite(int a, int b) {
    a = find(a); b = find(b);
    if (a == b) return 0;
    if (rnk[a] < rnk[b]) { int t = a; a = b; b = t; }
    parent[b] = a;
    if (rnk[a] == rnk[b]) rnk[a]++;
    return 1;
}

static int floodFillCount(void) {
    int N = R * C, count = 0;
    char *seen = calloc(N, 1);
    int *queue = malloc(N * sizeof(int));
    int dr[4] = {-1, 1, 0, 0}, dc[4] = {0, 0, -1, 1};
    for (int s = 0; s < N; s++) {
        if (!land[s] || seen[s]) continue;
        count++;
        int h = 0, t = 0;
        queue[t++] = s;
        seen[s] = 1;
        while (h < t) {
            int x = queue[h++], r = x / C, c = x % C;
            for (int d = 0; d < 4; d++) {
                int nr = r + dr[d], nc = c + dc[d];
                if (nr < 0 || nr >= R || nc < 0 || nc >= C) continue;
                int y = nr * C + nc;
                if (land[y] && !seen[y]) { seen[y] = 1; queue[t++] = y; }
            }
        }
    }
    free(seen);
    free(queue);
    return count;
}

int main(void) {
    int k;
    printf("Enter grid size R C: ");
    if (scanf("%d %d", &R, &C) != 2 || R < 1 || C < 1) { printf("Invalid input\n"); return 1; }
    printf("Enter number of land additions k: ");
    if (scanf("%d", &k) != 1 || k < 0) { printf("Invalid input\n"); return 1; }

    int N = R * C;
    parent = malloc(N * sizeof(int));
    rnk = calloc(N, sizeof(int));
    land = calloc(N, 1);
    for (int i = 0; i < N; i++) parent[i] = i;

    int count = 0, mismatches = 0;
    int dr[4] = {-1, 1, 0, 0}, dc[4] = {0, 0, -1, 1};
    printf("Enter %d positions (r c), 0-based:\n", k);
    for (int i = 0; i < k; i++) {
        int r, c;
        scanf("%d %d", &r, &c);
        if (r < 0 || r >= R || c < 0 || c >= C) { printf("  (%d, %d) out of range, ignored\n", r, c); continue; }
        int x = r * C + c;
        if (!land[x]) {
            land[x] = 1;
            count++;
            for (int d = 0; d < 4; d++) {
                int nr = r + dr[d], nc = c + dc[d];
                if (nr < 0 || nr >= R || nc < 0 || nc >= C) continue;
                int y = nr * C + nc;
                if (land[y] && unite(x, y)) count--;
            }
        }
        int check = floodFillCount();
        if (check != count) mismatches++;
        printf("  add (%d, %d) -> islands = %d   (flood fill: %d)\n", r, c, count, check);
    }

    if (N <= 400) {
        printf("\nFinal grid (# = land):\n");
        for (int r = 0; r < R; r++) {
            printf("  ");
            for (int c = 0; c < C; c++) putchar(land[r * C + c] ? '#' : '.');
            printf("\n");
        }
    }
    printf("\nValidation: %s\n", mismatches == 0 ? "all counts MATCH" : "MISMATCH found");

    free(parent); free(rnk); free(land);
    return 0;
}
