import sys
sys.path.append("/home/shun/software/python/")
from lib import *

fig=plt.figure(figsize=(20,10))
plt.subplots_adjust(bottom=.2,left=.15)
data=np.loadtxt('capacity_20220930.dat',delimiter='\t',dtype='float')
tolerance=data[:,0]
U_lc=data[:,1]
U_theta=data[:,2]
rate_min=data[:,3]
rate_ok=data[:,4]
rate_max=data[:,5]
TotalVolume=data[:,6]

s=45
s_legend=s-3
ax=plt.axes()
ax.set_xlabel('Tolerance [mm,mrad]',fontsize=s)
ax.set_ylabel('$B_z$ uniformity [$\mathrm{ppm_{p-p}}$]',fontsize=s)
plt.yscale('log')
ax.set_ylim(.1,3.e2)
ax.plot(tolerance,U_lc,marker='o',c='black',label='Achieved uniformity',linewidth=3)
ax.plot(ax.get_xlim(),[0.2, 0.2],c='#03af7a',linestyle='dashed',
        label="Required uniformity",linewidth=3)
ax.xaxis.set_major_locator(MaxNLocator(9))
ax.tick_params(axis='both',labelsize=s)
ax.legend(fontsize=s_legend)

#ax2.plot(tolerance,TotalVolume,marker='.',c='black',label='Iron total volume')
#ax2.set_ylabel('Volume [$\mathrm{cm^{3}}$]',fontsize=s)
#ax2.yaxis.set_major_locator(MaxNLocator(3))
#ax2.tick_params(axis='both',labelsize=s)
#ax2.legend(fontsize=s_legend-5,loc='upper left'
#           ,bbox_to_anchor=(0.,.95))
#
#ax3=ax2.twinx()
##ax3=plt.subplot(gs[2],sharex=ax1)
#ax3.set_ylabel('Ratio of pockets with iron of 3 type volume [/1]',fontsize=s_legend-6)
#ax3.plot(tolerance,rate_max,label="$0 \mathrm{cm^{3}}$",marker='.')
#ax3.plot(tolerance,rate_ok,marker='.',
#         label=">0 $\mathrm{cm^{3}}$ and <77 $\mathrm{cm^{3}}$")
#ax3.plot(tolerance,rate_min,label="$77 \mathrm{cm^{3}}$",marker='.')
#ax3.tick_params(axis='both',labelsize=s)
#ax3.legend(fontsize=s_legend-7,loc='upper left',bbox_to_anchor=(0.,.8))
#plt.subplots_adjust(hspace=.0)
plt.show()





