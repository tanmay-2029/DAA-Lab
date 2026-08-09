# Design and Analysis of Algorithms (DAA) — Lab 01

**Course:** BTech (CS-B and CE), 3rd Semester
**Instructor:** Dr. Ajaya Kumar Dash
**Date:** July 28, 2026

This repository contains C implementations, Python plotting scripts, and
result graphs for all 6 questions of DAA Lab-01.

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
