import matplotlib.pyplot as plt
import csv

x = []
y = []

with open('csvFIFO.csv','r') as csvFIFO:
	lines = csv.reader(csvFIFO, delimiter=',')
	for row in lines:
		x.append(row[0])
		y.append(row[1])

plt.plot(x, y, color = 'red', linestyle = 'solid', marker = 'o',label = "Frames vs Page Faults")

a = []
b = []
with open('csvLRU.csv','r') as csvLRU:
	lines = csv.reader(csvLRU, delimiter=',')
	for row in lines:
		a.append(row[0])
		b.append(row[1])

plt.plot(a, b, color = 'blue', linestyle = 'solid', marker = 'o',label = "Frames vs Page Faults")

c = []
d = []
with open('csvRandom.csv','r') as csvRandom:
	lines = csv.reader(csvRandom, delimiter=',')
	for row in lines:
		c.append(row[0])
		d.append(row[1])

plt.plot(c, d, color = 'green', linestyle = 'solid', marker = 'o',label = "Frames vs Page Faults")
plt.title('Random Page Replacement Algorithm')

plt.xlabel('Frames')
plt.ylabel('Page Faults')
plt.show()
