/*
 * Q9: Collatz Conjecture (3n+1 problem)
 * Lab-08, DAA
 *
 *   T(n) = n/2     if n is even
 *   T(n) = 3n + 1  if n is odd
 *
 * The program has 2 modes:
 *   1. trajectory of one starting value n
 *   2. analysis of every number in an interval [a, b]
 *
 * Things used: functions, loops, dynamic memory (malloc/realloc),
 * and overflow checking for unsigned long long.
 */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef unsigned long long ull;

// returns 1 if 3n+1 would overflow, else 0
int willOverflow(ull n)
{
    return n > (ULLONG_MAX - 1) / 3;
}

// one step of the Collatz function
ull nextTerm(ull n)
{
    if (n % 2 == 0)
        return n / 2;
    return 3 * n + 1;
}

// ---------- mode 1 : single number ----------
void analyseSingle(ull n)
{
    int capacity = 16;
    int size = 0;
    ull *traj = (ull *)malloc(capacity * sizeof(ull));
    if (traj == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }

    ull cur = n;
    ull peak = n;
    int overflow = 0;

    traj[size++] = cur;
    while (cur != 1) {
        if (cur % 2 == 1 && willOverflow(cur)) {
            overflow = 1;
            break;
        }
        cur = nextTerm(cur);
        if (cur > peak)
            peak = cur;

        // array full -> make it double the size
        if (size == capacity) {
            capacity = capacity * 2;
            ull *temp = (ull *)realloc(traj, capacity * sizeof(ull));
            if (temp == NULL) {
                printf("Memory allocation failed!\n");
                free(traj);
                return;
            }
            traj = temp;
        }
        traj[size++] = cur;
    }

    if (overflow) {
        printf("\nOverflow detected! Stopped before exceeding unsigned long long.\n");
    }

    printf("\nTrajectory of %llu:\n", n);
    int i;
    for (i = 0; i < size; i++) {
        printf("%llu", traj[i]);
        if (i != size - 1)
            printf(" -> ");
        if ((i + 1) % 8 == 0 && i != size - 1)
            printf("\n");
    }
    printf("\n\nNumber of steps   : %d\n", size - 1);
    printf("Maximum value     : %llu\n", peak);
    if (!overflow)
        printf("Reached 1         : yes\n");

    free(traj);
}

// ---------- mode 2 : interval ----------
void analyseInterval(ull a, ull b)
{
    ull n;
    ull bestStepsNum = a, bestPeakNum = a;
    int bestSteps = -1;
    ull bestPeak = 0;
    ull totalSteps = 0;
    ull count = 0;
    ull overflowCount = 0;

    for (n = a; n <= b; n++) {
        ull cur = n;
        ull peak = n;
        int steps = 0;
        int overflow = 0;

        while (cur != 1) {
            if (cur % 2 == 1 && willOverflow(cur)) {
                overflow = 1;
                break;
            }
            cur = nextTerm(cur);
            if (cur > peak)
                peak = cur;
            steps++;
        }

        if (overflow) {
            overflowCount++;
            continue;
        }

        totalSteps += steps;
        count++;

        if (steps > bestSteps) {
            bestSteps = steps;
            bestStepsNum = n;
        }
        if (peak > bestPeak) {
            bestPeak = peak;
            bestPeakNum = n;
        }

        if (n == ULLONG_MAX) break;   // avoid infinite loop at the end
    }

    printf("\nInterval [%llu, %llu]\n", a, b);
    printf("Numbers analysed          : %llu\n", count);
    if (count > 0) {
        printf("Longest trajectory        : n = %llu with %d steps\n", bestStepsNum, bestSteps);
        printf("Highest value reached     : n = %llu reaches %llu\n", bestPeakNum, bestPeak);
        printf("Average number of steps   : %.2f\n", (double)totalSteps / count);
    }
    if (overflowCount > 0)
        printf("Skipped due to overflow   : %llu\n", overflowCount);
    printf("Every analysed number reached 1 (no counterexample found).\n");
}

int main()
{
    int choice;
    ull n, a, b;

    printf("===== Collatz Conjecture Analyser =====\n");
    printf("1. Trajectory of a single number\n");
    printf("2. Analyse an interval [a, b]\n");
    printf("Enter your choice: ");
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input!\n");
        return 1;
    }

    if (choice == 1) {
        printf("Enter starting value n (n >= 1): ");
        scanf("%llu", &n);
        if (n < 1) {
            printf("n must be >= 1\n");
            return 1;
        }
        analyseSingle(n);
    } else if (choice == 2) {
        printf("Enter a and b (1 <= a <= b): ");
        scanf("%llu %llu", &a, &b);
        if (a < 1 || a > b) {
            printf("Invalid interval!\n");
            return 1;
        }
        analyseInterval(a, b);
    } else {
        printf("Invalid choice!\n");
        return 1;
    }

    return 0;
}
