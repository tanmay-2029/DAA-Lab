/*
 * Q2: Huffman Coding with canonical codebook
 * ------------------------------------------
 * Input : n, then n lines "symbol frequency" (symbol = one character).
 *
 * Algorithm:
 *   1. Put every symbol in a min-heap keyed by frequency.
 *   2. Repeat n-1 times: remove the two smallest nodes x, y and insert a new
 *      node with frequency f(x) + f(y) whose children are x and y.
 *   3. The depth of each leaf is its code length.
 *   4. Canonical code: sort symbols by (length, symbol). The first code is all
 *      zeros; each next code = (previous code + 1) shifted left by the
 *      increase in length. Codes are then lexicographically ordered by length.
 *
 * Validation:
 *   - Kraft sum  sum 2^(-len_i) must equal 1 (full binary tree).
 *   - The code must be prefix-free (checked pairwise).
 *   - Total cost sum f_i*len_i must equal the sum of all merge costs.
 *
 * Complexity:
 *   Building the tree : O(n log n) (2(n-1) heap removals, n-1 insertions)
 *   Code lengths      : O(n)
 *   Canonical codes   : O(n log n) for sorting
 *   Prefix check      : O(n^2) (validation only)
 *   Space             : O(n)
 */
#include <stdio.h>
#include <stdlib.h>

#define MAXN 256

typedef struct {
    long long freq;
    int left, right;   /* -1 for leaves */
    char symbol;
} Node;

typedef struct {
    char symbol;
    long long freq;
    int length;
    unsigned long long code;
} Code;

static Node nodes[2 * MAXN];
static int heap[2 * MAXN], heapSize = 0;

static int less(int a, int b) {
    if (nodes[a].freq != nodes[b].freq) return nodes[a].freq < nodes[b].freq;
    return a < b;   /* deterministic tie-break */
}

static void push(int x) {
    int i = heapSize++;
    heap[i] = x;
    while (i > 0 && less(heap[i], heap[(i - 1) / 2])) {
        int p = (i - 1) / 2, t = heap[i];
        heap[i] = heap[p]; heap[p] = t; i = p;
    }
}

static int pop(void) {
    int top = heap[0];
    heap[0] = heap[--heapSize];
    int i = 0;
    for (;;) {
        int l = 2 * i + 1, r = l + 1, s = i;
        if (l < heapSize && less(heap[l], heap[s])) s = l;
        if (r < heapSize && less(heap[r], heap[s])) s = r;
        if (s == i) break;
        int t = heap[i]; heap[i] = heap[s]; heap[s] = t; i = s;
    }
    return top;
}

static void assignLengths(int v, int depth, Code codes[], int *count) {
    if (nodes[v].left == -1) {
        codes[*count].symbol = nodes[v].symbol;
        codes[*count].freq = nodes[v].freq;
        codes[*count].length = depth == 0 ? 1 : depth;   /* single symbol gets "0" */
        (*count)++;
        return;
    }
    assignLengths(nodes[v].left, depth + 1, codes, count);
    assignLengths(nodes[v].right, depth + 1, codes, count);
}

static int byLengthThenSymbol(const void *a, const void *b) {
    const Code *x = a, *y = b;
    if (x->length != y->length) return x->length - y->length;
    return (unsigned char)x->symbol - (unsigned char)y->symbol;
}

static void printCode(const Code *c) {
    for (int b = c->length - 1; b >= 0; b--) putchar((c->code >> b) & 1 ? '1' : '0');
}

int main(void) {
    int n;
    printf("Enter number of symbols (1..%d): ", MAXN);
    if (scanf("%d", &n) != 1 || n < 1 || n > MAXN) { printf("Invalid input\n"); return 1; }

    for (int i = 0; i < n; i++) {
        printf("Symbol %d and frequency: ", i + 1);
        scanf(" %c %lld", &nodes[i].symbol, &nodes[i].freq);
        nodes[i].left = nodes[i].right = -1;
        push(i);
    }

    int total = n;
    long long mergeCost = 0;
    while (heapSize > 1) {
        int x = pop(), y = pop();
        nodes[total].freq = nodes[x].freq + nodes[y].freq;
        nodes[total].left = x;
        nodes[total].right = y;
        mergeCost += nodes[total].freq;
        push(total++);
    }
    int root = pop();

    Code codes[MAXN];
    int count = 0;
    assignLengths(root, 0, codes, &count);
    qsort(codes, count, sizeof(Code), byLengthThenSymbol);

    codes[0].code = 0;
    for (int i = 1; i < count; i++)
        codes[i].code = (codes[i - 1].code + 1) << (codes[i].length - codes[i - 1].length);

    long long freqSum = 0, cost = 0;
    printf("\nCanonical Huffman codebook:\n");
    printf("%8s %10s %8s   %s\n", "symbol", "freq", "length", "code");
    for (int i = 0; i < count; i++) {
        printf("%8c %10lld %8d   ", codes[i].symbol, codes[i].freq, codes[i].length);
        printCode(&codes[i]);
        printf("\n");
        freqSum += codes[i].freq;
        cost += codes[i].freq * codes[i].length;
    }

    printf("\nTotal encoded length  = %lld bits\n", cost);
    printf("Expected code length  = %.4f bits/symbol\n", (double)cost / freqSum);

    /* Validation */
    double kraft = 0;
    for (int i = 0; i < count; i++) kraft += 1.0 / (double)(1ULL << codes[i].length);

    int prefixFree = 1;
    for (int i = 0; i < count && prefixFree; i++)
        for (int j = i + 1; j < count && prefixFree; j++)   /* len_i <= len_j after sort */
            if ((codes[j].code >> (codes[j].length - codes[i].length)) == codes[i].code)
                prefixFree = 0;

    printf("\nKraft sum             = %.6f (%s)\n", kraft, (n == 1 || kraft == 1.0) ? "OK" : "FAIL");
    printf("Prefix-free           = %s\n", prefixFree ? "YES" : "NO");
    if (n > 1)
        printf("Sum of merge costs    = %lld (%s)\n", mergeCost, mergeCost == cost ? "MATCH" : "MISMATCH");
    return 0;
}
