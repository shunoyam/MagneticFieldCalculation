import numpy as np
import matplotlib.pyplot as plt
data=np.loadtxt('perfect2.dat')
data=data*1.e6/3.
fig = plt.figure()
plt.contour(data)
r_c=33.3
r_w=1.5
z_c=0.
z_w=5.
r_min=r_c-r_w
r_max=r_c+r_w
z_min=z_c-z_w
z_max=z_c+z_w

n_r=len(data[0])
n_z=len(data)
r=np.linspace(r_min,r_max,n_r)
plt.figure(figsize=(5,30))
plt.xlim(r_min,r_max)
plt.ylim(z_min,z_max)
z=np.linspace(z_min,z_max,n_z)
plt.xticks([r_min,r_c,r_max])
plt.xticks(fontsize=18)
plt.yticks(fontsize=18)
plt.xlabel('r[cm]', fontsize=18)
plt.ylabel('z[cm]', fontsize=18)
ax = plt.contour(r,z,data-1.357,colors='black',levels=5)
ax.clabel(fontsize=18,inline=True, inline_spacing=1)
plt.show()
