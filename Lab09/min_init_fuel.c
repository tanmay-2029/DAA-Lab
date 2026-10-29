/*
 * Q3: Minimum number of refuelling stops (and minimum initial fuel)
 * -----------------------------------------------------------------
 * Input : target D, initial fuel F, number of stations n, then n lines
 *         "d_i f_i" (distance from origin, fuel available). 1 unit of fuel
 *         drives 1 unit of distance; the tank is unlimited.
 *
 * (a) Minimum stops - "reverse" (deferred) greedy with a max-heap:
 *   Drive as far as the current fuel allows, remembering every station passed
 *   in a max-heap. Only when we cannot go further do we decide, looking back,
 *   to have stopped at the passed station with the MOST fuel.
 *   Why optimal: after k stops this gives the farthest reachable distance,
 *   since any k stations among those reachable give at most the k largest.
 *   If the heap is empty when fuel runs out, the target is unreachable (-1).
 *
 * (b) Minimum initial fuel needed if we stop at every station on the way:
 *   need = max over every point p (each station and D) of
 *          ( p - total fuel of stations strictly before p ).
 *
 * Validation of (a): DP  far[k] = farthest distance reachable with k stops,
 *   for each station i (in order) and k from i down to 0:
 *   if far[k] >= d_i then far[k+1] = max(far[k+1], far[k] + f_i).
 *   Answer = smallest k with far[k] >= D.
 *
 * Complexity:
 *   Sorting stations   : O(n log n)
 *   Greedy with heap   : O(n log n) time, O(n) space
 *   Min initial fuel   : O(n) after sorting
 *   DP validation      : O(n^2) time, O(n) space
 */
#include <stdio.h>
#include <stdlib.h>

typedef struct { long long d, f; } Station;

static int byDistance(const void *a, const void *b) {
    const Station *x = a, *y = b;
    return (x->d > y->d) - (x->d < y->d);
}

/* ---- max-heap of stations, keyed by fuel ---- */
static Station *heap;
static int heapSize = 0;

static void push(Station v) {
    int i = heapSize++;
    heap[i] = v;
    while (i > 0 && heap[(i - 1) / 2].f < heap[i].f) {
        Station t = heap[i]; heap[i] = heap[(i - 1) / 2]; heap[(i - 1) / 2] = t;
        i = (i - 1) / 2;
    }
}

static Station pop(void) {
    Station top = heap[0];
    heap[0] = heap[--heapSize];
    int i = 0;
    for (;;) {
        int l = 2 * i + 1, r = l + 1, m = i;
        if (l < heapSize && heap[l].f > heap[m].f) m = l;
        if (r < heapSize && heap[r].f > heap[m].f) m = r;
        if (m == i) break;
        Station t = heap[i]; heap[i] = heap[m]; heap[m] = t; i = m;
    }
    return top;
}

static int greedyStops(long long D, long long F, const Station *s, int n, Station *chosen) {
    long long reach = F;   /* farthest point reachable so far */
    int stops = 0, i = 0;
    heapSize = 0;
    while (reach < D) {
        while (i < n && s[i].d <= reach) push(s[i++]);
        if (heapSize == 0) return -1;
        Station best = pop();
        reach += best.f;
        chosen[stops++] = best;
    }
    return stops;
}

static int dpStops(long long D, long long F, const Station *s, int n) {
    long long *far = malloc((n + 1) * sizeof(long long));
    far[0] = F;
    for (int k = 1; k <= n; k++) far[k] = -1;
    for (int i = 0; i < n; i++)
        for (int k = i; k >= 0; k--)
            if (far[k] >= s[i].d && far[k] + s[i].f > far[k + 1])
                far[k + 1] = far[k] + s[i].f;
    int ans = -1;
    for (int k = 0; k <= n; k++) if (far[k] >= D) { ans = k; break; }
    free(far);
    return ans;
}

int main(void) {
    long long D, F;
    int n;
    printf("Enter target distance D and initial fuel F: ");
    if (scanf("%lld %lld", &D, &F) != 2) { printf("Invalid input\n"); return 1; }
    printf("Enter number of stations: ");
    if (scanf("%d", &n) != 1 || n < 0) { printf("Invalid input\n"); return 1; }

    Station *s = malloc((n + 1) * sizeof(Station));
    for (int i = 0; i < n; i++) {
        printf("Station %d (distance fuel): ", i + 1);
        scanf("%lld %lld", &s[i].d, &s[i].f);
    }
    qsort(s, n, sizeof(Station), byDistance);
    heap = malloc((n + 1) * sizeof(Station));

    Station *chosen = malloc((n + 1) * sizeof(Station));
    int stops = greedyStops(D, F, s, n, chosen);

    printf("\n(a) Minimum number of refuelling stops = %d\n", stops);
    if (stops > 0) {
        printf("    Stops used (distance, fuel): ");
        for (int i = 0; i < stops; i++) printf("(%lld, %lld) ", chosen[i].d, chosen[i].f);
        printf("\n");
    } else if (stops < 0) {
        printf("    Target cannot be reached.\n");
    }

    int dp = dpStops(D, F, s, n);
    printf("    DP check = %d  (%s)\n", dp, dp == stops ? "MATCH" : "MISMATCH");

    long long need = 0, collected = 0;
    for (int i = 0; i <= n; i++) {
        long long p = (i < n && s[i].d < D) ? s[i].d : D;
        if (p - collected > need) need = p - collected;
        if (i == n || s[i].d >= D) break;
        collected += s[i].f;
    }
    printf("\n(b) Minimum initial fuel to reach D (stopping at every station) = %lld\n", need);

    free(s); free(heap); free(chosen);
    return 0;
}
