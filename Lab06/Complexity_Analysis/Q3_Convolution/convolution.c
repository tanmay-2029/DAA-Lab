/*
 * Q3: Convolution of two vectors using a Divide & Conquer algorithm - O(n log n)
 *
 * C[k] = sum_{j=0}^{m-1} A[j] * B[k-j],   for vectors A (length m), B (length n), n >= m
 *
 * Direct evaluation of the convolution sum costs O(m*n). We instead use the
 * Divide & Conquer FFT (Cooley-Tukey) algorithm:
 *
 *   1. Zero-pad A and B to length N = next power of two >= (m + n - 1).
 *   2. Compute FFT(A) and FFT(B), each in O(N log N) via the recursive
 *      divide-and-conquer FFT.
 *   3. Point-wise multiply the transformed vectors in O(N).
 *   4. Compute the inverse FFT in O(N log N) to recover C.
 *
 * Total time complexity  : O(N log N) = O(n log n)   [N = Theta(n)]
 * Total space complexity : O(n)
 *
 * A brute-force O(m*n) convolution is also included, purely to verify
 * correctness of the divide-and-conquer result.
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct { double re, im; } Complex;

Complex cadd(Complex a, Complex b) { Complex r = { a.re + b.re, a.im + b.im }; return r; }
Complex csub(Complex a, Complex b) { Complex r = { a.re - b.re, a.im - b.im }; return r; }
Complex cmul(Complex a, Complex b) {
    Complex r = { a.re * b.re - a.im * b.im, a.re * b.im + a.im * b.re };
    return r;
}

/* Recursive (divide & conquer) FFT - Cooley-Tukey. n must be a power of 2.
 * T(n) = 2*T(n/2) + O(n)  =>  T(n) = O(n log n)
 */
void fft(Complex *a, int n, int invert) {
    if (n == 1) return;

    Complex *even = malloc((n / 2) * sizeof(Complex));
    Complex *odd  = malloc((n / 2) * sizeof(Complex));
    for (int i = 0; i < n / 2; i++) {
        even[i] = a[2 * i];
        odd[i]  = a[2 * i + 1];
    }

    fft(even, n / 2, invert);
    fft(odd, n / 2, invert);

    double angle = 2 * M_PI / n * (invert ? -1 : 1);
    Complex w = { 1, 0 };
    Complex wn = { cos(angle), sin(angle) };

    for (int i = 0; i < n / 2; i++) {
        Complex t = cmul(w, odd[i]);
        a[i] = cadd(even[i], t);
        a[i + n / 2] = csub(even[i], t);
        if (invert) {
            a[i].re /= 2; a[i].im /= 2;
            a[i + n / 2].re /= 2; a[i + n / 2].im /= 2;
        }
        w = cmul(w, wn);
    }

    free(even);
    free(odd);
}

int nextPowerOf2(int x) {
    int p = 1;
    while (p < x) p <<= 1;
    return p;
}

/* Brute-force O(m*n) convolution, used only to verify the D&C result. */
void naiveConvolution(double *A, int m, double *B, int n, double *C) {
    for (int k = 0; k < m + n - 1; k++) {
        C[k] = 0;
        for (int j = 0; j < m; j++)
            if (k - j >= 0 && k - j < n)
                C[k] += A[j] * B[k - j];
    }
}

int main(void) {
    int m, n;
    printf("=== Convolution of Two Vectors via Divide & Conquer FFT - O(n log n) ===\n");
    printf("Enter length of vector A (m): ");
    scanf("%d", &m);
    double *A = malloc(m * sizeof(double));
    printf("Enter %d elements of A:\n", m);
    for (int i = 0; i < m; i++) scanf("%lf", &A[i]);

    printf("Enter length of vector B (n), with n >= m: ");
    scanf("%d", &n);
    if (n < m) { printf("Error: this implementation expects n >= m.\n"); free(A); return 1; }
    double *B = malloc(n * sizeof(double));
    printf("Enter %d elements of B:\n", n);
    for (int i = 0; i < n; i++) scanf("%lf", &B[i]);

    int resultLen = m + n - 1;
    int size = nextPowerOf2(resultLen);

    Complex *fa = calloc(size, sizeof(Complex));
    Complex *fb = calloc(size, sizeof(Complex));
    for (int i = 0; i < m; i++) fa[i].re = A[i];
    for (int i = 0; i < n; i++) fb[i].re = B[i];

    fft(fa, size, 0);
    fft(fb, size, 0);
    for (int i = 0; i < size; i++) fa[i] = cmul(fa[i], fb[i]);
    fft(fa, size, 1);

    printf("\nConvolution result C (Divide & Conquer FFT), length %d:\n", resultLen);
    for (int i = 0; i < resultLen; i++)
        printf("C[%d] = %.4f\n", i, fa[i].re);

    double *naiveC = malloc(resultLen * sizeof(double));
    naiveConvolution(A, m, B, n, naiveC);

    int match = 1;
    for (int i = 0; i < resultLen; i++)
        if (fabs(fa[i].re - naiveC[i]) > 1e-3) { match = 0; break; }

    printf("\nVerification against brute-force O(m*n) convolution: %s\n", match ? "MATCH" : "MISMATCH");
    printf("Time Complexity  : O(n log n)  [after zero-padding to next power of 2]\n");
    printf("Space Complexity : O(n)\n");

    free(A); free(B); free(fa); free(fb); free(naiveC);
    return 0;
}
