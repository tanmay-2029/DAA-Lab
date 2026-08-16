//Q4: Matrix Multiplication using Divide and Conquer (Strassen's Method)

#include <stdio.h>
#include <stdlib.h>

typedef int **Matrix;

Matrix allocMatrix(int n) {
    Matrix m = malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) m[i] = calloc(n, sizeof(int));
    return m;
}

void freeMatrix(Matrix m, int n) {
    for (int i = 0; i < n; i++) free(m[i]);
    free(m);
}

Matrix addMatrix(Matrix a, Matrix b, int n) {
    Matrix c = allocMatrix(n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            c[i][j] = a[i][j] + b[i][j];
    return c;
}

Matrix subMatrix(Matrix a, Matrix b, int n) {
    Matrix c = allocMatrix(n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            c[i][j] = a[i][j] - b[i][j];
    return c;
}

/* naive O(n^3) multiplication, used as the base case and for verification */
Matrix naiveMultiply(Matrix a, Matrix b, int n) {
    Matrix c = allocMatrix(n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            int sum = 0;
            for (int k = 0; k < n; k++) sum += a[i][k] * b[k][j];
            c[i][j] = sum;
        }
    return c;
}

#define STRASSEN_CUTOFF 2 /* below this size, fall back to naive multiply */

Matrix strassenMultiply(Matrix a, Matrix b, int n) {
    if (n <= STRASSEN_CUTOFF) return naiveMultiply(a, b, n);

    int half = n / 2;

    Matrix a11 = allocMatrix(half), a12 = allocMatrix(half);
    Matrix a21 = allocMatrix(half), a22 = allocMatrix(half);
    Matrix b11 = allocMatrix(half), b12 = allocMatrix(half);
    Matrix b21 = allocMatrix(half), b22 = allocMatrix(half);

    for (int i = 0; i < half; i++)
        for (int j = 0; j < half; j++) {
            a11[i][j] = a[i][j];
            a12[i][j] = a[i][j + half];
            a21[i][j] = a[i + half][j];
            a22[i][j] = a[i + half][j + half];

            b11[i][j] = b[i][j];
            b12[i][j] = b[i][j + half];
            b21[i][j] = b[i + half][j];
            b22[i][j] = b[i + half][j + half];
        }

    Matrix t1 = addMatrix(a11, a22, half);
    Matrix t2 = addMatrix(b11, b22, half);
    Matrix m1 = strassenMultiply(t1, t2, half);
    freeMatrix(t1, half); freeMatrix(t2, half);

    Matrix t3 = addMatrix(a21, a22, half);
    Matrix m2 = strassenMultiply(t3, b11, half);
    freeMatrix(t3, half);

    Matrix t4 = subMatrix(b12, b22, half);
    Matrix m3 = strassenMultiply(a11, t4, half);
    freeMatrix(t4, half);

    Matrix t5 = subMatrix(b21, b11, half);
    Matrix m4 = strassenMultiply(a22, t5, half);
    freeMatrix(t5, half);

    Matrix t6 = addMatrix(a11, a12, half);
    Matrix m5 = strassenMultiply(t6, b22, half);
    freeMatrix(t6, half);

    Matrix t7 = subMatrix(a21, a11, half);
    Matrix t8 = addMatrix(b11, b12, half);
    Matrix m6 = strassenMultiply(t7, t8, half);
    freeMatrix(t7, half); freeMatrix(t8, half);

    Matrix t9  = subMatrix(a12, a22, half);
    Matrix t10 = addMatrix(b21, b22, half);
    Matrix m7 = strassenMultiply(t9, t10, half);
    freeMatrix(t9, half); freeMatrix(t10, half);

    Matrix c = allocMatrix(n);
    for (int i = 0; i < half; i++)
        for (int j = 0; j < half; j++) {
            c[i][j]               = m1[i][j] + m4[i][j] - m5[i][j] + m7[i][j];
            c[i][j + half]        = m3[i][j] + m5[i][j];
            c[i + half][j]        = m2[i][j] + m4[i][j];
            c[i + half][j + half] = m1[i][j] - m2[i][j] + m3[i][j] + m6[i][j];
        }

    freeMatrix(a11, half); freeMatrix(a12, half); freeMatrix(a21, half); freeMatrix(a22, half);
    freeMatrix(b11, half); freeMatrix(b12, half); freeMatrix(b21, half); freeMatrix(b22, half);
    freeMatrix(m1, half); freeMatrix(m2, half); freeMatrix(m3, half); freeMatrix(m4, half);
    freeMatrix(m5, half); freeMatrix(m6, half); freeMatrix(m7, half);

    return c;
}

void printMatrix(Matrix m, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) printf("%d ", m[i][j]);
        printf("\n");
    }
}

int isPowerOfTwo(int n) { return n > 0 && (n & (n - 1)) == 0; }

int main(void) {
    int n;
    printf("Enter n (matrix size, must be a power of 2): ");
    if (scanf("%d", &n) != 1 || !isPowerOfTwo(n)) {
        printf("n must be a positive power of 2.\n");
        return 1;
    }

    Matrix A = allocMatrix(n), B = allocMatrix(n);
    printf("Enter matrix A (%d x %d):\n", n, n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) scanf("%d", &A[i][j]);

    printf("Enter matrix B (%d x %d):\n", n, n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) scanf("%d", &B[i][j]);

    Matrix C = strassenMultiply(A, B, n);
    printf("\nProduct C = A x B (Strassen):\n");
    printMatrix(C, n);

    /* verify against naive multiplication */
    Matrix Cref = naiveMultiply(A, B, n);
    int ok = 1;
    for (int i = 0; i < n && ok; i++)
        for (int j = 0; j < n; j++)
            if (C[i][j] != Cref[i][j]) { ok = 0; break; }
    printf("\nVerification against naive O(n^3) multiplication: %s\n",
           ok ? "MATCH (correct)" : "MISMATCH (bug!)");

    freeMatrix(A, n); freeMatrix(B, n); freeMatrix(C, n); freeMatrix(Cref, n);
    return 0;
}
