import matplotlib.pyplot as plt

x_level1, x_level2 = [], []

for i in range(1000, 10001, 1000):
    x_level1.append(i)

for i in range(11000, 100001, 10000):
    x_level1.append(i)

y_level1 = []
y_level1 = [3.031, 6.721, 12.677, 15.611, 16.605, 20.296, 24.497, 28.294, 28.41, 33.253,
            80.653, 107.798, 140.698, 170.941, 208.024, 252.297, 283.954, 314.629, 343.37]


fig = plt.figure(1)


ax1 = plt.subplot(2, 1, 1)
ax1.set_xlabel('node size')
ax1.set_ylabel('running time (ms)')
plt.plot(x_level1, y_level1)
plt.title("level1")


x_level3 = []
for i in range(200000, 1000000, 100000):
    x_level3.append(i)

for i in range(1000000, 7000001, 500000):
    x_level3.append(i)
y_level3 = [0.74, 1.04, 1.39, 1.81, 2.14, 2.49, 2.84, 3.25, 3.66, 5.98,
            7.44, 9.78, 11.43, 13.31, 15.44, 17.57, 20.26, 21.85, 23.52, 25.42, 27.53]
ax3 = plt.subplot(2, 1, 2)
ax3.set_xlabel('node size')
ax3.set_ylabel('running time (s)')
plt.plot(x_level3, y_level3)
plt.title("level2")

plt.suptitle("B+Tree Insert Test")
plt.show()
