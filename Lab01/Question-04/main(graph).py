# Assignment 4 - Towers of Hanoi Plot
# Name: Alex Chen
# Roll No: 10240391

import matplotlib.pyplot as plt

n_val = []
moves_val = []

# Read CSV file manually
f = open("hanoi_moves.csv", "r")
lines = f.readlines()
f.close()

# Skip header line
for line in lines[1:]:
    parts = line.strip().split(",")
    n_val.append(int(parts[0]))
    moves_val.append(int(parts[1]))

# Create two plots side by side
plt.figure(figsize=(10, 4))

# First plot: Linear scale
plt.subplot(1, 2, 1)
plt.plot(n_val, moves_val, marker="o", color="purple")
plt.xlabel("Number of Discs (n)")
plt.ylabel("Total Moves")
plt.title("Linear Scale")
plt.grid(True)

# Second plot: Log scale
plt.subplot(1, 2, 2)
plt.plot(n_val, moves_val, marker="o", color="purple")
plt.yscale("log")
plt.xlabel("Number of Discs (n)")
plt.ylabel("Total Moves (log)")
plt.title("Log Scale")
plt.grid(True)

plt.tight_layout()
plt.savefig("hanoi_graph.png")
print("Graph saved successfully as hanoi_graph.png!")
