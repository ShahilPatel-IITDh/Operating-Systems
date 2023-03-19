import matplotlib.pyplot as plt
import csv

x = []
y = []

with open('csvLRU.csv','r') as csvLRU:
	lines = csv.reader(csvLRU, delimiter=',')
	for row in lines:
		x.append(row[0])
		y.append(row[1])

plt.plot(x, y, color = 'blue', linestyle = 'solid', marker = 'o', label = "Frames vs Page Faults")

plt.xlabel('Frames')
plt.ylabel('Page Faults')
plt.show()
