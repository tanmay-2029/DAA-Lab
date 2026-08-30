# Q4 — Heap Sort of N Randomly Generated Elements Stored in a File

Implement Heap Sort to sort N randomly generated elements stored in a
file, and analyze the complexity of the algorithm.

## Workflow

1. Generate N random integers and write them to `data/input.txt`.
2. Read the N integers back from `data/input.txt` into an array.
3. Build a max-heap, then repeatedly extract the maximum to produce a
   sorted array (classic in-place Heap Sort).
4. Write the sorted array to `data/output.txt` and print it.

## Build & Run

```bash
gcc -Wall -O2 -o heapsort_file heapsort_file.c
./heapsort_file
```

Example:

```
Enter number of random elements to generate (N): 10
Generated 10 random numbers into data/input.txt
Unsorted (from file): 8669 6110 6707 6131 2423 8753 1286 1239 1579 7176
Sorted: 1239 1286 1579 2423 6110 6131 6707 7176 8669 8753
Sorted numbers written to data/output.txt
```

## Complexity Analysis

**Build-heap phase:** heapifying all N/2 internal nodes bottom-up is a
tight **O(N)** (amortized), not O(N log N) — most nodes sit near the
bottom of the heap and need very few sift-down swaps.

**Extraction phase:** N extract-max operations, each costing O(log N) to
re-heapify after swapping the root with the last element → **O(N log N)**.

| Case | Time |
|---|---|
| Best / Average / Worst | **O(N log N)** — no bad-input worst case, unlike Quick Sort |
| Space | O(1) auxiliary — sorts in-place |

Heap Sort is **not stable**, but its O(N log N) bound holds regardless of
input distribution — that's its main advantage over Quick Sort's O(N²)
worst case.
