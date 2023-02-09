
from lib import *

fig=plt.figure(figsize=(10,10))

of=open('BrShimCoilSkelton.dat')
lines=of.readlines()
x=[]
y=[]
z=[]
for line in tqdm(lines):
    vals=line.split()
    x.append(float(vals[0]))
    y.append(float(vals[1]))
    z.append(float(vals[2]))

s=20
ax=fig.add_subplot(projection='3d')
ax.xaxis.set_pane_color((1.0,1.,1.0, 0.0))
ax.yaxis.set_pane_color((1.0,1.,1.0, 0.0))
ax.zaxis.set_pane_color((1.0,1.,1.0, 0.0))
ax.tick_params(labelbottom=False,labelleft=False,labelright=False,labeltop=False)
ax.grid(False)
pad=10
ax.set_xlabel('x (m)',fontsize=s,labelpad=pad)
ax.set_ylabel('y (m)',fontsize=s,labelpad=pad)
ax.set_zlabel('z (m)',fontsize=s,labelpad=pad)
ax.tick_params('x',length=0,which='major',labelsize=s)
ax.tick_params('y',length=0,which='major',labelsize=s)
ax.tick_params('z',length=0,which='major',labelsize=s)
ax.scatter(x,y,z,s=1,edgecolor="#ff4b00",linewidth=1,c="#ff4b00",marker='.',label='point',
           facecolor='#ff4b00')
ax.set_xlim([-1.2,1.2])
ax.set_ylim([-1.2,1.2])
ax.set_zlim([-1.2,1.2])
ax.legend()
of_storage=open('storage.dat')
xs=[]
ys=[]
zs=[]
lines=of_storage.readlines()
for line in tqdm(lines):
    vals=line.split()
    xs.append(float(vals[0])*1.e-2)
    ys.append(float(vals[1])*1.e-2)
    zs.append(float(vals[2])*1.e-2)

ax.scatter(xs,ys,zs,s=1,color='aqua')
plt.savefig("BrTypeShimCoil.png",dpi=300)













