/*
 * Q2 - Super Egg Testing Experiment
 * ----------------------------------
 * Name   : Tanmay
 * Course : BTech (CS-B / CE), 3rd Semester
 * Lab    : DAA Lab-07
 *
 * PROBLEM (in my own words):
 * We are given E eggs and a building with F floors. We want to find
 * the highest floor from which an egg can be dropped without
 * breaking. Some eggs might break when dropped from certain floors.
 * We want the minimum number of TRIALS that GUARANTEES we find the
 * answer, no matter how unlucky we are (worst case).
 *
 * DYNAMIC PROGRAMMING IDEA:
 * Let dp[e][f] = minimum number of trials needed in the worst case,
 * if we have e eggs and f floors to check.
 *
 * If we drop an egg from some floor "x" (1 <= x <= f):
 *   Case 1: Egg breaks.
 *           Now we have (e-1) eggs left, and we only need to check
 *           the (x-1) floors below x.
 *           Trials needed = dp[e-1][x-1]
 *   Case 2: Egg does NOT break.
 *           We still have e eggs, and we need to check the (f-x)
 *           floors above x.
 *           Trials needed = dp[e][f-x]
 *
 * Since we don't control whether the egg breaks or not, we must plan
 * for the WORST of the two cases:
 *      worst(x) = 1 + max( dp[e-1][x-1], dp[e][f-x] )
 *
 * We are allowed to CHOOSE which floor x to drop from, so we pick the
 * x that makes this worst case as SMALL as possible:
 *      dp[e][f] = min over x in [1..f] of worst(x)
 *
 * BASE CASES:
 *      dp[e][0] = 0            (no floors -> 0 trials needed)
 *      dp[1][f] = f             (with 1 egg we must test floors one
 *                                 by one from the bottom, no other
 *                                 choice, otherwise if it breaks we
 *                                 cannot be sure of the exact floor)
 *
 * COMPLEXITY:
 *   There are E*F sub-problems (states).
 *   Each sub-problem tries up to F values of x.
 *   So total time = O(E * F * F) = O(E * F^2)
 *   Space = O(E * F) for the dp table.
 *
 *   (There IS a smarter O(E * F log F) way and even an O(E log F)
 *    way using the "moves vs floors" trick, but for a lab exercise
 *    the O(E*F^2) table method is easiest to understand and code.)
 */

#include <stdio.h>
#include <stdlib.h>

int maxInt(int a, int b) { return (a > b) ? a : b; }
int minInt(int a, int b) { return (a < b) ? a : b; }

int eggDrop(int E, int F) {
    int e, f, x;

    /* dp[e][f] table, we use 1-indexed rows/cols for eggs/floors */
    int **dp = (int **)malloc((E + 1) * sizeof(int *));
    for (e = 0; e <= E; e++) {
        dp[e] = (int *)malloc((F + 1) * sizeof(int));
    }

    /* base case: 0 floors -> 0 trials, for any number of eggs */
    for (e = 0; e <= E; e++) {
        dp[e][0] = 0;
    }

    /* base case: 1 egg -> we must test floor by floor (linear) */
    for (f = 0; f <= F; f++) {
        dp[1][f] = f;
    }

    /* fill the rest of the table */
    for (e = 2; e <= E; e++) {
        for (f = 1; f <= F; f++) {
            int best = F + 1; /* something bigger than any real answer */
            for (x = 1; x <= f; x++) {
                int breaks   = dp[e - 1][x - 1];
                int survives = dp[e][f - x];
                int worst    = 1 + maxInt(breaks, survives);
                best = minInt(best, worst);
            }
            dp[e][f] = best;
        }
    }

    int answer = dp[E][F];

    for (e = 0; e <= E; e++) {
        free(dp[e]);
    }
    free(dp);

    return answer;
}

int main(void) {
    int E, F;

    printf("=== Q2: Super Egg Testing Experiment ===\n");
    printf("Enter number of eggs (E): ");
    scanf("%d", &E);
    printf("Enter number of floors (F): ");
    scanf("%d", &F);

    if (E <= 0 || F < 0) {
        printf("Please enter valid positive values.\n");
        return 0;
    }

    int trials = eggDrop(E, F);

    printf("\n---------- RESULT ----------\n");
    printf("Eggs (E)                    : %d\n", E);
    printf("Floors (F)                  : %d\n", F);
    printf("Min trials needed (worst case): %d\n", trials);
    printf("-----------------------------\n");

    printf("\nSanity check for the classic case (2 eggs, 100 floors),\n");
    printf("the known textbook answer is 14 -> this program gives: %d\n",
           eggDrop(2, 100));

    return 0;
}
