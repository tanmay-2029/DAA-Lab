/*
 * Q4 - Security Switches Puzzle
 * -------------------------------
 * Name   : Tanmay
 * Course : BTech (CS-B / CE), 3rd Semester
 * Lab    : DAA Lab-07
 *
 * PROBLEM (rules given):
 *   - There are n switches, numbered 1 (left) .. n (right). All start ON.
 *   - Switch n (rightmost) can be toggled anytime, freely.
 *   - Any OTHER switch i can be toggled ONLY IF switch (i+1) is ON
 *     AND every switch to the right of (i+1) is OFF.
 *   - We must turn ALL switches OFF, using as few toggles as possible.
 *
 * HOW I FIGURED OUT THE RECURRENCE (working with pen and paper first):
 *
 * Let switches[0..n-1] be positions 1..n.
 * To flip switch 1 (the leftmost), we need switch2 = ON and switches
 * 3..n all OFF. So the plan for n switches, all ON -> all OFF, is:
 *
 *   Step 1: Using only switches [2..n] (a smaller version of the SAME
 *           puzzle, size n-2, positions 3..n) turn OFF positions 3..n,
 *           while leaving switch 2 untouched (it stays ON).
 *           This needs T(n-2) moves, where T(k) is the answer for a
 *           k-switch version of this whole puzzle.
 *
 *   Step 2: Now switch2 = ON, switches 3..n = OFF, so we are ALLOWED
 *           to flip switch1. That's 1 move.
 *
 *   Step 3: Now we still need to turn off switches [2..n], which are
 *           currently (ON, OFF, OFF, ..., OFF). Turning this exact
 *           pattern into all-OFF is a DIFFERENT (but related) sub
 *           puzzle. I call this g(n-1) - and it turns out that
 *           g(k) behaves exactly like normal Tower-of-Hanoi:
 *               g(k) = 2*g(k-1) + 1,   g(1) = 1
 *           which gives g(k) = 2^k - 1.
 *
 *   So overall:  T(n) = T(n-2) + 1 + g(n-1)
 *              = T(n-2) + 1 + (2^(n-1) - 1)
 *              = T(n-2) + 2^(n-1)
 *
 *   With base cases T(0) = 0, T(1) = 1.
 *
 *   If you unroll this recurrence, it simplifies to a nice closed
 *   form (I checked it against the recurrence for small n and it
 *   matches perfectly):
 *
 *              T(n) = floor( 2^(n+1) / 3 )
 *
 * I check this claimed formula in the code below by comparing it
 * against the actual number of moves my simulation makes.
 *
 * COMPLEXITY:
 *   The recursive move-generation makes exactly T(n) toggle calls,
 *   and T(n) = O(2^n) (same order of growth as ordinary Hanoi),
 *   so time = O(2^n). This is expected: these switches puzzles
 *   ("Chinese rings" style puzzles) are inherently exponential.
 */

#include <stdio.h>
#include <math.h>

int switches[64];      /* 1 = ON, 0 = OFF */
long long moveCount = 0;

void toggle(int index) {
    switches[index] = 1 - switches[index];
    moveCount++;
    printf("Move %3lld: toggle switch #%d  -> now %s\n",
           moveCount, index + 1, switches[index] ? "ON" : "OFF");
}

/*
 * g(n): turns the pattern (ON, OFF, OFF, ..., OFF) of n switches
 * (starting at 'startIndex') into all-OFF. Behaves exactly like
 * normal 3-peg Tower of Hanoi's move count: g(n) = 2^n - 1.
 */
void clearFromLeadingOn(int n, int startIndex) {
    if (n == 0) return;
    if (n == 1) {
        toggle(startIndex);
        return;
    }
    clearFromLeadingOn(n - 1, startIndex + 1);
    toggle(startIndex);
    clearFromLeadingOn(n - 1, startIndex + 1);
}

/*
 * clearAll(n): turns n switches (ALL currently ON) into ALL OFF,
 * following the security-switch rules. This is the main T(n).
 */
void clearAll(int n, int startIndex) {
    if (n == 0) return;
    if (n == 1) {
        toggle(startIndex);
        return;
    }
    /* Step 1: clear positions [startIndex+2 .. startIndex+n-1]
     *         (size n-2), leaving position startIndex+1 untouched (ON) */
    clearAll(n - 2, startIndex + 2);

    /* Step 2: now we are allowed to flip the leftmost switch */
    toggle(startIndex);

    /* Step 3: clear the (ON, OFF, ..., OFF) pattern left in
     *         positions [startIndex+1 .. startIndex+n-1] (size n-1) */
    clearFromLeadingOn(n - 1, startIndex + 1);
}

int main(void) {
    int n, i;

    printf("=== Q4: Security Switches Puzzle ===\n");
    printf("Enter number of switches (n, all initially ON): ");
    scanf("%d", &n);

    if (n < 1 || n > 20) {
        printf("Please pick n between 1 and 20 (keeps output readable).\n");
        return 0;
    }

    for (i = 0; i < n; i++) switches[i] = 1; /* all ON */

    printf("\nStarting state: all %d switches are ON.\n\n", n);
    moveCount = 0;
    clearAll(n, 0);

    printf("\n---------- RESULT ----------\n");
    printf("Total moves used         : %lld\n", moveCount);

    long long formulaAnswer = (long long)floor(pow(2.0, n + 1) / 3.0);
    printf("Formula floor(2^(n+1)/3) : %lld\n", formulaAnswer);
    printf("(These two numbers should always match!)\n");

    int allOff = 1;
    for (i = 0; i < n; i++) {
        if (switches[i] != 0) allOff = 0;
    }
    printf("All switches now OFF?    : %s\n", allOff ? "YES" : "NO (bug!)");

    return 0;
}
