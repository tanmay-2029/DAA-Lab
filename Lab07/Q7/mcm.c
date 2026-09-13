/*
 * Q7 - Matrix Chain Multiplication (MCM)
 * -----------------------------------------
 * Name   : Tanmay
 * Course : BTech (CS-B / CE), 3rd Semester
 * Lab    : DAA Lab-07
 *
 * PROBLEM:
 * We are given a chain of matrices A1, A2, ..., Am to multiply.
 * Matrix multiplication is associative, so we can put the brackets
 * ( ) anywhere we like, but the TOTAL NUMBER of scalar multiplications
 * needed changes a lot depending on where we place them.
 * We want the bracket placement that needs the FEWEST scalar
 * multiplications.
 *
 * INPUT REPRESENTATION:
 * Instead of giving all the matrix dimensions individually, we give
 * one array p[] of size (m+1), where matrix Ai has dimensions
 * p[i-1] x p[i]. (This is the standard/compact way to represent a
 * chain of matrices.)
 *
 * DP IDEA:
 * Let dp[i][j] = minimum scalar multiplications needed to multiply
 * the chain of matrices Ai...Aj (i <= j).
 *
 * Base case: dp[i][i] = 0 (a single matrix needs no multiplication).
 *
 * For i < j, we try EVERY possible place "k" to split the chain into
 * (Ai..Ak) and (Ak+1..Aj):
 *      cost(k) = dp[i][k] + dp[k+1][j] + p[i-1]*p[k]*p[j]
 * (the last term is the cost of multiplying the two resulting
 *  matrices of size p[i-1] x p[k] and p[k] x p[j])
 *
 * dp[i][j] = minimum of cost(k) over all valid k (i <= k < j)
 *
 * We also remember WHICH k gave the best split (in split[i][j]) so
 * that we can print the actual optimal parenthesization at the end.
 *
 * COMPLEXITY:
 *   There are O(m^2) states (i,j pairs).
 *   Each state tries O(m) values of k.
 *   TOTAL TIME  = O(m^3)
 *   SPACE       = O(m^2)  for the dp and split tables.
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int dp[50][50];
int split[50][50];

/* print the optimal parenthesization using the split[][] table */
void printParenthesization(int i, int j) {
    if (i == j) {
        printf("A%d", i);
        return;
    }
    printf("(");
    printParenthesization(i, split[i][j]);
    printParenthesization(split[i][j] + 1, j);
    printf(")");
}

int main(void) {
    int m; /* number of matrices in the chain */
    int i, j, k, len;

    printf("=== Q7: Matrix Chain Multiplication ===\n");
    printf("Enter number of matrices in the chain: ");
    scanf("%d", &m);

    if (m <= 0 || m > 45) {
        printf("Please enter a valid number of matrices (1 to 45).\n");
        return 0;
    }

    int p[50]; /* p[0..m], matrix Ai has size p[i-1] x p[i] */
    printf("Enter the %d dimensions (p0 p1 ... p%d):\n", m + 1, m);
    printf("(Matrix A_i will have size p[i-1] x p[i])\n");
    for (i = 0; i <= m; i++) {
        scanf("%d", &p[i]);
    }

    /* base case: single matrix chains cost 0 */
    for (i = 1; i <= m; i++) {
        dp[i][i] = 0;
    }

    /* build up chains of increasing length */
    for (len = 2; len <= m; len++) {
        for (i = 1; i <= m - len + 1; i++) {
            j = i + len - 1;
            dp[i][j] = INT_MAX;

            for (k = i; k < j; k++) {
                int cost = dp[i][k] + dp[k + 1][j] + p[i - 1] * p[k] * p[j];
                if (cost < dp[i][j]) {
                    dp[i][j] = cost;
                    split[i][j] = k;
                }
            }
        }
    }

    printf("\n---------- RESULT ----------\n");
    printf("Minimum scalar multiplications needed : %d\n", dp[1][m]);
    printf("Optimal parenthesization               : ");
    printParenthesization(1, m);
    printf("\n-----------------------------\n");

    printf("\nSanity check: classic textbook example\n");
    printf("dims = 30 35 15 5 10 20 25 (6 matrices) should give 15125\n");
    {
        int testP[7] = {30, 35, 15, 5, 10, 20, 25};
        int tm = 6;
        int tdp[50][50], tsplit[50][50];
        for (i = 1; i <= tm; i++) tdp[i][i] = 0;
        for (len = 2; len <= tm; len++) {
            for (i = 1; i <= tm - len + 1; i++) {
                j = i + len - 1;
                tdp[i][j] = INT_MAX;
                for (k = i; k < j; k++) {
                    int cost = tdp[i][k] + tdp[k + 1][j] + testP[i - 1] * testP[k] * testP[j];
                    if (cost < tdp[i][j]) {
                        tdp[i][j] = cost;
                        tsplit[i][j] = k;
                    }
                }
            }
        }
        printf("This program computes: %d\n", tdp[1][tm]);
    }

    return 0;
}
