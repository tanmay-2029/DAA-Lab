import matplotlib.pyplot as plt

#lists for data
n_val = []
linear_comp = []
binary_comp = []

#open file manually
f = open("partition_point_comparisons.csv", "r")
lines = f.readlines()
f.close()

#skip csv header line
for line in lines[1:]:
    parts = line.strip().split(",")
    n_val.append(int(parts[0]))
    linear_comp.append(int(parts[1]))
    binary_comp.append(int(parts[2]))

#plot comparison
plt.plot(n_val, linear_comp, marker="o", label="Linear Search O(n)", color="red")
plt.plot(n_val, binary_comp, marker="o", label="Binary Search O(log n)", color="blue")

plt.xlabel("Array Size (n)")
plt.ylabel("Number of Comparisons")
plt.title("Partition Point Search: Linear vs Binary Search")
plt.legend()
plt.grid(True)

#save output image
plt.savefig("partition_point_comparisons.png")
print("Graph saved successfully as partition_point_comparisons.png")
