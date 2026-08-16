"""
Reads data.csv (produced by `./defective_coin --bench N`) and plots
the number of weighings used vs n, alongside floor(log2 n) to show
the algorithm stays within log2(n) + c weighings.

Usage:
    ./defective_coin --bench 200000 > data.csv
    python3 generate_graph.py
"""
import csv
import matplotlib.pyplot as plt

ns, weighings, logbound = [], [], []
with open("data.csv") as f:
    reader = csv.DictReader(f)
    for row in reader:
        ns.append(int(row["n"]))
        weighings.append(int(row["weighings"]))
        logbound.append(int(row["log2_n_floor"]))

plt.figure(figsize=(8, 5))
plt.plot(ns, weighings, marker="o", label="Actual weighings used")
plt.plot(ns, logbound, linestyle="--", label="floor(log2 n)")
plt.xscale("log", base=2)
plt.xlabel("Number of coins (n, log scale)")
plt.ylabel("Number of weighings")
plt.title("Defective Coin Search: Weighings vs n (D&C balance-scale algorithm)")
plt.legend()
plt.grid(True, which="both", linestyle="--", alpha=0.5)
plt.tight_layout()
plt.savefig("graph.png", dpi=150)
print("Saved graph.png")
