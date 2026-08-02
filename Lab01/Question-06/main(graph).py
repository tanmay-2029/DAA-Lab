import matplotlib.pyplot as plt

n_vals = []
comp_list = []
brute_times = []
sorted_times = []

f = open("uniqueness_timing.csv", "r")
lines = f.readlines()
f.close()

for line in lines[1:]:
    parts = line.strip().split(",")
    n_vals.append(int(parts[0]))
    comp_list.append(int(parts[1]))
    brute_times.append(float(parts[2]))
    sorted_times.append(float(parts[3]))

plt.figure(figsize=(12, 5))

# 1: comparisons for brute Force
plt.subplot(1, 2, 1)
plt.plot(n_vals, comp_list, marker="o", color="brown")
plt.xlabel("Array Size (n)")
plt.ylabel("Comparisons")
plt.title("Brute Force Comparisons O(n^2)")
plt.grid(True)

# 2: time Comparison
plt.subplot(1, 2, 2)
plt.plot(n_vals, brute_times, marker="o", label="Brute Force O(n^2)", color="red")
plt.plot(n_vals, sorted_times, label="Sort-then-scan O(n log n)", marker="o", color="blue")
plt.xlabel("Array Size (n)")
plt.ylabel("Time (seconds)")
plt.title("Execution Time Comparison")
plt.legend()
plt.grid(True)

plt.tight_layout()
plt.savefig("uniqueness_analysis.png")
print("Graph saved successfully as uniqueness_analysis.png")
