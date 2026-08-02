import matplotlib.pyplot as plt

n_val = []
moves_val = []

f = open("hanoi_moves.csv", "r")
lines = f.readlines()
f.close()

# skip header line
for line in lines[1:]:
    parts = line.strip().split(",")
    n_val.append(int(parts[0]))
    moves_val.append(int(parts[1]))

# create two plots side by side
plt.figure(figsize=(10, 4))

# 1st plot: Linear scale
plt.subplot(1, 2, 1)
plt.plot(n_val, moves_val, marker="o", color="purple")
plt.xlabel("Number of Discs (n)")
plt.ylabel("Total Moves")
plt.title("Linear Scale")
plt.grid(True)

# 2nd plot: Log scale
plt.subplot(1, 2, 2)
plt.plot(n_val, moves_val, marker="o", color="purple")
plt.yscale("log")
plt.xlabel("Number of Discs (n)")
plt.ylabel("Total Moves (log)")
plt.title("Log Scale")
plt.grid(True)

plt.tight_layout()
plt.savefig("hanoi_moves.png")
print("Graph saved successfully as hanoi_moves.png")
