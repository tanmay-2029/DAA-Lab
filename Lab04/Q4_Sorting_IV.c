#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int time;
    int type;       // 1 = entry, -1 = exit
} Event;

int compare(const void *a, const void *b)
{
    Event *e1 = (Event *)a;
    Event *e2 = (Event *)b;

    return e1->time - e2->time;
}

int main()
{
    int n, i;

    printf("Enter number of persons: ");
    scanf("%d", &n);

    Event events[2 * n];

    printf("Enter entry and exit time for each person:\n");

    for (i = 0; i < n; i++)
    {
        int a, b;

        printf("Person %d: ", i + 1);
        scanf("%d %d", &a, &b);

        events[2 * i].time = a;
        events[2 * i].type = 1;

        events[2 * i + 1].time = b;
        events[2 * i + 1].type = -1;
    }

    // Sort all events according to time
    qsort(events, 2 * n, sizeof(Event), compare);

    int count = 0;
    int maxPeople = 0;
    int maxTime = 0;

    // Scan the events
    for (i = 0; i < 2 * n; i++)
    {
        count = count + events[i].type;

        if (count > maxPeople)
        {
            maxPeople = count;
            maxTime = events[i].time;
        }
    }

    printf("\nMaximum number of people = %d", maxPeople);
    printf("\nTime when maximum people were present = %d\n", maxTime);

    return 0;
}