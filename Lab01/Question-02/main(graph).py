# plot the coin toss data
import pandas as pd
import matplotlib.pyplot as plt
import os
import sys

if not os.path.exists("fair.csv") or not os.path.exists("biased.csv"):
    print("Error: Run the C program first to generate CSVs!")
    sys.exit(1)

# using pandas is usually what most people do instead of the manual csv module
fair_df = pd.read_csv("fair.csv")
biased_df = pd.read_csv("biased.csv")

plt.figure(figsize=(10, 6))

# plot both
plt.plot(fair_df['toss'], fair_df['p_head'], label='Fair (0.5)', color='blue')
plt.plot(biased_df['toss'], biased_df['p_head'], label='Biased', color='red')

# reference line for 0.5
plt.axhline(0.5, color='gray', linestyle='--')

plt.xlabel('Tosses')
plt.ylabel('P(Heads)')
plt.title('Coin Toss Sim: Law of Large Numbers')
plt.legend()
plt.grid(True, alpha=0.3)

plt.tight_layout()
plt.savefig("results.png")
print("Plot saved to results.png")
