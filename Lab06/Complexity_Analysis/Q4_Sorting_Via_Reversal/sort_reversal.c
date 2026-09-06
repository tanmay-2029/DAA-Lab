/*
 * Q4: Sorting a Permutation via reverse(p, i, j)
 *
 * Approach 1 - "O(n) reversals" (selection-sort by reversal):
 *   For i = 0 .. n-2: find the position `pos` of value (i+1) in p[i..n-1]
 *   and call reverse(p, i, pos). This places the correct value at index i
 *   using exactly one reversal per position, i.e. at most n-1 reversals
 *   overall -- proving O(n) reversals always suffice to sort any
 *   permutation (see ANALYSIS.md for the full proof).
 *
 * Approach 2 - O(n log^2 n) COST algorithm:
 *   Standard merge sort, but instead of merging into an auxiliary array we
 *   merge two adjacent sorted blocks p[l..mid-1] and p[mid..r] IN PLACE
 *   using a "rotate" operation built from 3 reversals. Binary search finds
 *   the split points so that at most one rotation of size (mid-l)+(r-mid+1)
 *   is needed at the top of each merge, and the algorithm recurses only
 *   into the (typically much smaller) remaining pieces.
 *   A single merge of a block of size s costs O(s log s); summed across
 *   O(log n) merge-sort levels this gives a total reversal COST of
 *   O(n log^2 n), while the reverse() primitive itself is still just an
 *   O(length) array reversal.
 */
#include <stdio.h>
#include <stdlib.h>

long long totalCost = 0;
long long totalReversals = 0;

void reverseRange(int p[], int i, int j) {
    if (i >= j) return;
    totalReversals++;
    totalCost += (j - i + 1);
    while (i < j) {
        int t = p[i]; p[i] = p[j]; p[j] = t;
        i++; j--;
    }
}

/* rotate p[l..r] so the block [mid..r] moves before [l..mid-1], via 3 reversals.
 * Cost incurred = (r - l + 1) [three reversals but cost only counted once,
 * matching the standard "block rotation via reversal" trick].
 */
void rotateBlock(int p[], int l, int mid, int r) {
    if (l >= mid || mid > r) return;
    reverseRange(p, l, mid - 1);
    reverseRange(p, mid, r);
    reverseRange(p, l, r);
}

/* ---------------- Approach 1: O(n) reversals ---------------- */
void sortWithLinearReversals(int p[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int pos = i;
        for (int k = i; k < n; k++)
            if (p[k] == i + 1) { pos = k; break; }
        reverseRange(p, i, pos);
    }
}

/* ---------------- Approach 2: O(n log^2 n) cost ---------------- */
/* In-place merge of two sorted blocks p[l..mid-1] and p[mid..r] using
 * binary search + a single rotation, recursing into the smaller pieces. */
void inPlaceMerge(int p[], int l, int mid, int r) {
    if (l >= mid || mid > r) return;
    if (p[mid - 1] <= p[mid]) return; /* already ordered - nothing to do */

    /* find l1 in [l, mid): first index whose value > p[mid] */
    int l1 = l, h1 = mid;
    while (l1 < h1) {
        int m1 = (l1 + h1) / 2;
        if (p[m1] <= p[mid]) l1 = m1 + 1; else h1 = m1;
    }

    /* find l2 in [mid, r]: first index whose value >= p[l1] */
    int l2 = mid, h2 = r + 1;
    while (l2 < h2) {
        int m2 = (l2 + h2) / 2;
        if (p[l1] <= p[m2]) h2 = m2; else l2 = m2 + 1;
    }

    rotateBlock(p, l1, mid, l2 - 1);

    int newMid = l1 + (l2 - mid);
    inPlaceMerge(p, l, l1, newMid);
    inPlaceMerge(p, newMid + 1, l2, r);
}

void mergeSortByRotation(int p[], int l, int r) {
    if (r - l < 1) return;
    int mid = l + (r - l) / 2;
    mergeSortByRotation(p, l, mid);
    mergeSortByRotation(p, mid + 1, r);
    inPlaceMerge(p, l, mid + 1, r);
}

void printArray(int p[], int n) {
    for (int i = 0; i < n; i++) printf("%d ", p[i]);
    printf("\n");
}

int isSorted(int p[], int n) {
    for (int i = 1; i < n; i++) if (p[i - 1] > p[i]) return 0;
    return 1;
}

int main(void) {
    int n;
    printf("=== Sorting a Permutation via Reversals ===\n");
    printf("Enter n (size of the permutation of 1..n): ");
    scanf("%d", &n);

    int *p = malloc(n * sizeof(int));
    printf("Enter the permutation (n distinct integers from 1 to n):\n");
    for (int i = 0; i < n; i++) scanf("%d", &p[i]);

    int *p1 = malloc(n * sizeof(int));
    int *p2 = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) p1[i] = p2[i] = p[i];

    printf("\n--- Approach 1: Selection-sort by reversal (O(n) reversals) ---\n");
    totalCost = 0; totalReversals = 0;
    sortWithLinearReversals(p1, n);
    printf("Sorted array : "); printArray(p1, n);
    printf("Reversals used : %lld  (bound: O(n))\n", totalReversals);
    printf("Total cost     : %lld\n", totalCost);
    printf("Correctness    : %s\n", isSorted(p1, n) ? "SORTED CORRECTLY" : "ERROR - NOT SORTED");

    printf("\n--- Approach 2: Merge sort via rotation (O(n log^2 n) cost) ---\n");
    totalCost = 0; totalReversals = 0;
    mergeSortByRotation(p2, 0, n - 1);
    printf("Sorted array : "); printArray(p2, n);
    printf("Reversals used : %lld\n", totalReversals);
    printf("Total cost     : %lld  (bound: O(n log^2 n))\n", totalCost);
    printf("Correctness    : %s\n", isSorted(p2, n) ? "SORTED CORRECTLY" : "ERROR - NOT SORTED");

    free(p); free(p1); free(p2);
    return 0;
}
