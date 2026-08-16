#include <stdio.h>
#include <stdlib.h>

void expandSpecial(int *g, int n, int **out, int rowOff, int colOff) {
    if (n == 1) {
        out[rowOff][colOff] = g[0];
        return;
    }
    int half = n / 2;
    /* top-left = top-right's "M1" part uses g[0..half-1] (call it block A)
       bottom part uses g[half..n-1] (call it block B)
       Full matrix = [A B; B A] */
    expandSpecial(g,        half, out, rowOff,        colOff);        /* A -> top-left */
    expandSpecial(g + half, half, out, rowOff,        colOff + half); /* B -> top-right */
    expandSpecial(g + half, half, out, rowOff + half,  colOff);       /* B -> bottom-left */
    expandSpecial(g,        half, out, rowOff + half,  colOff + half);/* A -> bottom-right */
}

int **allocMatrix(int n) {
    int **m = malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) m[i] = malloc(n * sizeof(int));
    return m;
}
void freeMatrix(int **m, int n) { for (int i = 0; i < n; i++) free(m[i]); free(m); }

/* naive O(n^3) multiply, used only to verify correctness on small n */
int **naiveMultiply(int **a, int **b, int n) {
    int **c = allocMatrix(n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            long sum = 0;
            for (int k = 0; k < n; k++) sum += (long) a[i][k] * b[k][j];
            c[i][j] = (int) sum;
        }
    return c;
}

void addVec(int *r, int *a, int *b, int n) { for (int i = 0; i < n; i++) r[i] = a[i] + b[i]; }

void fastMultiplyGenVectors(int *gA, int *gB, int *gC, int n) {
    if (n == 1) {
        gC[0] = gA[0] * gB[0];
        return;
    }
    int half = n / 2;
    int *A1 = gA, *A2 = gA + half;
    int *B1 = gB, *B2 = gB + half;

    int *P1 = malloc(half * sizeof(int)); /* A1*B1 */
    int *P2 = malloc(half * sizeof(int)); /* A2*B2 */
    int *P3 = malloc(half * sizeof(int)); /* A1*B2 */
    int *P4 = malloc(half * sizeof(int)); /* A2*B1 */

    fastMultiplyGenVectors(A1, B1, P1, half);
    fastMultiplyGenVectors(A2, B2, P2, half);
    fastMultiplyGenVectors(A1, B2, P3, half);
    fastMultiplyGenVectors(A2, B1, P4, half);

    int *C1 = gC;
    int *C2 = gC + half;
    addVec(C1, P1, P2, half); /* C1 = A1B1 + A2B2 */
    addVec(C2, P3, P4, half); /* C2 = A1B2 + A2B1 */

    free(P1); free(P2); free(P3); free(P4);
}

int isPowerOfTwo(int n) { return n > 0 && (n & (n - 1)) == 0; }

int main(void) {
    int n;
    printf("Enter n (matrix size, must be a power of 2): ");
    if (scanf("%d", &n) != 1 || !isPowerOfTwo(n)) {
        printf("n must be a positive power of 2.\n");
        return 1;
    }

    int *gA = malloc(n * sizeof(int));
    int *gB = malloc(n * sizeof(int));
    printf("Enter %d generating values for matrix A (defines the special pattern): ", n);
    for (int i = 0; i < n; i++) scanf("%d", &gA[i]);
    printf("Enter %d generating values for matrix B: ", n);
    for (int i = 0; i < n; i++) scanf("%d", &gB[i]);

    /* Fast O(n log n) <= O(n^2) computation */
    int *gC = malloc(n * sizeof(int));
    fastMultiplyGenVectors(gA, gB, gC, n);

    /* Expand full matrices to display and to verify against naive multiply */
    int **A = allocMatrix(n), **B = allocMatrix(n), **Cfast = allocMatrix(n);
    expandSpecial(gA, n, A, 0, 0);
    expandSpecial(gB, n, B, 0, 0);
    expandSpecial(gC, n, Cfast, 0, 0);

    int **Cref = naiveMultiply(A, B, n);

    printf("\nMatrix A:\n");
    for (int i = 0; i < n; i++) { for (int j = 0; j < n; j++) printf("%d ", A[i][j]); printf("\n"); }
    printf("\nMatrix B:\n");
    for (int i = 0; i < n; i++) { for (int j = 0; j < n; j++) printf("%d ", B[i][j]); printf("\n"); }
    printf("\nProduct C = A x B (fast D&C on generating vectors):\n");
    for (int i = 0; i < n; i++) { for (int j = 0; j < n; j++) printf("%d ", Cfast[i][j]); printf("\n"); }

    int ok = 1;
    for (int i = 0; i < n && ok; i++)
        for (int j = 0; j < n; j++)
            if (Cfast[i][j] != Cref[i][j]) { ok = 0; break; }
    printf("\nVerification against naive O(n^3) full multiplication: %s\n",
           ok ? "MATCH (correct)" : "MISMATCH (bug!)");

    free(gA); free(gB); free(gC);
    freeMatrix(A, n); freeMatrix(B, n); freeMatrix(Cfast, n); freeMatrix(Cref, n);
    return 0;
}
