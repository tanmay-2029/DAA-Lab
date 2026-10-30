/*
 * DAA Lab-11, Q3: Bidirectional BFS on a word graph
 *
 * Words are nodes, two words are connected if they differ in exactly one
 * letter. We never build the edge list (it can be O(N^2)). Instead, for a word
 * we try every position and every letter 'a'..'z' (25 other letters) and look
 * the new word up in a hash table -> at most 25*L neighbours per word.
 *
 * Bidirectional BFS: grow one BFS from A and one from B, always expand the side
 * with the smaller frontier, one full level at a time. Stop when they touch.
 *
 * Rules I assumed (the sheet says it depends on the input constraints):
 *   - A does NOT have to be in the dictionary, B must be.
 *   - A == B  -> answer 0
 *
 * Input format:
 *   N L
 *   word_1 ... word_N      (one per line)
 *   A B
 *
 * Output: minimum number of mutations, or -1
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXL 10

static char *words;
static int *table;
static unsigned mask;
static int L;

static unsigned hash_word(const char *s)
{
    unsigned h = 2166136261u;
    for (int i = 0; i < L; i++) {
        h ^= (unsigned char)s[i];
        h *= 16777619u;
    }
    return h;
}

static int lookup(const char *s)
{
    unsigned slot = hash_word(s) & mask;
    while (table[slot] != -1) {
        if (memcmp(words + (size_t)table[slot] * (MAXL + 1), s, L) == 0)
            return table[slot];
        slot = (slot + 1) & mask;
    }
    return -1;
}

static void insert(int id)
{
    unsigned slot = hash_word(words + (size_t)id * (MAXL + 1)) & mask;
    while (table[slot] != -1) slot = (slot + 1) & mask;
    table[slot] = id;
}

static int expand(int *front, int fsize, int *next,
                  int *mine, int *other, int *best)
{
    int nsize = 0;
    char buf[MAXL + 2];

    for (int i = 0; i < fsize; i++) {
        int u = front[i];
        memcpy(buf, words + (size_t)u * (MAXL + 1), L);
        buf[L] = '\0';

        for (int pos = 0; pos < L; pos++) {
            char orig = buf[pos];

            for (char c = 'a'; c <= 'z'; c++) {
                if (c == orig) continue;

                buf[pos] = c;
                int v = lookup(buf);

                if (v == -1 || mine[v] != -1) continue;

                if (other[v] != -1) {
                    int cand = mine[u] + 1 + other[v];
                    if (*best == -1 || cand < *best)
                        *best = cand;
                }
                else {
                    mine[v] = mine[u] + 1;
                    next[nsize++] = v;
                }
            }
            buf[pos] = orig;
        }
    }
    return nsize;
}

int main(void)
{
    int N;

    printf("Enter number of words and word length: ");
    scanf("%d %d", &N, &L);

    if (N < 0 || L < 1 || L > MAXL) {
        printf("Invalid input\n");
        return 1;
    }

    int cap = N + 1;

    words = calloc((size_t)cap * (MAXL + 1), 1);

    unsigned size = 1;
    while (size < (unsigned)(2 * cap)) size <<= 1;

    mask = size - 1;
    table = malloc(sizeof(int) * size);

    for (unsigned i = 0; i < size; i++)
        table[i] = -1;

    int cnt = 0;
    char buf[64];

    printf("Enter %d dictionary words:\n", N);

    for (int i = 0; i < N; i++) {
        scanf("%63s", buf);

        if ((int)strlen(buf) != L) {
            printf("Invalid word length\n");
            return 1;
        }

        if (lookup(buf) != -1) continue;

        memcpy(words + (size_t)cnt * (MAXL + 1), buf, L);
        insert(cnt);
        cnt++;
    }

    char A[64], B[64];

    printf("Enter starting word A and destination word B: ");
    scanf("%63s %63s", A, B);

    if ((int)strlen(A) != L || (int)strlen(B) != L) {
        printf("Invalid A/B length\n");
        return 1;
    }

    int b = lookup(B);

    if (b == -1) {
        printf("-1\n");
        return 0;
    }

    if (strcmp(A, B) == 0) {
        printf("0\n");
        return 0;
    }

    int a = lookup(A);

    if (a == -1) {
        memcpy(words + (size_t)cnt * (MAXL + 1), A, L);
        insert(cnt);
        a = cnt++;
    }

    int *distF = malloc(sizeof(int) * cnt);
    int *distB = malloc(sizeof(int) * cnt);

    int *fF = malloc(sizeof(int) * cnt);
    int *fB = malloc(sizeof(int) * cnt);
    int *tmp = malloc(sizeof(int) * cnt);

    for (int i = 0; i < cnt; i++)
        distF[i] = distB[i] = -1;

    distF[a] = 0;
    fF[0] = a;
    int sF = 1;

    distB[b] = 0;
    fB[0] = b;
    int sB = 1;

    int best = -1;

    while (sF > 0 && sB > 0 && best == -1) {
        if (sF <= sB) {
            sF = expand(fF, sF, tmp, distF, distB, &best);
            int *t = fF;
            fF = tmp;
            tmp = t;
        }
        else {
            sB = expand(fB, sB, tmp, distB, distF, &best);
            int *t = fB;
            fB = tmp;
            tmp = t;
        }
    }

    printf("Minimum mutations: %d\n", best);

    free(words);
    free(table);
    free(distF);
    free(distB);
    free(fF);
    free(fB);
    free(tmp);

    return 0;
}