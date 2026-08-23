#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int start;
    int end;
} Interval;

// Compare intervals according to starting time
int compare(const void *a, const void *b)
{
    Interval *i1 = (Interval *)a;
    Interval *i2 = (Interval *)b;

    return i1->start - i2->start;
}

int main()
{
    int n, i;

    printf("Enter number of intervals: ");
    scanf("%d", &n);

    Interval arr[n];

    printf("Enter intervals:\n");

    for (i = 0; i < n; i++)
    {
        scanf("%d %d", &arr[i].start, &arr[i].end);
    }

    // Sort intervals according to start time
    qsort(arr, n, sizeof(Interval), compare);

    Interval result[n];
    int k = 0;

    // Start with first interval
    result[0] = arr[0];
    k = 1;

    for (i = 1; i < n; i++)
    {
        // Intervals overlap
        if (arr[i].start <= result[k - 1].end)
        {
            if (arr[i].end > result[k - 1].end)
            {
                result[k - 1].end = arr[i].end;
            }
        }
        else
        {
            // No overlap
            result[k] = arr[i];
            k++;
        }
    }

    printf("\nMerged intervals:\n");

    for (i = 0; i < k; i++)
    {
        printf("(%d, %d) ", result[i].start, result[i].end);
    }

    printf("\n");

    return 0;
}