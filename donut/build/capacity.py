#This macro was copied from IronCapacity.py on Aug 19

from lib import *

plt.figure()
fig = plt.figure()

of=open('IronCapacity_20220819.dat')
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

plt.subplots_adjust(left=.3, right=.7, bottom=.1, top=0.9,hspace=.4)
plt.tight_layout()
gs=gridspec.GridSpec(2,1,height_ratios=[1,1])
s=25
list_x=[0,0.2,0.4,.6,.8,1.]

ax1=fig.add_subplot(gs[0])
ax1.set_xlabel('Tolerance level [mm]',fontsize=s)
ax1.set_ylabel('U.I. after shimming [ppm]',fontsize=s)
plt.yscale('log')
plt.grid(which='major',color='black',linestyle='-',axis='y')
ax1.plot(tolerance,U_lc,label="Local",marker='.',c='#ff4b00')
ax1.plot(tolerance,U_theta,label="Azimuthal average",marker='.',c='#005aff')
ax1.set_xticklabels(list_x,fontsize=s)
ax1.set_yticklabels([1.e-4,1.e-3,1.e-2,1.e-1,1.e0,1.e1,1.e2],fontsize=s)
ax1.legend(fontsize=s-3)

ax2=fig.add_subplot(gs[1])
ax2.set_xlabel('Tolerance level [mm]',fontsize=s)
ax2.set_ylabel('Iron pocket status [/1]',fontsize=s)
ax2.plot(tolerance,n_up,label="Upper limit",marker='.')
ax2.plot(tolerance,n_ok,label="Within limit",marker='.')
ax2.plot(tolerance,n_low,label="Lower limit",marker='.')
ax2.set_xticklabels(list_x,fontsize=s)
ax2.yaxis.set_major_locator(MaxNLocator(4)) 
ax2.set_yticklabels([0,0,.2,.4,.6],fontsize=s)
ax2.legend(fontsize=s-3,loc='upper left',bbox_to_anchor=(1,1))
fig.savefig('IronCapacity_20220819.pdf')
plt.show()







