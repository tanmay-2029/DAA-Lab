/*
 * Q6 - The Best Time to Be Alive
 * ---------------------------------
 * Name   : Tanmay
 * Course : BTech (CS-B / CE), 3rd Semester
 * Lab    : DAA Lab-07
 *
 * PROBLEM:
 * We are given a list of scientists, each with a birth year and a
 * death year. We need to find the year (or years) when the LARGEST
 * number of these scientists were alive AT THE SAME TIME.
 * Special tie-breaking rule given in the question: if scientist A
 * died in the SAME year scientist B was born, then A's death is
 * considered to have happened BEFORE B's birth (so they were never
 * "alive together" in that shared year).
 *
 * IDEA (classic sweep-line / max overlapping intervals):
 * Every scientist contributes 2 "events":
 *      (birth_year,  +1)   -- someone becomes alive
 *      (death_year,  -1)   -- someone stops being alive
 *
 * If we sort all these events by year, and sweep through them while
 * keeping a running counter of "how many are alive right now", the
 * counter's maximum value (and the year where it happens) is exactly
 * the answer we want.
 *
 * THE TRICKY PART - the tie rule:
 * If a birth and a death happen in the SAME year, we must process
 * the DEATH event (-1) BEFORE the BIRTH event (+1) for that year.
 * That way the dying scientist is already "removed" before the new
 * one is "added", so they don't get counted as alive together.
 * I do this simply by sorting events primarily by year, and for
 * equal years, deaths (-1) come before births (+1).
 *
 * COMPLEXITY:
 *   n scientists -> 2n events.
 *   Sorting takes O(n log n).
 *   The single sweep afterwards takes O(n).
 *   TOTAL TIME  = O(n log n)
 *   SPACE       = O(n)   (for the events array)
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int year;
    int type; /* 0 = death (-1), 1 = birth (+1)  -- death must sort first */
} Event;

/* comparator for qsort: sort by year, and for equal years put
 * deaths (type 0) before births (type 1) */
int compareEvents(const void *a, const void *b) {
    const Event *e1 = (const Event *)a;
    const Event *e2 = (const Event *)b;

    if (e1->year != e2->year) {
        return e1->year - e2->year;
    }
    return e1->type - e2->type; /* 0 (death) before 1 (birth) */
}

int main(void) {
    int n, i;

    printf("=== Q6: The Best Time to Be Alive ===\n");
    printf("Enter number of scientists: ");
    scanf("%d", &n);

    if (n <= 0) {
        printf("Need at least 1 scientist.\n");
        return 0;
    }

    Event *events = (Event *)malloc(2 * n * sizeof(Event));

    printf("Enter birth year and death year for each scientist:\n");
    for (i = 0; i < n; i++) {
        int birth, death;
        printf("  Scientist %d (birth death): ", i + 1);
        scanf("%d %d", &birth, &death);

        events[2 * i].year = birth;
        events[2 * i].type = 1; /* birth = +1 */

        events[2 * i + 1].year = death;
        events[2 * i + 1].type = 0; /* death = -1 */
    }

    qsort(events, 2 * n, sizeof(Event), compareEvents);

    int aliveNow = 0;
    int maxAlive = 0;
    int bestYear = 0;

    for (i = 0; i < 2 * n; i++) {
        if (events[i].type == 1) {
            aliveNow++;   /* someone was born */
        } else {
            aliveNow--;   /* someone died */
        }

        if (aliveNow > maxAlive) {
            maxAlive = aliveNow;
            bestYear = events[i].year;
        }
    }

    printf("\n---------- RESULT ----------\n");
    printf("Best year to be alive        : %d\n", bestYear);
    printf("Number of scientists alive   : %d\n", maxAlive);
    printf("-----------------------------\n");

    free(events);
    return 0;
}
