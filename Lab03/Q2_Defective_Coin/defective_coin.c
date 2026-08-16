#include <stdlib.h>
#include <math.h>

static long weighings = 0; /* counts calls to the balance scale */


int weigh(int weights[], int leftIdx, int rightIdx, int len) {
    weighings++;
    long sumLeft = 0, sumRight = 0;
    for (int i = 0; i < len; i++) {
        sumLeft  += weights[leftIdx + i];
        sumRight += weights[rightIdx + i];
    }
    if (sumLeft < sumRight) return -1;
    if (sumLeft > sumRight) return 1;
    return 0;
}

int findDefective(int weights[], int lo, int hi) {
    int n = hi - lo + 1;
    if (n <= 0) return -1;
    if (n == 1) return lo; /* single suspect coin */

    int half = n / 2;
    int leftStart  = lo;
    int rightStart = lo + half;
    int cmpLen  = half;
    int leftover = n - 2 * half; /* 0 if even, 1 if odd */

    int result = weigh(weights, leftStart, rightStart, cmpLen);

    if (result == 0) {
        if (leftover == 1) return lo + 2 * half; /* leftover is sole suspect */
        return -1; /* whole range genuine */
    } else if (result < 0) {
        return findDefective(weights, leftStart, leftStart + cmpLen - 1);
    } else {
        return findDefective(weights, rightStart, rightStart + cmpLen - 1);
    }
}

/* Benchmark mode: for sizes 1..N, place the defective coin at a
 * random position, run findDefective, and record the number of
 * weighings used (plus the theoretical floor(log2 n) bound). */
static void benchmark(int maxN) {
    printf("n,weighings,log2_n_floor\n");
    for (int n = 1; n <= maxN; n *= 2) {
        int *weights = malloc(sizeof(int) * n);
        for (int i = 0; i < n; i++) weights[i] = 100;
        int defectivePos = rand() % n;
        weights[defectivePos] = 99;

        weighings = 0;
        int idx = findDefective(weights, 0, n - 1);
        if (idx != -1 && n > 1) {
            int refIdx = (idx == 0) ? 1 : 0;
            weigh(weights, idx, refIdx, 1);
        }
        int logBound = (n <= 1) ? 0 : (int) floor(log2((double) n));
        printf("%d,%ld,%d\n", n, weighings, logBound);
        free(weights);
    }
}

int main(int argc, char *argv[]) {
    if (argc >= 3 && argv[1][0] == '-' && argv[1][1] == '-' && argv[1][2] == 'b') {
        srand(7);
        benchmark(atoi(argv[2]));
        return 0;
    }

    int n;
    printf("Enter number of coins: ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Invalid input\n");
        return 1;
    }

    int *weights = malloc(sizeof(int) * n);
    printf("Enter %d coin weights (all equal except at most one lighter coin): ", n);
    for (int i = 0; i < n; i++) scanf("%d", &weights[i]);

    weighings = 0;
    int defectiveIdx = findDefective(weights, 0, n - 1);
    
    if (defectiveIdx != -1 && n > 1) {
        int refIdx = (defectiveIdx == 0) ? 1 : 0;
        int cmp = weigh(weights, defectiveIdx, refIdx, 1);
        if (cmp >= 0) {
            /* suspect is not lighter than the reference -> genuine */
            defectiveIdx = -1;
        }
    }

    if (defectiveIdx == -1) {
        printf("\nResult: No defective (lighter) coin found. All coins are genuine.\n");
    } else {
        printf("\nResult: Defective (lighter) coin found at position %d (1-indexed), weight = %d\n",
               defectiveIdx + 1, weights[defectiveIdx]);
    }
    printf("Number of weighings used: %ld  (theoretical bound: floor(log2 %d) + c = %d + c)\n",
           weighings, n, (int) floor(log2((double) n)));

    free(weights);
    return 0;
}
