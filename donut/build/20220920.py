import matplotlib.pyplot as plt
import numpy as np
import matplotlib.patches as patches
plt.figure()
ax=plt.axes()

of=open('20220920.dat')
lines=of.readlines()
x=[]
y=[]
bx_list=[]
by_list=[]
factor=1.
for line in lines:
    vals=line.split()
    x.append(float(vals[0]))
    y.append(float(vals[1]))
    by=float(vals[2])*factor
    by_list.append(by)
    bx=float(vals[3])*factor
    bx_list.append(bx)
    
scale_linear=8.e-4
circle=patches.Circle(xy=(0, 0), radius=.333, fc='none',ec='gray',label="r=33.3 cm")
circle.set_linestyle('--')
plt.quiver(x,y,bx_list,by_list,color='black',
           angles='xy',scale_units='xy', scale=scale_linear)
ax.add_patch(circle)
plt.xlabel("x [m]",fontsize=20)
plt.ylabel("y [m]",fontsize=20,labelpad=-5)
plt.xlim([-.39,.39])
plt.ylim([-.39,.39])
plt.tick_params(labelsize=20)
ax.set_aspect('equal')
plt.draw()
plt.show()










