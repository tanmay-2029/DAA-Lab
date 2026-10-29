/*
 * Q8: Minimum Number of Meeting Rooms
 * -----------------------------------
 * Input : n, then n lines "start end". A meeting occupies [start, end), so a
 *         meeting ending at time t frees its room for one starting at t.
 *
 * Greedy (chronological sweep):
 *   Sort the start times and the end times separately. Walk through the
 *   starts in order; before each start, release every meeting whose end time
 *   is <= that start. rooms in use = starts seen - ends released.
 *   The answer is the maximum rooms in use at any moment, which is also a
 *   lower bound (that many meetings overlap at that moment), so it is optimal.
 *
 * Also shown: the actual room assignment using a min-heap of room end times
 * (give each meeting, in order of start time, the room that frees up earliest).
 *
 * Validation: brute force - for each meeting start s, count meetings with
 *   start <= s < end; the maximum count is the answer.
 *
 * Complexity:
 *   Sorting      : O(n log n)
 *   Sweep        : O(n)
 *   Assignment   : O(n log n) with the heap
 *   Brute force  : O(n^2)
 *   Space        : O(n)
 */
#include <stdio.h>
#include <stdlib.h>

typedef struct { int start, end, id, room; } Meeting;

static int cmpInt(const void *a, const void *b) {
    int x = *(const int *)a, y = *(const int *)b;
    return (x > y) - (x < y);
}

static int byStart(const void *a, const void *b) {
    const Meeting *x = a, *y = b;
    if (x->start != y->start) return (x->start > y->start) - (x->start < y->start);
    return x->id - y->id;
}

/* min-heap of (end time, room number) */
typedef struct { int end, room; } Slot;
static Slot *heap;
static int heapSize = 0;

static int slotLess(Slot a, Slot b) { return a.end != b.end ? a.end < b.end : a.room < b.room; }

static void push(Slot s) {
    int i = heapSize++;
    heap[i] = s;
    while (i > 0 && slotLess(heap[i], heap[(i - 1) / 2])) {
        Slot t = heap[i]; heap[i] = heap[(i - 1) / 2]; heap[(i - 1) / 2] = t;
        i = (i - 1) / 2;
    }
}

static Slot pop(void) {
    Slot top = heap[0];
    heap[0] = heap[--heapSize];
    int i = 0;
    for (;;) {
        int l = 2 * i + 1, r = l + 1, m = i;
        if (l < heapSize && slotLess(heap[l], heap[m])) m = l;
        if (r < heapSize && slotLess(heap[r], heap[m])) m = r;
        if (m == i) break;
        Slot t = heap[i]; heap[i] = heap[m]; heap[m] = t; i = m;
    }
    return top;
}

int main(void) {
    int n;
    printf("Enter number of meetings: ");
    if (scanf("%d", &n) != 1 || n < 1) { printf("Invalid input\n"); return 1; }
    Meeting *m = malloc(n * sizeof(Meeting));
    int *starts = malloc(n * sizeof(int)), *ends = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        printf("Meeting %d (start end): ", i + 1);
        scanf("%d %d", &m[i].start, &m[i].end);
        m[i].id = i + 1;
        starts[i] = m[i].start;
        ends[i] = m[i].end;
    }

    /* Sweep with two sorted arrays */
    qsort(starts, n, sizeof(int), cmpInt);
    qsort(ends, n, sizeof(int), cmpInt);
    int rooms = 0, inUse = 0, e = 0, peakTime = starts[0];
    for (int s = 0; s < n; s++) {
        while (e < n && ends[e] <= starts[s]) { e++; inUse--; }
        inUse++;
        if (inUse > rooms) { rooms = inUse; peakTime = starts[s]; }
    }
    printf("\nMinimum number of rooms = %d  (reached at time %d)\n", rooms, peakTime);

    /* Room assignment with a min-heap */
    qsort(m, n, sizeof(Meeting), byStart);
    heap = malloc(n * sizeof(Slot));
    int opened = 0;
    for (int i = 0; i < n; i++) {
        if (heapSize > 0 && heap[0].end <= m[i].start) {
            Slot s = pop();
            m[i].room = s.room;
        } else {
            m[i].room = ++opened;
        }
        push((Slot){m[i].end, m[i].room});
    }
    printf("\nRoom assignment:\n");
    for (int i = 0; i < n; i++)
        printf("  Meeting %d [%d, %d) -> Room %d\n", m[i].id, m[i].start, m[i].end, m[i].room);
    printf("Rooms used by assignment = %d (%s)\n", opened, opened == rooms ? "MATCH" : "MISMATCH");

    /* Brute force */
    int bf = 0;
    for (int i = 0; i < n; i++) {
        int c = 0;
        for (int j = 0; j < n; j++)
            if (m[j].start <= m[i].start && m[i].start < m[j].end) c++;
        if (c > bf) bf = c;
    }
    printf("Brute-force check        = %d (%s)\n", bf, bf == rooms ? "MATCH" : "MISMATCH");

    free(m); free(starts); free(ends); free(heap);
    return 0;
}
