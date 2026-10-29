/*
 * Q6: Reorganise String so equal characters are at least K apart
 * ---------------------------------------------------------------
 * Input : string S (no spaces) and integer K.
 *
 * Greedy with a max-heap and a cooldown queue:
 *   - Count the frequency of each character; put all characters in a
 *     max-heap keyed by remaining count (ties: smaller character first).
 *   - At each position pop the most frequent AVAILABLE character, append it,
 *     decrease its count and put it in a waiting queue.
 *   - Once a character has waited K-1 more positions (queue length reaches K)
 *     it goes back into the heap if its count is still positive.
 *   - If the heap is empty before the string is complete -> impossible,
 *     return "" (empty string).
 *   Placing the most frequent character as early as possible is optimal
 *   because it has the least slack.
 *
 * Validation:
 *   - Output is a permutation of S and same characters are >= K apart.
 *   - Feasibility check: with maxCount = highest frequency and numMax = number
 *     of characters having it, a solution exists iff
 *         (maxCount - 1) * K + numMax <= |S|      (for K >= 1)
 *
 * Complexity (n = |S|, A = alphabet size, here 256):
 *   Counting      : O(n)
 *   Main loop     : O(n log A) (one heap operation per character placed)
 *   Space         : O(n + A)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define A 256

static int cnt[A];
static int heap[A], heapSize = 0;

static int higher(int a, int b) {   /* a has priority over b */
    if (cnt[a] != cnt[b]) return cnt[a] > cnt[b];
    return a < b;
}

static void push(int c) {
    int i = heapSize++;
    heap[i] = c;
    while (i > 0 && higher(heap[i], heap[(i - 1) / 2])) {
        int t = heap[i]; heap[i] = heap[(i - 1) / 2]; heap[(i - 1) / 2] = t;
        i = (i - 1) / 2;
    }
}

static int pop(void) {
    int top = heap[0];
    heap[0] = heap[--heapSize];
    int i = 0;
    for (;;) {
        int l = 2 * i + 1, r = l + 1, m = i;
        if (l < heapSize && higher(heap[l], heap[m])) m = l;
        if (r < heapSize && higher(heap[r], heap[m])) m = r;
        if (m == i) break;
        int t = heap[i]; heap[i] = heap[m]; heap[m] = t; i = m;
    }
    return top;
}

/* Writes the result into out; returns 1 on success, 0 if impossible. */
static int reorganise(const char *s, int K, char *out) {
    int n = strlen(s);
    if (K <= 1) { strcpy(out, s); return 1; }

    memset(cnt, 0, sizeof cnt);
    heapSize = 0;
    for (int i = 0; i < n; i++) cnt[(unsigned char)s[i]]++;
    for (int c = 0; c < A; c++) if (cnt[c]) push(c);

    int *queue = malloc((n + 1) * sizeof(int));
    int qHead = 0, qTail = 0, len = 0;
    while (len < n) {
        if (heapSize == 0) { free(queue); out[0] = '\0'; return 0; }
        int c = pop();
        out[len++] = (char)c;
        cnt[c]--;
        queue[qTail++] = c;
        if (qTail - qHead >= K) {          /* waited K-1 positions */
            int back = queue[qHead++];
            if (cnt[back] > 0) push(back);
        }
    }
    out[n] = '\0';
    free(queue);
    return 1;
}

static int isValid(const char *s, const char *out, int K) {
    int n = strlen(s), c1[A] = {0}, c2[A] = {0}, last[A];
    if ((int)strlen(out) != n) return 0;
    for (int i = 0; i < n; i++) { c1[(unsigned char)s[i]]++; c2[(unsigned char)out[i]]++; }
    for (int c = 0; c < A; c++) { if (c1[c] != c2[c]) return 0; last[c] = -1000000; }
    for (int i = 0; i < n; i++) {
        unsigned char c = out[i];
        if (i - last[c] < K) return 0;
        last[c] = i;
    }
    return 1;
}

static int feasible(const char *s, int K) {
    int n = strlen(s), f[A] = {0}, maxCount = 0, numMax = 0;
    if (K <= 1) return 1;
    for (int i = 0; i < n; i++) f[(unsigned char)s[i]]++;
    for (int c = 0; c < A; c++) if (f[c] > maxCount) maxCount = f[c];
    for (int c = 0; c < A; c++) if (f[c] == maxCount) numMax++;
    return (long long)(maxCount - 1) * K + numMax <= n;
}

int main(void) {
    char S[10001];
    int K;
    printf("Enter string S: ");
    if (scanf("%10000s", S) != 1) return 1;
    printf("Enter distance K: ");
    if (scanf("%d", &K) != 1) return 1;

    char *out = malloc(strlen(S) + 1);
    int ok = reorganise(S, K, out);

    printf("\nResult: \"%s\"\n", out);
    if (ok) printf("Valid (permutation, equal chars >= %d apart): %s\n", K, isValid(S, out, K) ? "YES" : "NO");
    else printf("Impossible to rearrange -> empty string\n");

    int f = feasible(S, K);
    printf("Feasibility formula says: %s  (%s)\n", f ? "possible" : "impossible",
           f == ok ? "MATCH" : "MISMATCH");

    free(out);
    return 0;
}
