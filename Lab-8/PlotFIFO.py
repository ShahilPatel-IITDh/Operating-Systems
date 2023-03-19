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
plt.title('FIFO Page Replacement Algorithm')

plt.xlabel('Frames')
plt.ylabel('Page Faults')
plt.show()
