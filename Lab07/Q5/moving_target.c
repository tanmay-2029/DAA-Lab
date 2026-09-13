/*
 * Q5 - Hitting a Moving Target
 * -----------------------------
 * Name   : Tanmay
 * Course : BTech (CS-B / CE), 3rd Semester
 * Lab    : DAA Lab-07
 *
 * PROBLEM:
 * There are n hiding spots in a straight line, numbered 1..n.
 * A target is hiding in one of them, but we never see it. Between
 * every two shots, the target MUST move to a NEIGHBOURING spot
 * (spot i can only go to i-1 or i+1, it can never stay still).
 * We fire one shot per turn at one spot. Does a strategy exist that
 * GUARANTEES a hit eventually, and if so, what is it?
 *
 * KEY OBSERVATION (parity trick):
 * Colour the spots alternately, like a chess board: 1=odd, 2=even,
 * 3=odd, ... Every time the target moves to a neighbour, its colour
 * FLIPS (odd->even or even->odd). So if we knew the target started
 * on an EVEN spot, we could just sweep 2,3,4,...,(n-1) - because at
 * the moment we shoot spot k, only targets that started on a spot of
 * the correct parity could possibly BE at spot k (this can be proven,
 * but the easiest way to convince ourselves is to actually SIMULATE
 * every possibility and check the "still might be alive" set becomes
 * empty).
 *
 * ALGORITHM (well known solution to this classic puzzle):
 *      Shoot:  2, 3, 4, ..., n-1        (forward sweep)
 *      then:   n-1, n-2, ..., 3, 2      (backward sweep)
 *      Total shots = 2*(n-2)
 *
 * The first sweep is guaranteed to catch the target if it started on
 * an EVEN spot. The second sweep (repeating the same spots backwards)
 * is guaranteed to catch it if it started on an ODD spot. Since it
 * must have started on ONE of the two, firing both sweeps guarantees
 * a hit.
 *
 * HOW I VERIFY THIS IN CODE (instead of just trusting the theory):
 * I keep a SET of "positions the target could still be sitting on,
 * given that it hasn't been shot yet". This is a standard technique
 * for this type of puzzle:
 *   1. Start: possible = {1, 2, ..., n}
 *   2. Shoot spot p -> remove p from 'possible' (any target really
 *      there is now dead, so it can't still be a "possible survivor")
 *   3. The target (if any) then moves -> every remaining possible
 *      position q is replaced by its neighbours (q-1 and/or q+1)
 *   4. Repeat with the next shot.
 * If 'possible' ever becomes EMPTY right after a shot (before the
 * next move), that means every starting position + every path the
 * target could have taken has already been shot at some point -
 * i.e. a hit is GUARANTEED.
 *
 * COMPLEXITY:
 *   n spots, O(n) shots in our strategy, each shot does O(n) work to
 *   update the possible-position set -> overall O(n^2) time, O(n)
 *   space. Very cheap.
 */

#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int n, i;

    printf("=== Q5: Hitting a Moving Target ===\n");
    printf("Enter number of hiding spots (n, n >= 2): ");
    scanf("%d", &n);

    if (n < 2) {
        printf("Need at least 2 spots for this puzzle to make sense.\n");
        return 0;
    }

    /* build the shot sequence: 2,3,...,n-1, then n-1,...,3,2 */
    int maxShots = 2 * (n - 2);
    if (maxShots < 0) maxShots = 0; /* n=2 or n=3 edge cases */

    int *shots = (int *)malloc((maxShots > 0 ? maxShots : 1) * sizeof(int));
    int shotCount = 0;

    if (n >= 3) {
        for (i = 2; i <= n - 1; i++) shots[shotCount++] = i;
        for (i = n - 1; i >= 2; i--) shots[shotCount++] = i;
    } else {
        /* n == 2: with only spots 1 and 2, the target must hop
         * between them every single move, so shooting the SAME
         * spot twice (with the forced move in between) is enough
         * to guarantee catching either starting parity. */
        shots[shotCount++] = 1;
        shots[shotCount++] = 1;
    }

    printf("\nShot sequence chosen (%d shots): ", shotCount);
    for (i = 0; i < shotCount; i++) printf("%d ", shots[i]);
    printf("\n");

    /* possible[i] = 1 means spot (i+1) is still a candidate for
     * where an un-hit target could currently be */
    int *possible = (int *)malloc(n * sizeof(int));
    for (i = 0; i < n; i++) possible[i] = 1;

    int guaranteedHitAt = -1;

    for (int s = 0; s < shotCount; s++) {
        int shotSpot = shots[s];

        /* fire the shot: remove that spot from possible set */
        possible[shotSpot - 1] = 0;

        /* check: is the possible set already empty? */
        int anyLeft = 0;
        for (i = 0; i < n; i++) if (possible[i]) anyLeft = 1;

        if (!anyLeft) {
            guaranteedHitAt = s + 1;
            printf("\nAfter shot #%d (spot %d), NO possible position is left.\n",
                   s + 1, shotSpot);
            printf("=> The target is GUARANTEED to have been hit by now.\n");
            break;
        }

        /* target moves: every remaining possible spot spreads to its
         * neighbours for the NEXT round */
        int *nextPossible = (int *)calloc(n, sizeof(int));
        for (i = 0; i < n; i++) {
            if (possible[i]) {
                if (i - 1 >= 0) nextPossible[i - 1] = 1; /* moved left  */
                if (i + 1 < n)  nextPossible[i + 1] = 1; /* moved right */
            }
        }
        free(possible);
        possible = nextPossible;
    }

    printf("\n---------- RESULT ----------\n");
    if (guaranteedHitAt != -1) {
        printf("Guaranteed hit achieved after %d shot(s).\n", guaranteedHitAt);
    } else {
        printf("Full shot sequence used (%d shots) - checking final state...\n", shotCount);
        int anyLeft = 0;
        for (i = 0; i < n; i++) if (possible[i]) anyLeft = 1;
        if (!anyLeft) {
            printf("Target guaranteed hit by the end of the sequence.\n");
        } else {
            printf("Some positions still possible - something is off for this n.\n");
        }
    }
    printf("Number of shots in strategy : %d  (matches formula 2*(n-2) for n>=3)\n", shotCount);

    free(possible);
    free(shots);
    return 0;
}
