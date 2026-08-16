"""
Reads data.csv (produced by `./maxmin --bench N`) and plots the actual
number of comparisons against the theoretical 3n/2 bound.

Usage:
    ./maxmin --bench 200000 > data.csv
    python3 generate_graph.py
"""
import csv
import matplotlib.pyplot as plt

ns, comps, bounds = [], [], []
with open("data.csv") as f:
    reader = csv.DictReader(f)
    for row in reader:
        ns.append(int(row["n"]))
        comps.append(int(row["comparisons"]))
        bounds.append(float(row["bound_3n_over_2"]))

plt.figure(figsize=(8, 5))
plt.plot(ns, comps, marker="o", label="Actual comparisons (D&C Max-Min)")
plt.plot(ns, bounds, linestyle="--", label="Theoretical bound 3n/2")
plt.xscale("log", base=2)
plt.xlabel("Number of elements (n, log scale)")
plt.ylabel("Number of comparisons")
plt.title("Divide & Conquer Max-Min: Comparisons vs n")
plt.legend()
plt.grid(True, which="both", linestyle="--", alpha=0.5)
plt.tight_layout()
plt.savefig("graph.png", dpi=150)
print("Saved graph.png")
