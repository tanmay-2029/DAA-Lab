#include <stdio.h>

void sort(int arr[], int n)
{
    int i, j, temp;

    // Bubble sort
    for (i = 0; i < n - 1; i++)
    {
        for (j = 0; j < n - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

int binarySearch(int arr[], int n, int key)
{
    int low = 0;
    int high = n - 1;

    while (low <= high)
    {
        int mid = (low + high) / 2;

        if (arr[mid] == key)
            return 1;

        if (arr[mid] < key)
            low = mid + 1;
        else
            high = mid - 1;
    }

    return 0;
}

int main()
{
    int n, x;
    int i;

    printf("Enter size of sets: ");
    scanf("%d", &n);

    int S1[n], S2[n];

    printf("Enter elements of S1:\n");
    for (i = 0; i < n; i++)
        scanf("%d", &S1[i]);

    printf("Enter elements of S2:\n");
    for (i = 0; i < n; i++)
        scanf("%d", &S2[i]);

    printf("Enter x: ");
    scanf("%d", &x);

    // sort S1
    sort(S1, n);

    // check every element of S2
    for (i = 0; i < n; i++)
    {
        int required = x - S2[i];

        if (binarySearch(S1, n, required))
        {
            printf("Pair exists.\n");
            printf("%d + %d = %d\n", required, S2[i], x);
            return 0;
        }
    }

    printf("No such pair exists.\n");

    return 0;
}