import sys
sys.path.append("/home/shun/software/python/")

from lib import *

fig=plt.figure(figsize=(20,10))
data=np.loadtxt('AxialShift20221127.dat',delimiter='\t',dtype='float')
dz=data[:,0]
colors=['#ff4b00','#05af','#03af7a','black','gray','m']
fontsize=40
scale_linear=1.e-2
margin=.2
plt.subplots_adjust(left=.113,right=.95,bottom=.137,top=.9,hspace=.1)
ax=plt.axes()
labels=['Iron yoke','MC10','MC11','MC20','MC21','MC30']
for i in range(6):
    ax.plot(dz,data[:,i+1],color=colors[i],label=labels[i],markersize=5,marker='o')


ax.set_xlabel('Axial shift [mm]',fontsize=fontsize)
ax.set_ylabel('$B_z$ uniformity [$\mathrm{ppm_{p-p}}$]',fontsize=fontsize)
#ax.get_xaxis().set_tick_params(pad=5)
#ax.xaxis.set_major_locator(MaxNLocator(6))
#ax.set_yscale('log')
#ax.set_xlim(-1,1)
ax.tick_params(axis='both',labelsize=fontsize)
ax.legend(fontsize=fontsize-5,loc='upper center')
plt.savefig('AxialShiftResults.pdf')
plt.show()







