/*
 * Q6: Edit Distance with Traceback
 * Lab-08, DAA
 *
 * D[i][j] = min operations to convert A[0..i-1] into B[0..j-1]
 *   D[i][0] = i   (delete everything)
 *   D[0][j] = j   (insert everything)
 *   if A[i-1] == B[j-1]  ->  D[i][j] = D[i-1][j-1]
 *   else  D[i][j] = 1 + min( D[i-1][j]    (delete),
 *                            D[i][j-1]    (insert),
 *                            D[i-1][j-1]  (substitute) )
 *
 * Traceback: start at D[m][n] and walk back to D[0][0],
 * at every cell check which neighbour we came from.
 *
 * Time  : O(m * n)
 * Space : O(m * n)
 */
#include <stdio.h>
#include <string.h>

#define MAX 1001

int D[MAX][MAX];

int min3(int a, int b, int c)
{
    int m = a;
    if (b < m) m = b;
    if (c < m) m = c;
    return m;
}

void readLine(char s[])
{
    fgets(s, MAX, stdin);
    int len = strlen(s);
    if (len > 0 && s[len - 1] == '\n')
        s[len - 1] = '\0';
}

int main()
{
    char A[MAX], B[MAX];
    int m, n, i, j;

    printf("Enter string A: ");
    readLine(A);
    printf("Enter string B: ");
    readLine(B);

    m = strlen(A);
    n = strlen(B);

    for (i = 0; i <= m; i++) {
        for (j = 0; j <= n; j++) {
            if (i == 0)
                D[i][j] = j;
            else if (j == 0)
                D[i][j] = i;
            else if (A[i - 1] == B[j - 1])
                D[i][j] = D[i - 1][j - 1];
            else
                D[i][j] = 1 + min3(D[i - 1][j], D[i][j - 1], D[i - 1][j - 1]);
        }
    }

    printf("\nMinimum edit distance = %d\n", D[m][n]);

    // ---- traceback ----
    // ops are found from the end, so store them and print reversed
    char op[2 * MAX];     // 'M' match, 'S' substitute, 'I' insert, 'D' delete
    char ca[2 * MAX];     // char of A involved
    char cb[2 * MAX];     // char of B involved
    int steps = 0;

    i = m;
    j = n;
    while (i > 0 || j > 0) {
        if (i > 0 && j > 0 && A[i - 1] == B[j - 1] && D[i][j] == D[i - 1][j - 1]) {
            op[steps] = 'M'; ca[steps] = A[i - 1]; cb[steps] = B[j - 1];
            i--; j--;
        } else if (i > 0 && j > 0 && D[i][j] == D[i - 1][j - 1] + 1) {
            op[steps] = 'S'; ca[steps] = A[i - 1]; cb[steps] = B[j - 1];
            i--; j--;
        } else if (i > 0 && D[i][j] == D[i - 1][j] + 1) {
            op[steps] = 'D'; ca[steps] = A[i - 1]; cb[steps] = '-';
            i--;
        } else {
            op[steps] = 'I'; ca[steps] = '-'; cb[steps] = B[j - 1];
            j--;
        }
        steps++;
    }

    printf("\nTraceback (A -> B):\n");
    int k, num = 1;
    for (k = steps - 1; k >= 0; k--) {
        if (op[k] == 'M')
            printf("  Keep        '%c'\n", ca[k]);
        else if (op[k] == 'S')
            printf("%2d. Substitute '%c' with '%c'\n", num++, ca[k], cb[k]);
        else if (op[k] == 'D')
            printf("%2d. Delete     '%c'\n", num++, ca[k]);
        else
            printf("%2d. Insert     '%c'\n", num++, cb[k]);
    }

    // alignment view
    printf("\nAlignment:\n  A: ");
    for (k = steps - 1; k >= 0; k--) printf("%c", ca[k]);
    printf("\n  B: ");
    for (k = steps - 1; k >= 0; k--) printf("%c", cb[k]);
    printf("\n  ('-' means a gap)\n");

    return 0;
}
