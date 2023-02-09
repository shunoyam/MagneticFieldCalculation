import matplotlib.pyplot as plt
import numpy as np
import matplotlib.patches as patches
plt.figure()
fig = plt.figure()

of=open('IronCapacity_20220816.dat')
lines=of.readlines()
tolerance=[]
UseRate=[]
B_tg=[]
U_lc=[]
U_theta=[]
n_up=[]
n_ok=[]
n_low=[]
for line in lines:
    vals=line.split()
    tolerance.append(float(vals[0])*1.e3)
    UseRate.append(float(vals[1]))
    B_tg.append(float(vals[2]))
    U_lc.append(float(vals[3])*1.e6)
    U_theta.append(float(vals[4])*1.e6)
    n_up.append(float(vals[5]))
    n_ok.append(float(vals[6]))
    n_low.append(float(vals[7]))

ax1=fig.add_subplot(2,2,1)
ax1.set_xlabel('Tolerance level [mm]')
ax1.set_ylabel('Use rate of iron shim [a.u.]')
ax1.plot(tolerance,UseRate,marker='.')

ax2=fig.add_subplot(2,2,2)
ax2.set_xlabel('Tolerance level [mm]')
ax2.set_ylabel('(Target magnetic field [T] -3)*1.e3')
ax2.plot(tolerance,B_tg,marker='.')

ax3=fig.add_subplot(2,2,3)
ax3.set_xlabel('Tolerance level [mm]')
ax3.set_ylabel('Uniformity after shimming [ppm]')
plt.yscale('log')
plt.grid(which='major',color='black',linestyle='-',axis='y')
ax3.plot(tolerance,U_lc,label="Local",marker='.')
ax3.plot(tolerance,U_theta,label="Azimuthal average",marker='.')
ax3.legend()

ax4=fig.add_subplot(2,2,4)
ax4.set_xlabel('Tolerance level [mm]')
ax4.set_ylabel('Rate of 980 iron shim pockets [a.u]')
ax4.plot(tolerance,n_up,label="Upper limit",marker='.')
ax4.plot(tolerance,n_ok,label="Within limit",marker='.')
ax4.plot(tolerance,n_low,label="Lower limit",marker='.')
ax4.legend()
fig.savefig('IronCapacity_20220815.pdf')
plt.show()







