# Q2 — K'th Smallest Element Without Sorting

Find the K'th smallest element in a list of N numbers **without sorting**
the list, and analyze the complexity of the algorithm.

## Approach

Randomized **Quickselect**, generalized from Q1 to an arbitrary rank K
(K=1 → smallest element, K=N → largest element) instead of only the
median.

## Build & Run

```bash
gcc -Wall -O2 -o kth_smallest kth_smallest.c
./kth_smallest
```

Example:

```
Enter number of elements (N): 7
Enter 7 numbers:
5 3 8 1 9 2 7
Enter K (1 <= K <= N): 3
3-th smallest element = 3
```

## Complexity Analysis

| Case | Time |
|---|---|
| Average / Expected | **O(N)** |
| Worst case | O(N²) |
| Space | O(1) auxiliary, O(log N) expected recursion depth |

This is asymptotically better than sorting first (O(N log N)) and then
indexing, whenever only a single order statistic is needed rather than the
full sorted order.
