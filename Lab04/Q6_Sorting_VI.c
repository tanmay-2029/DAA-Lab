#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int point;
    int type;       // 1 = start, -1 = end
} Event;

// Sort events
int compare(const void *a, const void *b)
{
    Event *e1 = (Event *)a;
    Event *e2 = (Event *)b;

    // Sort by point
    if (e1->point != e2->point)
        return e1->point - e2->point;

    // If same point, start (+1) comes before end (-1)
    return e2->type - e1->type;
}

int main()
{
    int n, i;

    printf("Enter number of intervals: ");
    scanf("%d", &n);

    Event events[2 * n];

    printf("Enter intervals (left right):\n");

    for (i = 0; i < n; i++)
    {
        int l, r;

        scanf("%d %d", &l, &r);

        events[2 * i].point = l;
        events[2 * i].type = 1;

        events[2 * i + 1].point = r;
        events[2 * i + 1].type = -1;
    }

    // Sort all events
    qsort(events, 2 * n, sizeof(Event), compare);

    int count = 0;
    int maxCount = 0;
    int bestPoint = 0;

    // Sweep through all events
    for (i = 0; i < 2 * n; i++)
    {
        count = count + events[i].type;

        if (count > maxCount)
        {
            maxCount = count;
            bestPoint = events[i].point;
        }
    }

    printf("\nPoint = %d\n", bestPoint);
    printf("Maximum number of intervals = %d\n", maxCount);

    return 0;
}