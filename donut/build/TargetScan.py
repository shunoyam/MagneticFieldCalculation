from lib import *

def C(u,v,alpha,beta,gamma):
    u_req=.2
    c=(u/alpha)+(v/beta)
    if u>u_req:
        c+=gamma
    return c
    
alpha=.01#ppm
beta=100.#cm3
gamma=100

plt.figure()
of=open('20221009.dat')
lines=of.readlines()
t=np.array([])
U=np.array([])
V=np.array([])
Cs=np.array([])
for line in lines:
    vals=line.split()
    t=np.append(t,1.e6*float(vals[0]))
    u=float(vals[1])
    U=np.append(U,u)
    v=float(vals[2])
    V=np.append(V,v)
    Cs=np.append(Cs,C(u,v,alpha,beta,gamma))
    
colors=['#ff4b00','#05af','#03af7a']
gs=gridspec.GridSpec(2,1,height_ratios=[1.9,1])
s=30
s_legend=s-7
ax=plt.subplot(gs[0])
ax.plot(t,U,color=colors[0],label='Achieved uniformity',marker='.')

ax.set_xlabel('(Target field (T) - 3)*$10^{6}$',fontsize=s)
ax.set_ylabel('Uniformity ($\mathrm{ ppm_{p-p}})$',fontsize=s)
#ax.get_xaxis().set_tick_params(pad=5)
#ax.xaxis.set_major_locator(MaxNLocator(6))
#ax.set_xlim(-1,1)
ax.plot([-210,0],[0.2,.2],c='red',label='Required uniformity',linestyle='dashed')
ax.tick_params(axis='x',labelsize=0)
ax.tick_params(axis='y',labelsize=s)
ax.legend(fontsize=s_legend,loc='upper center')
ax2=ax.twinx()
ax2.plot(t,V,color=colors[1],label='Total iron volume',marker='.')
ax2.tick_params(axis='both',labelsize=s)
ax2.set_ylabel('Volume  ($\mathrm{cm^{3}}$)',fontsize=s)
ax2.legend(fontsize=s_legend,loc='upper center',bbox_to_anchor=(0.5,.75))
ax3=plt.subplot(gs[1],sharex=ax)
ax3.plot(t,Cs,color=colors[2],marker='.')
#,label='(U_z)/$#alpha$+V_{total}/#beta +#gamma #theta(U_{z}-U_{z}^{req})')
ax3.tick_params(axis='both',labelsize=s)
ax3.set_xlabel('(Target field (T) - 3)*$10^{6}$',fontsize=s)
ax3.set_ylabel('Cost (a.u.)',fontsize=s)
ax3.scatter(t[Cs.argmin()],min(Cs),marker='o',color='black',s=80,label='Minimum')
ax3.legend(fontsize=s_legend)
ax3.set_ylim([min(Cs)-1,32])
ax3.set_xlim([-191,-59])
plt.draw()
plt.subplots_adjust(hspace=0)
plt.savefig('tmp.pdf')
plt.show()







