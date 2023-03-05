#! /usr/bin/env python3

import random
import numpy as np
import matplotlib.pyplot as plt

# asize is the size of the address space
asize = 1024
# psize is the size of the page table
psize = 1024
# uList is the list of valid fraction
uList = [0] * psize

# i is the number of random seeds
for i in range(50000):
    random.seed(i)
    # j is the limit
    for j in range(psize):
        limit = j
        # vaddr is the virtual address
        virtualAddress = int(asize * random.random())
        if (virtualAddress < limit):
            uList[j] += 1

figure = plt.figure()
# linspace: Return evenly spaced numbers over a specified interval., 1 to psize
x = np.linspace(1, psize, psize)
plt.plot(x, [u/50000 for u in uList], color='black')
plt.ylim(0, 1)
plt.margins(0)
plt.xlabel('Limit')
plt.ylabel('Valid fraction (Average)')
plt.savefig('valid_fraction.png', dpi=227)
plt.show()
