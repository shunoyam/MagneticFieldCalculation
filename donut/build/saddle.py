import matplotlib.pyplot as plt
import numpy as np
plt.figure()

of=open('saddle.dat')
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
#    bx_log.append(np.log(abs(bx))*bx/abs(bx))
    by=float(vals[3])
    by_linear.append(by)
#    by_log.append(np.log(abs(by))*by/abs(by))
    
    
scale_linear=1.e-2
plt.quiver(x,y,bx_linear,by_linear,color='black',
           angles='xy',scale_units='xy', scale=scale_linear)
plt.draw()
plt.show()
