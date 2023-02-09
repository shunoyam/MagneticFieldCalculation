from lib import *

plt.figure()
ax=plt.axes()

    
InputFile=open('20221006.dat')
lines=InputFile.readlines()
r_lower=[]
z_lower=[]
r_upper=[]
z_upper=[]
for line in lines:
    vals=line.split()
    if vals[3]=='lower':
        r_lower.append(float(vals[0])*1.e2)
        z_lower.append(float(vals[2])*1.e2)
    else:
        r_upper.append(float(vals[0])*1.e2)
        z_upper.append(float(vals[2])*1.e2)
s=20
#ax.scatter(r_lower,z_lower,color='#ff4b00',label='lower')
ax.scatter(r_upper,z_upper,color='#05af',label='upper')
yr=7.
xr=2.
ax.set_xlim(33.3-xr,33.3+xr)
ax.set_ylim(-yr,yr)
ax.yaxis.set_major_locator(MaxNLocator(7))
minorLocator=MultipleLocator(1)
ax.xaxis.set_minor_locator(minorLocator)
ax.yaxis.set_minor_locator(minorLocator)
plt.xlabel("r [cm]",fontsize=s)
plt.ylabel("z [cm]",fontsize=s)
plt.tick_params(labelsize=s)
ax.set_aspect('equal')
#ax.legend(fontsize=s,loc='upper left',bbox_to_anchor=(1.,.8))
plt.draw()
plt.show()







