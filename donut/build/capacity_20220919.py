#This macro was copied from IronCapacity.py on Aug 19

from lib import *

plt.figure()
fig = plt.figure()

of=open('capacity_20220925.dat')
#of=open('20220919.dat')
lines=of.readlines()
tolerance=[]
U_lc=[]
U_theta=[]
rate_min=[]
rate_ok=[]
rate_max=[]
for line in lines:
    vals=line.split()
    tolerance.append(float(vals[0])*1.e3)
    U_lc.append(float(vals[1]))
    U_theta.append(float(vals[2]))
    rate_min.append(float(vals[3]))
    rate_ok.append(float(vals[4]))
    rate_max.append(float(vals[5]))

plt.subplots_adjust(left=.3, right=.7, bottom=.1, top=0.9,hspace=.4)
plt.tight_layout()
gs=gridspec.GridSpec(2,1,height_ratios=[1,1])
s=25

ax1=fig.add_subplot(gs[0])

ax1.set_xlabel('Tolerance level [mm]',fontsize=s)
ax1.set_ylabel('Achieved uniformity [ppm]',fontsize=s)
plt.yscale('log')
plt.grid(which='major',color='black',linestyle='-',axis='y')
#ax1.set_xlim(0,1,1.6)
ax1.plot(tolerance,U_lc,marker='.',c='b')
p=plt.plot([0.1,1.5],[0.2, 0.2], "g", linestyle='dashed',label="Requirement")

xlabel=[0.,.2,.4,.6,.8,1.,1.2,1.4,1.6]
ax1.set_xticklabels(xlabel,fontsize=s)
ax1.set_yticklabels([1.e-1,1.e0,1.e0,1.e1,1.e2,1.e3],fontsize=s)
ax1.legend(fontsize=s-3)

ax2=fig.add_subplot(gs[1])
#ax2.set_xlim(0,1,1.6)
ax2.set_xlabel('Tolerance level [mm]',fontsize=s)
ax2.set_ylabel('Iron pocket status [/1]',fontsize=s)
ax2.plot(tolerance,rate_max,label="$v=v_{max}$",marker='.')
ax2.plot(tolerance,rate_ok,label="$v_{min}<v<v_{max}$",marker='.')
ax2.plot(tolerance,rate_min,label="$v=v_{min}$",marker='.')
ax2.set_xticklabels(xlabel,fontsize=s)
ax2.yaxis.set_major_locator(MaxNLocator(3)) 
ax2.set_ylim(-.01,.9)
ax2.set_yticklabels([-.4,0,.4,.8],fontsize=s)
ax2.legend(fontsize=s-10,loc='upper left',bbox_to_anchor=(0.4,.7))
fig.savefig('IronCapacity_20220919.pdf')
plt.show()







