/*
 * Q8: Optimal Binary Search Tree (OBST)
 * Lab-08, DAA  (follows the CLRS approach)
 *
 * Keys k1..kn have probability p[1..n]
 * Dummy keys d0..dn have probability q[0..n]
 *
 * e[i][j]    = expected cost of optimal BST containing keys ki..kj
 * w[i][j]    = sum of probabilities from ki..kj and d(i-1)..dj
 * root[i][j] = which key is the root of that optimal subtree
 *
 * Base: e[i][i-1] = q[i-1],  w[i][i-1] = q[i-1]
 * w[i][j] = w[i][j-1] + p[j] + q[j]
 * e[i][j] = min over r in i..j of ( e[i][r-1] + e[r+1][j] + w[i][j] )
 *
 * Time  : O(n^3)   (l, i, r loops)
 * Space : O(n^2)
 */
#include <stdio.h>
#include <math.h>

#define MAX 100
#define INF 1e18

double p[MAX + 2], q[MAX + 2];
double e[MAX + 3][MAX + 3], w[MAX + 3][MAX + 3];
int root[MAX + 3][MAX + 3];

// prints the tree: i..j is the range of keys in this subtree
void printTree(int i, int j, int parent, char side)
{
    if (i > j) {
        // empty subtree -> it is a dummy key d_j
        printf("  d%d is the %s child of k%d\n", j, (side == 'L') ? "left" : "right", parent);
        return;
    }
    int r = root[i][j];
    if (parent == 0)
        printf("  k%d is the root\n", r);
    else
        printf("  k%d is the %s child of k%d\n", r, (side == 'L') ? "left" : "right", parent);

    printTree(i, r - 1, r, 'L');
    printTree(r + 1, j, r, 'R');
}

int main()
{
    int n, i, j, l, r;

    printf("Enter number of keys n (1 to %d): ", MAX);
    scanf("%d", &n);
    if (n < 1 || n > MAX) {
        printf("Invalid n!\n");
        return 1;
    }

    printf("Enter probabilities p1..p%d (successful search): ", n);
    for (i = 1; i <= n; i++)
        scanf("%lf", &p[i]);

    printf("Enter probabilities q0..q%d (unsuccessful search): ", n);
    for (i = 0; i <= n; i++)
        scanf("%lf", &q[i]);

    // all probabilities should add up to 1
    double total = 0;
    for (i = 1; i <= n; i++) total += p[i];
    for (i = 0; i <= n; i++) total += q[i];
    if (fabs(total - 1.0) > 1e-6) {
        printf("Warning: all probabilities add up to %.4f, not 1.\n", total);
        printf("Still continuing with the calculation...\n");
    }

    // base cases
    for (i = 1; i <= n + 1; i++) {
        e[i][i - 1] = q[i - 1];
        w[i][i - 1] = q[i - 1];
    }

    // l = number of keys in the subtree
    for (l = 1; l <= n; l++) {
        for (i = 1; i <= n - l + 1; i++) {
            j = i + l - 1;
            e[i][j] = INF;
            w[i][j] = w[i][j - 1] + p[j] + q[j];
            for (r = i; r <= j; r++) {
                double t = e[i][r - 1] + e[r + 1][j] + w[i][j];
                if (t < e[i][j]) {
                    e[i][j] = t;
                    root[i][j] = r;
                }
            }
        }
    }

    printf("\nMinimum expected search cost = %.4f\n", e[1][n]);
    printf("\nStructure of the optimal BST:\n");
    printTree(1, n, 0, ' ');

    return 0;
}
