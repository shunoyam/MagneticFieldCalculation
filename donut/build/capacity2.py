#This macro was copied from IronCapacity.py on Aug 19
import sys
sys.path.append("/home/shun/software/python/")
from lib import *

fig=plt.figure(figsize=(15,10))

of=open('capacity_20220930.dat')
lines=of.readlines()
tolerance=[]
U_lc=[]
U_theta=[]
rate_min=[]
rate_ok=[]
rate_max=[]
TotalVolume=[]
for line in lines:
    vals=line.split()
    tolerance.append(float(vals[0]))
    U_lc.append(float(vals[1]))
    U_theta.append(float(vals[2]))
    rate_min.append(float(vals[3]))
    rate_ok.append(float(vals[4]))
    rate_max.append(float(vals[5])*1.e2)
    TotalVolume.append(float(vals[6]))

plt.subplots_adjust(left=.15, right=.9, bottom=.12, top=0.934,hspace=0)
plt.tight_layout()
gs=gridspec.GridSpec(2,1,height_ratios=[1.9,1])
s=30
s_legend=s-5
s_marker=10
w=2

ax1=plt.subplot(gs[0])
ax1.set_xlabel('Tolerance (mm,mrad)',fontsize=s)
ax1.set_ylabel('$B_{z}$ uniformity $(\mathrm{ppm_{p-p}})$',fontsize=s)
ax1.set_yscale('log')
ax1.set_ylim(.05,600)
ax1.plot(tolerance,U_lc,marker='o',c='black',label='Achieved uniformity',markersize=s_marker)
ax1.plot([-10,10],[0.2, 0.2],c='#03af7a',linestyle='dashed',label="Required uniformity",
         markersize=s_marker)
ax1.set_xlim(0,1.6)
ax1.tick_params(axis='x',labelsize=0)
ax1.tick_params(axis='y',labelsize=s,width=w)
ax1.legend(fontsize=s_legend)




ax2=ax1.twinx()
ax2.set_ylabel('Ratio (%)',fontsize=s)
ax2.set_ylim(-5,100)
ax2.yaxis.set_major_locator(MaxNLocator(5))
ax2.plot(tolerance,rate_max,label="Ratio of pockets with full of iron",
         marker='v',c='#05af',markersize=s_marker)
ax2.tick_params(axis='both',labelsize=s,width=w)
ax2.legend(fontsize=s_legend,loc='upper left',bbox_to_anchor=(0,.7))

ax3=plt.subplot(gs[1],sharex=ax1)
ax3.plot(tolerance,TotalVolume,marker='s',
         c='#ff4b00',label='Iron total volume',markersize=s_marker)
ax3.set_xlabel('Tolerance (mm,mrad)',fontsize=s)
ax3.get_xaxis().set_tick_params(pad=5)
ax3.get_yaxis().set_tick_params(pad=-3)
ax3.set_ylabel('Volume ($\mathrm{cm^{3}}$)',fontsize=s-4)
ax3.xaxis.set_major_locator(MaxNLocator(6))
ax3.yaxis.set_major_locator(MaxNLocator(4))
ax3.tick_params(axis='both',labelsize=s,width=w)
ax3.tick_params(axis='x',pad=10)
ax3.tick_params(axis='y',pad=5)
ax3.set_ylabel(ax3.get_ylabel(),labelpad=20)
ax3.set_ylim(0,35000)
ax3.plot([-10,10],[30800,30800],c='r',linestyle='dashdot',label="Total capacity",
         markersize=s_marker)
ax3.legend(fontsize=s_legend)





#ax3.yaxis.set_major_formatter(ptick.ScalarFormatter(useMathText=True))
#ax3.ticklabel_format(style="sci", axis="y", scilimits=(3,3))
plt.savefig("capacity.pdf")
plt.show()





