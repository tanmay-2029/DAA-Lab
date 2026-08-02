# Assignment 3 - Bubble Sort Comparison Plot
# Name: Alex Chen
# Roll No: 10240391

import matplotlib.pyplot as plt

# lists for data
n_list = []
opt_list = []
naive_list = []

# open file
f = open("bubble_sort_comparisons.csv", "r")
lines = f.readlines()
f.close()

# skip header line
for line in lines[1:]:
    parts = line.strip().split(",")
    n_list.append(int(parts[0]))
    opt_list.append(int(parts[1]))
    naive_list.append(int(parts[2]))

# plot curves
plt.plot(n_list, opt_list, label="Optimized Bubble Sort", color="green")
plt.plot(n_list, naive_list, label="Naive Bubble Sort", color="red", linestyle="--")

plt.xlabel("Array Size (n)")
plt.ylabel("Number of Comparisons")
plt.title("Bubble Sort: Optimized vs Naive Performance")
plt.legend()
plt.grid(True)

# save plot
plt.savefig("bubble_sort_graph.png")
print("Plot successfully saved as bubble_sort_graph.png!")
