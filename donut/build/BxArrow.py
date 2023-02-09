import matplotlib.pyplot as plt
import numpy as np
import matplotlib.patches as patches
plt.figure()
ax=plt.axes()

of=open('BxArrow.dat')
lines=of.readlines()
x=[]
y=[]
bx_log=[]
by_log=[]
bx_linear=[]
by_linear=[]
factor=1.
for line in lines:
    vals=line.split()
    x.append(float(vals[0]))
    y.append(float(vals[1]))
    by=float(vals[2])*factor
    by_linear.append(by)
#    bx_log.append(np.log(abs(bx))*bx/abs(bx))
    bx=float(vals[3])*factor
    bx_linear.append(bx)
#    by_log.append(np.log(abs(by))*by/abs(by))
    
    
scale_linear=1.e-2
circle=patches.Circle(xy=(0, 0), radius=.333, fc='none',ec='gray',label="r=33.3 cm")
circle.set_linestyle('--')
plt.quiver(x,y,bx_linear,by_linear,color='black',
           angles='xy',scale_units='xy', scale=scale_linear)
#plt.figure(figsize=(5,5))
ax.add_patch(circle)
plt.xlabel("x [m]",fontsize=20)
plt.ylabel("y [m]",fontsize=20)
plt.tick_params(labelsize=20)
#plt.grid()
ax.set_aspect('equal')
plt.draw()
plt.show()










