# Q1 — Median of N Numbers Without Sorting

Find the median of a list of N numbers **without sorting** the list, and
analyze the complexity of the algorithm.

## Approach

Randomized **Quickselect**. The median is the (N/2)-th order statistic (or
the average of the two middle order statistics when N is even). Quickselect
partitions the array like Quick Sort, but recurses into only the side that
contains the target rank, instead of sorting the whole array.

## Build & Run

```bash
gcc -Wall -O2 -o median median.c
./median
```

Example:

```
Enter number of elements (N): 7
Enter 7 numbers:
5 3 8 1 9 2 7
Median = 5.00
```

## Complexity Analysis

| Case | Time |
|---|---|
| Average / Expected | **O(N)** |
| Worst case | O(N²) (rare — occurs if the pivot is always the smallest/largest element) |
| Space | O(1) auxiliary, O(log N) expected recursion depth |

Recurrence (average case): `T(N) = T(N/2) + O(N)` → `T(N) = O(N)`.

This beats the naive approach of sorting first (O(N log N)) since we only
need one order statistic, not a fully sorted array. A deterministic O(N)
**worst-case** guarantee is possible using the median-of-medians pivot
selection strategy, at the cost of a larger constant factor.
