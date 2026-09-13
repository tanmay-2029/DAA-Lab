/*
 * Q1 - Invert the coin-triangle
 * ------------------------------
 * Name   : Tanmay
 * Course : BTech (CS-B / CE), 3rd Semester
 * Lab    : DAA Lab-07
 *
 * PROBLEM (in my own words):
 * We have coins arranged in an upward pointing equilateral triangle
 * (n rows, row 0 = top has 1 coin, row (n-1) = bottom has n coins).
 * We want to slide some coins so that the SAME coins now form a
 * DOWNWARD pointing triangle. Each move = sliding ONE coin to a new
 * empty spot. We want the MINIMUM number of moves.
 *
 * MY IDEA:
 * A coin that is already sitting on a spot that the final (inverted)
 * triangle also needs, does not have to move at all.
 * So minimum moves = (total coins) - (max coins that can already be
 * in the correct final spot).
 *
 * To find this, I give every coin an integer coordinate (no need for
 * sqrt(3) / floating point headaches):
 *      A = 2*c - r         B = r
 * where r = row number (0 = apex) and c = column in that row (0..r).
 * This (A,B) pair is a unique integer address for every coin centre
 * on the triangular grid, and neighbouring coins differ by simple
 * integer steps.
 *
 * If we rotate the WHOLE triangle by 180 degrees (that's what turns
 * "pointing up" into "pointing down") and then also SLIDE the whole
 * inverted shape by some amount (dA, dB), we still get a valid
 * downward triangle (just placed somewhere else). Rotating a point
 * (A,B) by 180 degrees (about the origin) simply gives (-A,-B).
 * Adding a shift (dA,dB) gives the final "target" grid.
 *
 * So I brute-force search over many possible shifts (dA, dB) and for
 * each one count how many ORIGINAL coin positions are ALSO present in
 * the shifted+rotated target grid. The shift that gives the maximum
 * overlap gives us the minimum number of moves:
 *
 *      min_moves = total_coins - max_overlap
 *
 * COMPLEXITY:
 *  total coins            m = n(n+1)/2
 *  number of shifts tried ~ O(n^2)
 *  checking one shift      O(m) points, each with an O(m) search
 *                          in the coin list  ->  O(m^2) per shift
 *  TOTAL                   O(n^2 * m^2) = O(n^6)  (a lot, but this
 *  is a brute force search done for a LAB so it is okay for small n,
 *  like n up to 8 or so, same size as the figure in the question).
 *
 *  NOTE: This can be made much faster (down to about O(n^4)) by using
 *  a hash-set instead of a linear scan for "does this point exist",
 *  but I kept it simple and easy to follow for now.
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int A;
    int B;
} Point;

/* simple linear search - is (a,b) present in the array? */
int isPresent(Point arr[], int count, int a, int b) {
    int i;
    for (i = 0; i < count; i++) {
        if (arr[i].A == a && arr[i].B == b) {
            return 1;
        }
    }
    return 0;
}

int main(void) {
    int n, total;
    int r, c, idx;

    printf("=== Q1: Invert the Coin Triangle ===\n");
    printf("Enter number of rows of the triangle (n): ");
    scanf("%d", &n);

    if (n <= 0) {
        printf("n should be a positive number.\n");
        return 0;
    }

    total = n * (n + 1) / 2;
    Point *coins = (Point *)malloc(total * sizeof(Point));
    if (coins == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    /* Step 1: build the original (upward) triangle coordinates */
    idx = 0;
    for (r = 0; r < n; r++) {
        for (c = 0; c <= r; c++) {
            coins[idx].A = 2 * c - r;
            coins[idx].B = r;
            idx++;
        }
    }

    /* Step 2: try many shifts of the rotated triangle and keep the
     * best (maximum overlap) one */
    int bestOverlap = -1, bestDA = 0, bestDB = 0;
    int dA, dB;

    for (dA = -n; dA <= n; dA++) {
        for (dB = 0; dB <= 2 * n; dB++) {
            int overlap = 0;
            for (idx = 0; idx < total; idx++) {
                /* rotate coin (A,B) by 180 degrees -> (-A,-B)      */
                /* then shift the whole rotated triangle by dA,dB   */
                int targetA = -coins[idx].A + dA;
                int targetB = -coins[idx].B + dB;

                if (isPresent(coins, total, targetA, targetB)) {
                    overlap++;
                }
            }
            if (overlap > bestOverlap) {
                bestOverlap = overlap;
                bestDA = dA;
                bestDB = dB;
            }
        }
    }

    int minMoves = total - bestOverlap;

    printf("\n---------- RESULT ----------\n");
    printf("Rows in the triangle      : %d\n", n);
    printf("Total number of coins     : %d\n", total);
    printf("Best overlap found        : %d coin(s) already correct\n", bestOverlap);
    printf("MINIMUM NUMBER OF MOVES   : %d\n", minMoves);
    printf("-----------------------------\n");

    printf("\n(For n = 4, the textbook's classic 10-coin example, this\n");
    printf("program should print 3, which matches the well known answer.)\n");

    free(coins);
    return 0;
}
