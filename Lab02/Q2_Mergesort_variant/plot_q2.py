import csv
import math
import matplotlib.pyplot as plt

ns, t2, t3 = [], [], []
with open("results_q2.csv") as f:
    reader = csv.DictReader(f)
    for row in reader:
        ns.append(int(row["n"]))
        t2.append(float(row["mergesort2_ms"]))
        t3.append(float(row["mergesort3_ms"]))

# reference n log n curve, scaled to match mergesort2 at the largest n
nlogn = [n * math.log2(n) for n in ns]
scale = t2[-1] / nlogn[-1]
ref = [scale * v for v in nlogn]

# ---- linear-scale comparison ----
plt.figure(figsize=(8, 6))
plt.plot(ns, t2, marker="o", label="Merge Sort (2-way)")
plt.plot(ns, t3, marker="s", label="Modified Merge Sort (3-way)")
plt.plot(ns, ref, linestyle="--", color="gray", label="c·n·log₂(n) reference")
plt.xlabel("n (array size)")
plt.ylabel("Time (ms)")
plt.title("Merge Sort vs Modified 3-way Merge Sort")
plt.legend()
plt.grid(True, alpha=0.3)
plt.tight_layout()
plt.savefig("mergesort_comparison.png", dpi=150)
print("saved mergesort_comparison.png")

# ---- log-log to confirm Theta(n log n) growth shape ----
plt.figure(figsize=(8, 6))
plt.plot(ns, t2, marker="o", label="Merge Sort (2-way)")
plt.plot(ns, t3, marker="s", label="Modified Merge Sort (3-way)")
plt.plot(ns, ref, linestyle="--", color="gray", label="c·n·log₂(n) reference")
plt.xscale("log")
plt.yscale("log")
plt.xlabel("n (array size, log scale)")
plt.ylabel("Time (ms, log scale)")
plt.title("Merge Sort vs Modified 3-way Merge Sort (log-log)")
plt.legend()
plt.grid(True, which="both", alpha=0.3)
plt.tight_layout()
plt.savefig("mergesort_loglog.png", dpi=150)
print("saved mergesort_loglog.png")
