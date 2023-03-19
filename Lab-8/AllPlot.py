import matplotlib.pyplot as plt
import csv

xFIFO = []
yFIFO = []

xLRU = []
yLRU = []

xRandom = []
yRandom = []

with open('csvFIFO.csv','r') as csvFIFO, open('csvLRU.csv','r') as csvLRU, open('csvRandom.csv','r') as csvRandom:
	FIFOlines = csv.reader(csvFIFO, delimiter=',')
	for row in FIFOlines:
		xFIFO.append(row[0])
		yFIFO.append(row[1])
		
	LRUlines = csv.reader(csvLRU, delimiter=',')
	for row in LRUlines:
		xLRU.append(row[0])
		yLRU.append(row[1])
	
	Randomlines = csv.reader(csvRandom, delimiter=',')
	for row in Randomlines:
		xRandom.append(row[0])
		yRandom.append(row[1])
	
# plot all the data
plt.plot(xFIFO, yFIFO, color = 'blue', linestyle = 'solid', marker = 'o',label = "FIFO")
plt.plot(xLRU, yLRU, color = 'green', linestyle = 'solid', marker = 'o',label = "LRU")
plt.plot(xRandom, yRandom, color = 'red', linestyle = 'solid', marker = 'o',label = "Random")

plt.xlabel('Frames')
plt.ylabel('Page Faults')
plt.show()
