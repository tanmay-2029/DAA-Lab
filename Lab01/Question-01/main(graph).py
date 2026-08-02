import csv
import matplotlib.pyplot as plt

file=open("growth_values.csv","r")
reader=csv.reader(file)

header=next(reader)

rows=[]
for row in reader:
    temp=[]
    for x in row:
        temp.append(float(x))
    rows.append(temp)
file.close()

n=[]
for row in rows:
    n.append(row[0])

for i in range(len(header)-1):
    y=[]
    for row in rows:
        y.append(row[i+1])
    plt.plot(n,y,label=header[i+1])

plt.yscale("log")
plt.xlabel("n")
plt.ylabel("f(n)")
plt.title("Growth of Functions")
plt.legend(fontsize=8)
plt.grid(True)
plt.savefig("growth_of_functions.png")
print("Graph saved")
