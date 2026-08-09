import csv
import math
import matplotlib.pyplot as plt

# ---------------- Experiment A: fixed n, vary k ----------------
ks, seq_k, pair_k = [], [], []
with open("results_q3_vary_k.csv") as f:
    for row in csv.DictReader(f):
        ks.append(int(row["k"]))
        seq_k.append(float(row["sequential_ms"]))
        pair_k.append(float(row["pairwise_ms"]))

n_fixed = 500
k2_ref = [k * k for k in ks]
scale_k2 = seq_k[-1] / k2_ref[-1]
k2_ref = [scale_k2 * v for v in k2_ref]

klogk_ref = [k * math.log2(k) for k in ks]
scale_klogk = pair_k[-1] / klogk_ref[-1]
klogk_ref = [scale_klogk * v for v in klogk_ref]

plt.figure(figsize=(8, 6))
plt.plot(ks, seq_k, marker="o", label="Method 1: Sequential (O(n·k²))")
plt.plot(ks, pair_k, marker="s", label="Method 2: Pairwise (O(n·k·log k))")
plt.plot(ks, k2_ref, linestyle="--", color="lightcoral", label="c·k² reference")
plt.plot(ks, klogk_ref, linestyle="--", color="gray", label="c·k·log₂k reference")
plt.xlabel(f"k (number of arrays, n = {n_fixed} fixed)")
plt.ylabel("Time (ms)")
plt.title(f"Merging k Sorted Arrays of size n={n_fixed}: Method 1 vs Method 2")
plt.legend()
plt.grid(True, alpha=0.3)
plt.tight_layout()
plt.savefig("vary_k.png", dpi=150)
print("saved vary_k.png")

plt.figure(figsize=(8, 6))
plt.plot(ks, seq_k, marker="o", label="Method 1: Sequential (O(n·k²))")
plt.plot(ks, pair_k, marker="s", label="Method 2: Pairwise (O(n·k·log k))")
plt.xscale("log")
plt.yscale("log")
plt.xlabel(f"k (log scale, n = {n_fixed} fixed)")
plt.ylabel("Time (ms, log scale)")
plt.title("Method 1 vs Method 2 (log-log) — slope reveals polynomial degree")
plt.legend()
plt.grid(True, which="both", alpha=0.3)
plt.tight_layout()
plt.savefig("vary_k_loglog.png", dpi=150)
print("saved vary_k_loglog.png")

# ---------------- Experiment B: fixed k, vary n ----------------
ns, seq_n, pair_n = [], [], []
with open("results_q3_vary_n.csv") as f:
    for row in csv.DictReader(f):
        ns.append(int(row["n"]))
        seq_n.append(float(row["sequential_ms"]))
        pair_n.append(float(row["pairwise_ms"]))

k_fixed = 16
plt.figure(figsize=(8, 6))
plt.plot(ns, seq_n, marker="o", label="Method 1: Sequential")
plt.plot(ns, pair_n, marker="s", label="Method 2: Pairwise")
plt.xlabel(f"n (size of each array, k = {k_fixed} fixed)")
plt.ylabel("Time (ms)")
plt.title(f"Merging k={k_fixed} Sorted Arrays: Both Methods are Linear in n")
plt.legend()
plt.grid(True, alpha=0.3)
plt.tight_layout()
plt.savefig("vary_n.png", dpi=150)
print("saved vary_n.png")
