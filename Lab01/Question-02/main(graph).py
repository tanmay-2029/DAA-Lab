# plot the coin toss data
import pandas as pd
import matplotlib.pyplot as plt
import os
import sys

if not os.path.exists("fair_coin.csv") or not os.path.exists("biased_coin.csv"):
    print("Error: Run the C program first to generate CSVs!")
    sys.exit(1)

fair_df = pd.read_csv("fair_coin.csv")
biased_df = pd.read_csv("biased_coin.csv")

plt.figure(figsize=(10, 6))

# plot both
plt.plot(fair_df['toss'], fair_df['p_head'], label='Fair (0.5)', color='blue')
plt.plot(biased_df['toss'], biased_df['p_head'], label='Biased', color='red')

plt.axhline(0.5, color='gray', linestyle='--')

plt.xlabel('Tosses')
plt.ylabel('P(Heads)')
plt.title('Coin Toss Sim: Law of Large Numbers')
plt.legend()
plt.grid(True, alpha=0.3)

plt.tight_layout()
plt.savefig("coin_convergence.png")
print("Plot saved to results.png")
