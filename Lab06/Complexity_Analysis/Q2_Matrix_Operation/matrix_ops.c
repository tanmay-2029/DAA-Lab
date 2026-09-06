/*
 * Q2: 2D Square Matrix Operations and their Complexities
 *
 *   (i)   Matrix Addition                          -> O(n^2)
 *   (ii)  Matrix Multiplication                     -> O(n^3)  (naive)
 *   (iii) Zero matrix check                         -> O(n^2)
 *   (iv)  Symmetric matrix check                    -> O(n^2)
 *   (v)   Determinant                               -> O(n^3)  (Gaussian elimination)
 *   (vi)  Transpose in place                        -> O(n^2)
 *   (vii) Eigenvalue / eigenvector                  -> iterative, see note below
 *
 * NOTE on (vii): There is no simple closed-form worst-case complexity for
 * general eigen-decomposition; production algorithms (QR algorithm) are
 * iterative and only converge approximately. This program demonstrates the
 * *power iteration* method, which finds the dominant eigenvalue/eigenvector
 * in O(n^2) work per iteration (O(n^2 * k) for k iterations). See
 * ANALYSIS.md for a fuller discussion.
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAXN 20

void inputMatrix(double m[MAXN][MAXN], int n, const char *name) {
    printf("Enter elements of matrix %s (%d x %d), row-wise:\n", name, n, n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            scanf("%lf", &m[i][j]);
}

void printMatrix(double m[MAXN][MAXN], int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) printf("%8.2f ", m[i][j]);
        printf("\n");
    }
}

/* (i) Matrix Addition : O(n^2) */
void matrixAdd(double a[MAXN][MAXN], double b[MAXN][MAXN], double c[MAXN][MAXN], int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            c[i][j] = a[i][j] + b[i][j];
}

/* (ii) Matrix Multiplication : O(n^3) naive */
void matrixMultiply(double a[MAXN][MAXN], double b[MAXN][MAXN], double c[MAXN][MAXN], int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            c[i][j] = 0;
            for (int k = 0; k < n; k++)
                c[i][j] += a[i][k] * b[k][j];
        }
}

/* (iii) Zero matrix check : O(n^2) */
int isZeroMatrix(double m[MAXN][MAXN], int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (m[i][j] != 0) return 0;
    return 1;
}

/* (iv) Symmetric matrix check : O(n^2) */
int isSymmetric(double m[MAXN][MAXN], int n) {
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            if (m[i][j] != m[j][i]) return 0;
    return 1;
}

/* (v) Determinant via Gaussian elimination with partial pivoting : O(n^3) */
double determinant(double m[MAXN][MAXN], int n) {
    double a[MAXN][MAXN];
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            a[i][j] = m[i][j];

    double det = 1;
    for (int i = 0; i < n; i++) {
        int pivot = i;
        for (int k = i + 1; k < n; k++)
            if (fabs(a[k][i]) > fabs(a[pivot][i])) pivot = k;

        if (fabs(a[pivot][i]) < 1e-12) return 0;

        if (pivot != i) {
            for (int j = 0; j < n; j++) {
                double t = a[i][j]; a[i][j] = a[pivot][j]; a[pivot][j] = t;
            }
            det *= -1;
        }

        det *= a[i][i];
        for (int k = i + 1; k < n; k++) {
            double factor = a[k][i] / a[i][i];
            for (int j = i; j < n; j++)
                a[k][j] -= factor * a[i][j];
        }
    }
    return det;
}

/* (vi) Transpose in place : O(n^2) */
void transposeInPlace(double m[MAXN][MAXN], int n) {
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++) {
            double t = m[i][j]; m[i][j] = m[j][i]; m[j][i] = t;
        }
}

/* (vii) Dominant eigenvalue/eigenvector via Power Iteration : O(n^2) per iteration */
void powerIteration(double m[MAXN][MAXN], int n, int iterations, double *eigenvalue, double eigenvector[MAXN]) {
    double v[MAXN], vNew[MAXN];
    for (int i = 0; i < n; i++) v[i] = 1.0;

    for (int it = 0; it < iterations; it++) {
        for (int i = 0; i < n; i++) {
            vNew[i] = 0;
            for (int j = 0; j < n; j++) vNew[i] += m[i][j] * v[j];
        }
        double norm = 0;
        for (int i = 0; i < n; i++) norm += vNew[i] * vNew[i];
        norm = sqrt(norm);
        if (norm < 1e-12) norm = 1;
        for (int i = 0; i < n; i++) v[i] = vNew[i] / norm;
    }

    double numerator = 0, denominator = 0, av[MAXN];
    for (int i = 0; i < n; i++) {
        av[i] = 0;
        for (int j = 0; j < n; j++) av[i] += m[i][j] * v[j];
        numerator += v[i] * av[i];
        denominator += v[i] * v[i];
    }
    *eigenvalue = numerator / denominator;
    for (int i = 0; i < n; i++) eigenvector[i] = v[i];
}

int main(void) {
    int n;
    printf("=== 2D Square Matrix Operations and their Complexities ===\n");
    printf("Enter size of square matrix (n <= %d): ", MAXN);
    scanf("%d", &n);
    if (n > MAXN || n <= 0) { printf("Invalid n (must be 1..%d)\n", MAXN); return 1; }

    double A[MAXN][MAXN], B[MAXN][MAXN], C[MAXN][MAXN];
    inputMatrix(A, n, "A");

    int choice;
    do {
        printf("\n----------------- MENU -----------------\n");
        printf("1. Matrix Addition (needs matrix B)          (O(n^2))\n");
        printf("2. Matrix Multiplication (needs matrix B)    (O(n^3))\n");
        printf("3. Check Zero Matrix                         (O(n^2))\n");
        printf("4. Check Symmetric Matrix                    (O(n^2))\n");
        printf("5. Determinant (Gaussian elimination)        (O(n^3))\n");
        printf("6. Transpose A in place                      (O(n^2))\n");
        printf("7. Dominant Eigenvalue/Eigenvector (Power Iteration)\n");
        printf("8. Print current matrix A\n");
        printf("9. Re-enter matrix A\n");
        printf("0. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                inputMatrix(B, n, "B");
                matrixAdd(A, B, C, n);
                printf("A + B =\n"); printMatrix(C, n);
                break;
            case 2:
                inputMatrix(B, n, "B");
                matrixMultiply(A, B, C, n);
                printf("A * B =\n"); printMatrix(C, n);
                break;
            case 3:
                printf("%s\n", isZeroMatrix(A, n) ? "Matrix A is a ZERO matrix." : "Matrix A is NOT a zero matrix.");
                break;
            case 4:
                printf("%s\n", isSymmetric(A, n) ? "Matrix A is SYMMETRIC." : "Matrix A is NOT symmetric.");
                break;
            case 5:
                printf("Determinant(A) = %.4f\n", determinant(A, n));
                break;
            case 6:
                transposeInPlace(A, n);
                printf("Matrix A transposed in place:\n");
                printMatrix(A, n);
                break;
            case 7: {
                double eigenvalue, eigenvector[MAXN];
                powerIteration(A, n, 200, &eigenvalue, eigenvector);
                printf("Dominant eigenvalue (approx)               = %.4f\n", eigenvalue);
                printf("Corresponding eigenvector (approx, unit norm): ");
                for (int i = 0; i < n; i++) printf("%.4f ", eigenvector[i]);
                printf("\n");
                break;
            }
            case 8:
                printMatrix(A, n);
                break;
            case 9:
                inputMatrix(A, n, "A");
                break;
            case 0:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice.\n");
        }
    } while (choice != 0);

    return 0;
}
