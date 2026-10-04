/*
 * Q3: Longest Common Subsequence (LCS)
 * Lab-08, DAA
 *
 * L[i][j] = length of LCS of X[0..i-1] and Y[0..j-1]
 *   if X[i-1] == Y[j-1]  ->  L[i][j] = L[i-1][j-1] + 1
 *   else                 ->  L[i][j] = max(L[i-1][j], L[i][j-1])
 *
 * Then we go back from L[m][n] to build the actual string.
 *
 * Time  : O(m * n)
 * Space : O(m * n)
 */
#include <stdio.h>
#include <string.h>

#define MAX 1001

int L[MAX][MAX];

int max(int a, int b)
{
    return (a > b) ? a : b;
}

// reads a line and removes the newline at the end
void readLine(char s[])
{
    fgets(s, MAX, stdin);
    int len = strlen(s);
    if (len > 0 && s[len - 1] == '\n')
        s[len - 1] = '\0';
}

int main()
{
    char X[MAX], Y[MAX];
    char result[MAX];
    int m, n, i, j, k;

    printf("Enter first sequence X (max %d chars): ", MAX - 1);
    readLine(X);
    printf("Enter second sequence Y (max %d chars): ", MAX - 1);
    readLine(Y);

    m = strlen(X);
    n = strlen(Y);

    // first row and first column are 0 (empty string)
    for (i = 0; i <= m; i++) {
        for (j = 0; j <= n; j++) {
            if (i == 0 || j == 0)
                L[i][j] = 0;
            else if (X[i - 1] == Y[j - 1])
                L[i][j] = L[i - 1][j - 1] + 1;
            else
                L[i][j] = max(L[i - 1][j], L[i][j - 1]);
        }
    }

    // traceback to get the string
    k = L[m][n];
    result[k] = '\0';
    i = m;
    j = n;
    while (i > 0 && j > 0) {
        if (X[i - 1] == Y[j - 1]) {
            result[--k] = X[i - 1];
            i--;
            j--;
        } else if (L[i - 1][j] >= L[i][j - 1]) {
            i--;
        } else {
            j--;
        }
    }

    printf("\nLength of LCS = %d\n", L[m][n]);
    printf("LCS string    = \"%s\"\n", result);

    return 0;
}
