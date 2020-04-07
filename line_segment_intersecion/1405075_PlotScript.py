import numpy as np
import matplotlib.pyplot as plt

p = []
q = []

with open('1405075_input2.txt') as f1:
	next(f1)
	for line in f1:
		xC, yC, xD, yD = line.split()
		p.append(xC)
		p.append(xD)
		q.append(yC)
		q.append(yD)

fig = plt.figure(figsize=(16, 16))
ax = fig.add_subplot(111)

for i in range(0, len(p), 2):
    plt.plot(p[i:i+2], q[i:i+2], 'bo-')

a = []
b = []

with open('1405075_output.txt') as f1:
	for line in f1:
		xC, yC = line.split()
		a.append(xC)
		b.append(yC)

plt.scatter(a, b, s=30, c='r')
for xy in zip(a, b):
    ax.annotate('(%s, %s)' % xy, xy=xy, textcoords='data')

plt.title("Line-Segment Intersection")
plt.xlabel("X")
plt.ylabel("Y")
plt.grid()
plt.show()
