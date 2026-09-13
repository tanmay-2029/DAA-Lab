/*
 * Q3 - Reve's Puzzle (Tower of Hanoi with 4 pegs)
 * -------------------------------------------------
 * Name   : Tanmay
 * Course : BTech (CS-B / CE), 3rd Semester
 * Lab    : DAA Lab-07
 *
 * PROBLEM:
 * Normal Tower of Hanoi has 3 pegs and needs (2^n - 1) moves for n
 * disks. Here we get an EXTRA (4th) peg, which should let us solve
 * it in FEWER moves. For n = 8 disks, we are told the answer is 33.
 *
 * IDEA (Frame-Stewart Algorithm):
 * To move n disks using 4 pegs (from, to, and 2 spare pegs p1, p2):
 *   1. Pick some k disks (the top k, smallest ones) and move them from
 *      "from" to a spare peg p1, but we are still ALLOWED to use all
 *      4 pegs for this smaller sub-problem -> FS(k) moves.
 *   2. Now move the remaining (n-k) bigger disks from "from" to "to"
 *      using the classic 3-peg method (because peg p1 is now full of
 *      the k small disks, so it can't be used) -> costs (2^(n-k) - 1)
 *      moves.
 *   3. Move the k small disks from p1 to "to", again using all 4 pegs
 *      -> FS(k) moves again.
 *
 * So:  FS(n) = min over k (0 <= k < n) of [ 2*FS(k) + 2^(n-k) - 1 ]
 *      FS(0) = 0
 *
 * We try every possible k and keep the one that gives the least total
 * moves - this is exactly a DP problem (similar style to matrix chain
 * multiplication in Q7!).
 *
 * COMPLEXITY:
 *   For each n from 1 to N we try every k from 0 to n-1 -> O(N^2)
 *   overall for building the whole table of best move counts.
 *   Space is O(N).
 *
 * This program:
 *   (a) builds the DP table of minimum move counts FS[0..n]
 *   (b) prints FS[8] to confirm it really is 33
 *   (c) actually GENERATES the full move sequence (peg by peg) using
 *       the same recursive idea, so we can literally see and count
 *       the 33 moves for 8 disks.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int bestK[100];      /* bestK[n] = the k value that gave the minimum for n disks */
long long fsMoves[100]; /* fsMoves[n] = minimum number of moves for n disks, 4 pegs */

long long power2(int p) {
    long long result = 1;
    int i;
    for (i = 0; i < p; i++) result *= 2;
    return result;
}

/* fill fsMoves[] and bestK[] for 0..maxN using the recurrence */
void buildFrameStewartTable(int maxN) {
    int n, k;
    fsMoves[0] = 0;
    bestK[0] = 0;

    for (n = 1; n <= maxN; n++) {
        long long best = -1;
        int bestKforN = 0;
        for (k = 0; k < n; k++) {
            long long candidate = 2 * fsMoves[k] + power2(n - k) - 1;
            if (best == -1 || candidate < best) {
                best = candidate;
                bestKforN = k;
            }
        }
        fsMoves[n] = best;
        bestK[n] = bestKforN;
    }
}

int moveCounter = 0;

/* standard 3-peg Tower of Hanoi (used once the 4th peg is "busy") */
void hanoi3(int n, char from, char to, char via) {
    if (n == 0) return;
    hanoi3(n - 1, from, via, to);
    moveCounter++;
    printf("Move %2d: disk %d   %c -> %c\n", moveCounter, n, from, to);
    hanoi3(n - 1, via, to, from);
}

/* 4-peg version using the Frame-Stewart idea */
void hanoi4(int n, char from, char to, char p1, char p2) {
    if (n == 0) return;
    if (n == 1) {
        moveCounter++;
        printf("Move %2d: disk %d   %c -> %c\n", moveCounter, n, from, to);
        return;
    }

    int k = bestK[n];

    /* Step 1: move top k disks out of the way using all 4 pegs */
    hanoi4(k, from, p1, to, p2);

    /* Step 2: move remaining (n-k) disks with only 3 pegs available */
    hanoi3(n - k, from, to, p2);

    /* Step 3: bring the k disks back onto the destination peg */
    hanoi4(k, p1, to, from, p2);
}

int main(void) {
    int n;

    printf("=== Q3: Reve's Puzzle (4-peg Tower of Hanoi) ===\n");
    buildFrameStewartTable(20);

    printf("\nMinimum moves table (n disks, 4 pegs):\n");
    for (n = 1; n <= 10; n++) {
        printf("  n = %2d  ->  %lld moves\n", n, fsMoves[n]);
    }

    printf("\nFor n = 8 disks, the puzzle statement says the answer is 33.\n");
    printf("This program's table gives fsMoves[8] = %lld\n", fsMoves[8]);

    printf("\nEnter number of disks to actually SIMULATE the move sequence for: ");
    scanf("%d", &n);

    if (n < 1 || n > 12) {
        printf("Please choose a small n (1 to 12) so the move list stays readable.\n");
        return 0;
    }

    printf("\nMove sequence for n = %d disks (pegs A,B,C,D ; move all disks A -> D):\n", n);
    moveCounter = 0;
    hanoi4(n, 'A', 'D', 'B', 'C');

    printf("\nTotal moves actually made : %d\n", moveCounter);
    printf("Expected (from table)     : %lld\n", fsMoves[n]);

    return 0;
}
