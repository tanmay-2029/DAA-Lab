#include <stdio.h>
#include <stdlib.h>

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

/* Choose k-1 elements */
int findSum(int arr[], int n, int k, int index,
            int count, int sum, int T)
{
    int i;

    // We have selected k-1 elements
    if (count == k - 1)
    {
        int required = T - sum;

        if (binarySearch(arr, n, required))
            return 1;

        return 0;
    }

    for (i = index; i < n; i++)
    {
        if (findSum(arr, n, k, i + 1,
                    count + 1, sum + arr[i], T))
        {
            return 1;
        }
    }

    return 0;
}

int main()
{
    int n, k, T;
    int i;

    printf("Enter n: ");
    scanf("%d", &n);

    int S[n];

    printf("Enter elements:\n");

    for (i = 0; i < n; i++)
        scanf("%d", &S[i]);

    printf("Enter k: ");
    scanf("%d", &k);

    printf("Enter T: ");
    scanf("%d", &T);

    // Sort the set
    sort(S, n);

    if (findSum(S, n, k, 0, 0, 0, T))
        printf("Yes, %d elements add up to %d.\n", k, T);
    else
        printf("No, %d elements do not add up to %d.\n", k, T);

    return 0;
}