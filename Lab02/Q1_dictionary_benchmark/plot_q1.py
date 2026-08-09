import csv
from collections import defaultdict
import matplotlib.pyplot as plt

rows = defaultdict(lambda: defaultdict(list))  # rows[structure][metric] = [(n, t), ...]

with open("results_q1.csv") as f:
    reader = csv.DictReader(f)
    for row in reader:
        s = row["structure"]
        n = int(row["n"])
        for metric in ("insert_ms", "search_ms", "delete_ms", "max_ms"):
            rows[s][metric].append((n, float(row[metric])))

pretty = {
    "unsorted_array": "Unsorted Array",
    "sorted_array": "Sorted Array",
    "singly_linked_unsorted": "Singly Linked (Unsorted)",
    "singly_linked_sorted": "Singly Linked (Sorted)",
    "doubly_linked_unsorted": "Doubly Linked (Unsorted)",
    "doubly_linked_sorted": "Doubly Linked (Sorted)",
}

metric_title = {
    "insert_ms": "Insert",
    "search_ms": "Search",
    "delete_ms": "Delete",
    "max_ms": "Max",
}

for metric in ("insert_ms", "search_ms", "delete_ms", "max_ms"):
    plt.figure(figsize=(8, 6))
    for structure, series in rows.items():
        pts = sorted(series[metric])
        xs = [p[0] for p in pts]
        ys = [p[1] for p in pts]
        plt.plot(xs, ys, marker="o", label=pretty[structure])
    plt.xlabel("n (number of elements)")
    plt.ylabel("Time (ms)")
    plt.title(f"{metric_title[metric]} — Empirical Growth vs n")
    plt.legend(fontsize=8)
    plt.grid(True, alpha=0.3)
    plt.tight_layout()
    out = f"{metric.replace('_ms', '')}_growth.png"
    plt.savefig(out, dpi=150)
    print("saved", out)
