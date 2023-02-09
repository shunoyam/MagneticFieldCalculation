import matplotlib.pyplot as plt
import numpy as np

fig = plt.figure()
ax = fig.add_subplot(projection='3d')

x = np.arange(4)
y = np.arange(5)
xx, yy = np.meshgrid(x, y)

xr = xx.ravel()
yr = yy.ravel()

height = xr + yr
print('height',len(height))
print('xr',len(xr))
print('yr',len(yr))
bottom = np.ones_like(height)
width = 0.5
depth = 0.5

ax.bar3d(xr, yr, bottom, width, depth, height)
plt.show()
