/*
 * Q1: Fractional Knapsack with Deterioration Rate
 * -----------------------------------------------
 * Model: the knapsack contents are consumed one unit of weight per unit of
 * time, starting at t = 0, so a unit consumed in time slot t (t = 0,1,..,W-1)
 * is worth  d_i - lambda_i * t,  where d_i = v_i / w_i.
 * Weights and W are integers; an item may be taken in any number of whole
 * units 0..w_i, i.e. a fraction x_i / w_i of it.
 *
 * Step 1 - Greedy scheduling order (exchange argument):
 *   Take two units of items i and j in slots t1 < t2.
 *     i first : -lambda_i*t1 - lambda_j*t2
 *     j first : -lambda_j*t1 - lambda_i*t2
 *     (i first) - (j first) = (lambda_i - lambda_j)(t2 - t1)
 *   So i should come first iff lambda_i >= lambda_j. Any optimal solution can
 *   be rearranged to consume items in DECREASING order of lambda (fastest
 *   decaying first), each item in one contiguous block.
 *
 * Step 2 - Fractional choices:
 *   Picking amounts greedily in that order is NOT enough. Example: W = 1,
 *   A(v=1,w=1,lambda=100), B(v=5,w=1,lambda=0): greedy takes A first (value 1)
 *   but taking only B gives 5. With the order fixed, the amounts are chosen
 *   by a DP over (item, time used):
 *     best[j][t] = max over x in 0..min(w_j, t) of
 *                  best[j-1][t-x] + x*d_j - lambda_j * (x*(2t-x-1)/2)
 *   (x units of item j occupy slots t-x .. t-1). Answer = max_t best[n][t].
 *
 * Validation: brute force over every permutation of the items and every
 * integer amount of each item (small inputs only).
 *
 * Complexity:
 *   Sorting by lambda : O(n log n)
 *   DP                : O(n * W * wmax) <= O(n * W^2) time, O(n * W) space
 *   Brute force       : O(n! * prod(w_i + 1) * n)
 */
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int id;
    double v, lambda, d;
    int w;
} Item;

static int byLambdaDesc(const void *a, const void *b) {
    const Item *x = a, *y = b;
    if (x->lambda != y->lambda) return x->lambda < y->lambda ? 1 : -1;
    if (x->d != y->d) return x->d < y->d ? 1 : -1;
    return x->id - y->id;
}

/* Value of x units of item it occupying slots start .. start+x-1. */
static double blockValue(const Item *it, int start, int x) {
    double sumSlots = (double)x * (2.0 * start + x - 1) / 2.0;
    return x * it->d - it->lambda * sumSlots;
}

/* ---------- brute force ---------- */
static int n, W;
static Item *items;
static int perm[10], used[10];
static double bruteBest;

static void tryAmounts(int k, int t, double val) {
    if (val > bruteBest) bruteBest = val;
    if (k == n) return;
    const Item *it = &items[perm[k]];
    for (int x = 0; x <= it->w && t + x <= W; x++)
        tryAmounts(k + 1, t + x, val + blockValue(it, t, x));
}

static void tryPerms(int k) {
    if (k == n) { tryAmounts(0, 0, 0.0); return; }
    for (int i = 0; i < n; i++) {
        if (used[i]) continue;
        used[i] = 1; perm[k] = i;
        tryPerms(k + 1);
        used[i] = 0;
    }
}

int main(void) {
    printf("Enter number of items n: ");
    if (scanf("%d", &n) != 1 || n < 1) { printf("Invalid input\n"); return 1; }
    printf("Enter knapsack capacity W (integer): ");
    if (scanf("%d", &W) != 1 || W < 0) { printf("Invalid input\n"); return 1; }

    items = malloc(n * sizeof(Item));
    printf("Enter value v, weight w (integer) and decay rate lambda for each item:\n");
    for (int i = 0; i < n; i++) {
        printf("Item %d: ", i + 1);
        scanf("%lf %d %lf", &items[i].v, &items[i].w, &items[i].lambda);
        items[i].id = i + 1;
        items[i].d = items[i].v / items[i].w;
    }

    /* Step 1: greedy order */
    Item *ord = malloc(n * sizeof(Item));
    for (int i = 0; i < n; i++) ord[i] = items[i];
    qsort(ord, n, sizeof(Item), byLambdaDesc);

    printf("\nScheduling order (decreasing lambda): ");
    for (int i = 0; i < n; i++) printf("Item%d ", ord[i].id);
    printf("\n");

    /* Step 2: DP over (item, time used) */
    double **best = malloc((n + 1) * sizeof(double *));
    int **take = malloc((n + 1) * sizeof(int *));
    for (int j = 0; j <= n; j++) {
        best[j] = malloc((W + 1) * sizeof(double));
        take[j] = calloc(W + 1, sizeof(int));
    }
    best[0][0] = 0;
    for (int t = 1; t <= W; t++) best[0][t] = -1e300;   /* unreachable */

    for (int j = 1; j <= n; j++) {
        const Item *it = &ord[j - 1];
        for (int t = 0; t <= W; t++) {
            best[j][t] = -1e300;
            for (int x = 0; x <= it->w && x <= t; x++) {
                if (best[j - 1][t - x] <= -1e299) continue;
                double val = best[j - 1][t - x] + blockValue(it, t - x, x);
                if (val > best[j][t]) { best[j][t] = val; take[j][t] = x; }
            }
        }
    }

    int bestT = 0;
    for (int t = 0; t <= W; t++) if (best[n][t] > best[n][bestT]) bestT = t;

    int *units = calloc(n, sizeof(int));
    for (int j = n, t = bestT; j >= 1; j--) {
        units[j - 1] = take[j][t];
        t -= take[j][t];
    }

    printf("\n%6s %10s %10s %12s %10s\n", "Item", "fraction", "units", "time slots", "value");
    int t = 0;
    for (int j = 0; j < n; j++) {
        if (units[j] == 0) continue;
        printf("%6d %10.3f %10d %5d..%-6d %10.3f\n", ord[j].id, (double)units[j] / ord[j].w,
               units[j], t, t + units[j] - 1, blockValue(&ord[j], t, units[j]));
        t += units[j];
    }
    printf("\nMaximum total value = %.4f  (capacity used %d of %d)\n", best[n][bestT], bestT, W);

    double space = 1;
    for (int i = 0; i < n; i++) space *= items[i].w + 1;
    if (n <= 6 && space <= 200000) {
        bruteBest = 0;
        tryPerms(0);
        double diff = bruteBest - best[n][bestT];
        printf("Brute-force optimum = %.4f  (%s)\n", bruteBest,
               (diff < 1e-9 && diff > -1e-9) ? "MATCH" : "MISMATCH");
    }

    for (int j = 0; j <= n; j++) { free(best[j]); free(take[j]); }
    free(best); free(take); free(units); free(ord); free(items);
    return 0;
}
