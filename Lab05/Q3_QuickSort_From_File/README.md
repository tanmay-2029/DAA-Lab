# Q3 — Quick Sort of N Random Elements Stored in a File

Implement Quick Sort of N random elements stored in a file.

## Workflow

1. Generate N random integers and write them to `data/input.txt`.
2. Read the N integers back from `data/input.txt` into an array.
3. Quick Sort the array in-place (Lomuto partition scheme).
4. Write the sorted array to `data/output.txt` and print it.

## Build & Run

```bash
gcc -Wall -O2 -o quicksort_file quicksort_file.c
./quicksort_file
```

Example:

```
Enter number of random elements to generate (N): 10
Generated 10 random numbers into data/input.txt
Unsorted (from file): 1695 2516 4152 6714 4853 2995 7471 8916 7041 4276
Sorted: 1695 2516 2995 4152 4276 4853 6714 7041 7471 8916
Sorted numbers written to data/output.txt
```

## Complexity Analysis

| Case | Time | When it happens |
|---|---|---|
| Best / Average | **O(N log N)** | Balanced partitions |
| Worst | O(N²) | Already-sorted / reverse-sorted input with a poor pivot choice |
| Space | O(log N) avg, O(N) worst (recursion stack) |

Recurrence (average case): `T(N) = 2T(N/2) + O(N)` → `T(N) = O(N log N)` by
the Master Theorem. Since the input here is randomly generated, the O(N²)
worst case is extremely unlikely in practice.
