import matplotlib.pyplot as plt
import numpy as np
import matplotlib.patches as patches

plt.figure()
ax = plt.axes()

of=open('thinkBx.dat')
lines=of.readlines()
x=[]
y=[]
bx_log=[]
by_log=[]
bx_linear=[]
by_linear=[]

for line in lines:
    vals=line.split()
    x.append(float(vals[0]))
    y.append(float(vals[1]))
    bx=float(vals[2])
    bx_linear.append(bx)
    by=float(vals[3])
    by_linear.append(by)
    
DrawingRange=.9
scale_linear=2.e-3
r_shim=.86
plt.quiver(x,y,bx_linear,by_linear,color='black',
           angles='xy',scale_units='xy', scale=scale_linear,label="horizontal field)
plt.xlim([-DrawingRange,DrawingRange])
plt.ylim([-DrawingRange,DrawingRange])
circle=patches.Circle(xy=(0, 0), radius=.333, fc='none',ec='r',label="r=33.3 cm")
arc=patches.Arc(xy=(0,0),width=r_shim*2.,height=r_shim*2.,theta1=-30.,theta2=30.,
                ec='blue',label="top view of 2 coils")
ax.add_patch(circle)
ax.add_patch(arc)
plt.title("magnetic filed z=0 by z-symmetric 2 saddle-type coils ",fontsize=20)
plt.xlabel("x [m]",fontsize=20)
plt.ylabel('y [m]',fontsize=20)
plt.rcParams["figure.figsize"] = (1,1)
plt.tick_params(labelsize=20)
plt.grid()
ax.set_aspect('equal')
plt.draw()
plt.legend(fontsize=20)
plt.show()
