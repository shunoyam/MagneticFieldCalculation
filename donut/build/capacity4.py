#This macro was copied from IronCapacity.py on Aug 19

from lib import *

plt.figure()
fig = plt.figure()

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
    rate_max.append(float(vals[5]))
    TotalVolume.append(float(vals[6])/1.e3)

plt.subplots_adjust(left=0.32,right=.7,bottom=.356,top=0.8)
plt.
plt.tight_layout()
ax1=fig.add_subplot(1,1,1)
ax2=ax1.twinx()
ax3=ax1.twinx()
rspine = ax3.spines['right']
rspine.set_position(('axes',1.15))

s=25
s_legend=s-3
s_marker=6

ax1.set_xlabel('Tolerance (mm,mrad)',fontsize=s)
ax1.set_ylabel('Uniformity (pp$\mathrm{m_{p-p}}$)',fontsize=s)
ax1.set_yscale('log')
ax1.set_ylim(0.05,600)
ax1.plot(tolerance,U_lc,marker='o',markersize=s_marker,c='black',label='Achieved uniformity')
ax1.plot([-10.,10],[.2,.2],color='#03af7a',
         label="Requirement uniformity",linestyle='dashed')
ax1.set_xlim(0,1.6)
ax1.xaxis.set_major_locator(MaxNLocator(6))
ax1.tick_params(axis='both',labelsize=s)

ax1.tick_params(axis='y',labelsize=s)
#ax1.legend(fontsize=s_legend)

ax2.set_ylabel('Ratio (%)',fontsize=s_legend-3)
ax2.plot(tolerance,rate_max,label="Ratio of pockets with full of iron",
         marker='v',markersize=s_marker,c='#05af')
ax2.set_ylim(-0.05,.8)
ax2.yaxis.set_major_locator(MaxNLocator(5))
ax2.tick_params(axis='both',labelsize=s_legend-3)
#ax2.legend(fontsize=s_legend-7,loc='upper left',bbox_to_anchor=(0.,.7))

ax3.plot(tolerance,TotalVolume,label='Iron total volume',
         marker='s',markersize=s_marker,c='#ff4b00')
ax3.set_ylabel('Volume ($\mathrm{cm^{3}}$)/1000',fontsize=s_legend-3)
ax3.set_ylim(0,18)
ax3.yaxis.set_major_locator(MaxNLocator(4))
ax3.tick_params(axis='both',labelsize=s_legend-3)
#ax3.legend(fontsize=s_legend-7,loc='upper left',bbox_to_anchor=(0.,.6))

fig.savefig('IronCapacity_20220919.pdf')
plt.show()





