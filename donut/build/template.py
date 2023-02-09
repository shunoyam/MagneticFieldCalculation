import sys
sus.path.append("/home/shun/software/python/")

plt.figure()

of=open('')
lines=of.readlines()
x=[]
for line in lines:
    vals=line.split()
    x.append(float(vals[0]))
colors=['#ff4b00','#05af','#03af7a']
    
fontsize=20
scale_linear=1.e-2
ax=plt.axes()
ax.plot(x,y,color=colors[0],marker='o',label='name')

#gs=gridspex.GridSpec(2,1,height_ratios=[1,1])
#ax=plt.subplot(gs[0])

ax.set_xlabel('x (m)',fontsize=fontsize)
ax.set_ylabel('y (m)',fontsize=fontsize)
#ax.get_xaxis().set_tick_params(pad=5)
#ax.xaxis.set_major_locator(MaxNLocator(6))
#ax.set_yscale('log')
#ax.set_xlim(-1,1)
ax.tick_params(axis='both',labelsize=fontsize)
ax.legend(fontsize=fontsize,bbox_to_anchor=(0,1),loc='upper left')
#ax.set_aspect('equal')
#ax2=ax.twinx()
plt.draw()
plt.savefig('tmp.pdf')
plt.show()







