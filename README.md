# Design and Analysis of Algorithms (DAA) — Lab 01

**Course:** BTech (CS-B and CE), 3rd Semester
**Instructor:** Dr. Ajaya Kumar Dash
**Date:** July 28, 2026

This repository contains C implementations, Python plotting scripts, and
result graphs for all 6 questions of DAA Lab-01.

## Repository structure

```
DAA-Lab01/
├── Makefile                        # build/run everything at once
├── Q1_order_of_growth/
│   ├── order_of_growth.c           # ranks the 12 functions by growth rate
│   ├── plot_growth.py              # plots f(n) vs n (log scale)
│   └── growth_of_functions.png
├── Q2_coin_toss/
│   ├── coin_toss.c                 # fair vs biased coin simulation
│   ├── plot_coin.py                # plots convergence of P(HEAD)
│   └── coin_convergence.png
├── Q3_bubble_sort/
│   ├── bubble_sort.c               # optimised vs naive bubble sort
│   ├── plot_bubble.py              # plots comparisons vs n
│   └── bubble_sort_comparisons.png
├── Q4_towers_of_hanoi/
│   ├── hanoi.c                     # ToH simulation + move counting
│   ├── plot_hanoi.py               # plots moves vs discs (linear & log)
│   └── hanoi_moves.png
├── Q5_partition_point/
│   ├── partition_point.c           # linear scan vs binary search
│   ├── plot_partition.py           # plots comparisons vs n
│   └── partition_point_comparisons.png
├── Q6_element_uniqueness/
│   ├── element_uniqueness.c        # brute force vs sort-then-scan
│   ├── plot_uniqueness.py          # plots comparisons/time vs n
│   └── uniqueness_analysis.png
└── README.md
```

Each question's `.c` program, when run, also writes a `.csv` file with the
raw data it collected; the matching `plot_*.py` script reads that CSV and
produces the `.png` graph shown below. Every program prints its own
concluding analysis to the terminal.

## Requirements

- `gcc` (any recent version, C99+)
- `python3` with `matplotlib` (`pip install matplotlib --break-system-packages`)
- `make` (optional, but convenient)

## How to build and run everything

```bash
make run-all
```

This compiles every C program, runs it (generating the CSVs), and runs the
matching Python script (generating the PNGs). To do it manually for a
single question, `cd` into its folder and run, e.g.:

```bash
cd Q3_bubble_sort
gcc -O2 -o bubble_sort bubble_sort.c
./bubble_sort
python3 plot_bubble.py
```

---

## Q1 — Put them in Order

**Task:** Place the 12 given functions in increasing order of growth for
sufficiently large `n`.

| `n log₂n` | `12√n`      | `1/n`     | `n^(log₂n)` |
|---|---|---|---|
| `100n²+6n` | `n^0.51`   | `n²−324` | `50n^0.5`  |
| `2n³`     | `3ⁿ`        | `2³²·n`   | `log₂n`     |

**Theoretical increasing order (Θ-classes):**

```
1/n  <  log2(n)  <  sqrt(n) [12√n, 50n^0.5]  <  n^0.51
     <  n [2^32·n]  <  n·log2(n)  <  n^2 [n^2-324, 100n^2+6n]
     <  n^3 [2n^3]  <  n^(log2 n)  <  3^n
```

- Terms in the *same* asymptotic class (e.g. `12√n` and `50n^0.5`, or
  `n²−324` and `100n²+6n`) are ordered among themselves by their leading
  constant, since Θ-notation alone can't separate them.
- **The trick term is `2³²·n`.** `2³² ≈ 4.29×10⁹` is a huge but *fixed*
  constant, so this term is still Θ(n) — not exponential. Because the
  constant is so large, it will *numerically* dominate `n²`, `n³`, even
  `nˡᵒᵍ²ⁿ` for any n you can realistically compute, and only drops below
  `n·log₂n` once `n > 2^(2^32)`. This program prints both the
  **empirical** ranking (at a computable `n`) and the **theoretical**
  ranking, and explains the discrepancy — it's a deliberate test of the
  "sufficiently large n" clause in the definition of asymptotic growth.

`order_of_growth.c` evaluates every function over `n = 2..60`, dumps the
values to `growth_values.csv`, and prints both rankings. `plot_growth.py`
plots all 12 curves on a single log-scale graph.

![Q1 growth curves](Q1_order_of_growth/growth_of_functions.png)

---

## Q2 — Fair vs Biased coin

**Task:** Simulate coin tosses in C and show `P(HEAD) ≈ 0.5` for a fair
coin; extend to compare fair vs. biased coins.

`coin_toss.c` tosses a simulated coin `n` times (default `n = 100000`),
tracking the running observed `P(HEAD)`, for both a fair coin (`p=0.5`)
and a biased coin (`p` given on the command line, default `0.7`). It logs
the running average to `fair_coin.csv` / `biased_coin.csv`.

**Result:** by the Law of Large Numbers, the observed frequency converges
to the true probability as the number of tosses grows — visible below as
both curves flattening out at their respective `p`.

![Q2 coin convergence](Q2_coin_toss/coin_convergence.png)

---

## Q3 — Performance analysis of bubble sort

**Task:** Implement (i) bubble sort with early termination and (ii)
bubble sort that always runs all `n-1` passes; compare comparison counts.

`bubble_sort.c` runs both versions on the same randomised array for sizes
`n = 10..2000` and logs comparison counts to
`bubble_sort_comparisons.csv`. It also runs a **best-case demo** on an
already-sorted array of size 1000.

**Result:** on random data, both versions are Θ(n²) — comparisons scale
almost identically, since a swap is likely on nearly every pass. On
already-sorted data, though, the optimised version terminates after a
single pass (`n-1` comparisons, Θ(n)) while the naive version still does
the full `n(n-1)/2` comparisons (Θ(n²)) regardless.

![Q3 bubble sort comparisons](Q3_bubble_sort/bubble_sort_comparisons.png)

---

## Q4 — Towers of Hanoi

**Task:** Simulate ToH in C, plot total moves vs. `n` discs, and draw a
conclusion.

`hanoi.c` recursively solves ToH, printing the full move sequence for a
small `n` (default 4) and counting total moves for `n = 1..25`, logged to
`hanoi_moves.csv`.

**Result:** `T(n) = 2ⁿ − 1`, confirmed numerically. The plot's log-scale
panel is a straight line — the signature of exponential growth — showing
ToH is a Θ(2ⁿ) algorithm: each additional disc *doubles* the work.

![Q4 Hanoi moves](Q4_towers_of_hanoi/hanoi_moves.png)

---

## Q5 — Find the partition point

**Task:** Array `A` has a run of 0's followed by a run of 1's; find the
exact transition point.

`partition_point.c` implements:
- **Linear scan** — O(n) worst case.
- **Binary search** — exploits the fact that the array is already sorted
  (monotonically non-decreasing 0's then 1's), finding the transition in
  O(log n).

Comparisons for both are logged across `n = 100..102400` (doubling each
step) to `partition_point_comparisons.csv`.

**Result:** the plot makes the O(n) vs O(log n) gap dramatic — linear
scan's comparisons grow proportionally with `n`, while binary search's
barely increase at all.

![Q5 partition point](Q5_partition_point/partition_point_comparisons.png)

---

## Q6 — Element uniqueness

**Task:** For `n` random numbers, check for duplicates; conclude on
efficiency for large `n`.

`element_uniqueness.c` implements:
- **Brute force** — compare every pair, O(n²) worst case.
- **Sort-then-scan** — sort (O(n log n)), then a single linear pass
  checks adjacent elements for equality — O(n log n) total.

To measure the true worst case fairly, the benchmark array is a random
**permutation of `0..n-1`** (guaranteed unique), so brute force can't
early-exit on a lucky early duplicate. Comparisons and wall-clock timings
for `n = 500..8000` are logged to `uniqueness_timing.csv`.

**Result:** brute-force comparisons grow quadratically (matches
`n(n-1)/2`); its running time visibly outpaces the near-linear-looking
sort-then-scan curve as `n` grows — confirming O(n²) vs O(n log n).

![Q6 element uniqueness](Q6_element_uniqueness/uniqueness_analysis.png)

---

## Author

Prepared for DAA Lab-01 submission (BTech CS-B / CE, 3rd Semester).
