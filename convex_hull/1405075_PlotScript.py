import matplotlib.pyplot as plt

p = []
q = []

with open('1405075_input1.txt') as f2:
	next(f2)
	for line in f2:
		xC, yC = line.split()
		p.append(xC)
		q.append(yC)

fig = plt.figure()
ax = fig.add_subplot(111)

plt.scatter(p, q, s=10, c='r')
for xy in zip(p,q):                                       
    ax.annotate('(%s, %s)' % xy, xy=xy, textcoords='data') 

plt.grid()

x = []
y = []

with open('1405075_output.txt') as f1:
	for line in f1:
		xC, yC = line.split()
		x.append(xC)
		y.append(yC)

plt.plot(x, y, linewidth=4)

plt.title("Convex Hull")
plt.xlabel("X")
plt.ylabel("Y")

plt.show()
