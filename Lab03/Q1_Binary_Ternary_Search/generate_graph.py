"""
Reads data.csv (produced by `./binary_ternary_search --bench N`) and plots
average comparisons vs n for binary search and ternary search.

Usage:
    ./binary_ternary_search --bench 200000 > data.csv
    python3 generate_graph.py
"""
import csv
import matplotlib.pyplot as plt

ns, binc, terc = [], [], []
with open("data.csv") as f:
    reader = csv.DictReader(f)
    for row in reader:
        ns.append(int(row["n"]))
        binc.append(float(row["binary_comparisons"]))
        terc.append(float(row["ternary_comparisons"]))

plt.figure(figsize=(8, 5))
plt.plot(ns, binc, marker="o", label="Binary Search")
plt.plot(ns, terc, marker="s", label="Ternary Search")
plt.xscale("log", base=2)
plt.xlabel("Number of elements (n, log scale)")
plt.ylabel("Average number of comparisons")
plt.title("Binary Search vs Ternary Search: Comparisons vs n")
plt.legend()
plt.grid(True, which="both", linestyle="--", alpha=0.5)
plt.tight_layout()
plt.savefig("graph.png", dpi=150)
print("Saved graph.png")
