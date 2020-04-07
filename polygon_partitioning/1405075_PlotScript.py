import matplotlib.pyplot as plt

p = []
q = []

with open('1405075_input2.txt') as f1:
	next(f1)
	for line in f1:
		xC, yC = line.split()
		p.append(xC)
		q.append(yC)
	p.append(p[0])
	q.append(q[0])


fig = plt.figure(figsize=(16, 16))
ax = fig.add_subplot(111)

plt.scatter(p, q, s=10, c='r')
for xy in zip(p,q):                                       
    ax.annotate('(%s, %s)' % xy, xy=xy, textcoords='data') 

plt.grid()
plt.plot(p, q, linewidth=2, color='b')

x = []
y = []

with open('1405075_output.txt') as f2:
	for line in f2:
		xC, yC = line.split()
		x.append(xC)
		y.append(yC)

for i in range(0, len(x), 2):
    plt.plot(x[i:i+2], y[i:i+2], linewidth=2, color='r')

plt.title("Polygon Partitioning")
plt.xlabel("X")
plt.ylabel("Y")

plt.show()
