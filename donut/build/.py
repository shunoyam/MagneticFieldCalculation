import sys
sys.path.append("/home/shun/software/python/")

from lib import *

fig=plt.figure(figsize=(20,10))
data=np.loadtxt('.dat',delimiter='\t',dtype='float')
x=data[:,0]
colors=['#ff4b00','#05af','#03af7a']
    
fontsize=30
scale_linear=1.e-2
margin=.1
plt.subplots_adjust(left=margin,right=1.-margin,bottom=margin,top=1.-margin,hspace=.1)
#gs=gridspec.GridSpec(2,1,height_ratios=[1,1])
#ax=plt.subplot(gs[0])
ax=plt.axes()
ax.plot(x,y,color=colors[0],marker='o',label='name',markersize=1)


ax.set_xlabel('x []',fontsize=fontsize)
ax.set_ylabel('y []',fontsize=fontsize)
#ax.get_xaxis().set_tick_params(pad=5)
#ax.xaxis.set_major_locator(MaxNLocator(6))
#ax.set_yscale('log')
#ax.set_xlim(-1,1)
ax.tick_params(axis='both',labelsize=fontsize)
ax.legend(fontsize=fontsize,bbox_to_anchor=(0,1),loc='upper left')
#plt.subplots_adjust(hspace=0)
#ax.set_aspect('equal')
#ax2=ax.twinx()
#plt.savefig('tmp.pdf')
plt.show()







