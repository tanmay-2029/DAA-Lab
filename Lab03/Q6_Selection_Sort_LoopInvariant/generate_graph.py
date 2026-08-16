"""
Reads data.csv (produced by `./selection_sort --bench N`) and plots
comparisons vs n for random, already-sorted, and reverse-sorted
inputs, alongside the theoretical n(n-1)/2 curve -- showing that
selection sort's comparison count is input-independent (Theta(n^2)
in both best and worst case).

Usage:
    ./selection_sort --bench 12800 > data.csv
    python3 generate_graph.py
"""
import csv
import matplotlib.pyplot as plt

ns, rand_c, sorted_c, rev_c = [], [], [], []
with open("data.csv") as f:
    reader = csv.DictReader(f)
    for row in reader:
        ns.append(int(row["n"]))
        rand_c.append(int(row["comparisons_random"]))
        sorted_c.append(int(row["comparisons_sorted"]))
        rev_c.append(int(row["comparisons_reverse"]))

theoretical = [n * (n - 1) / 2 for n in ns]

plt.figure(figsize=(8, 5))
plt.plot(ns, rand_c, marker="o", label="Random input")
plt.plot(ns, sorted_c, marker="s", label="Already sorted input")
plt.plot(ns, rev_c, marker="^", label="Reverse sorted input")
plt.plot(ns, theoretical, linestyle="--", color="black", label="Theoretical n(n-1)/2")
plt.xlabel("Number of elements (n)")
plt.ylabel("Number of comparisons")
plt.title("Selection Sort: Comparisons vs n (Theta(n^2) regardless of input)")
plt.legend()
plt.grid(True, linestyle="--", alpha=0.5)
plt.tight_layout()
plt.savefig("graph.png", dpi=150)
print("Saved graph.png")
