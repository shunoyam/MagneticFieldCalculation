import numpy as np
import matplotlib.pyplot as plt
import matplotlib.pyplot as plt
import numpy as np
from mpl_toolkits.axes_grid1 import make_axes_locatable
x_list=[]
y_list=[]
z_list=[]
InputFile=open('after_python.dat')
lines=InputFile.readlines()
for line in lines:
    vals=line.split()
    x_list.append(vals[0])
    y_list.append(vals[1])
    z_list.append(vals[2])
fig = plt.figure()
r_c=33.3
r_w=1.5
z_c=0.
z_w=5.
r_min=r_c-r_w
r_max=r_c+r_w
z_min=z_c-z_w
z_max=z_c+z_w
cm = plt.cm.get_cmap('RdYlBu')
ax = fig.add_axes((0.1, 0.1, 0.9, 0.9))
ax.set_xlabel("r [cm]")
ax.set_ylabel("z [cm]")
print(min(z_list))
maingraph=ax.scatter(x_list, y_list,z_list,vmin=min(z_list),vmax=max(z_list),s=35,cmap=cm)

print(max(z_list))
divider= make_axes_locatable(ax)
color_ax = divider.append_axes("right", size="７%", pad="2%") #append_axesで新しいaxesを作成                                           
colorbar=fig.colorbar(maingraph, cax=color_ax )
line=ax.scatter(xline_list, yline_list, c="black", s=2)
plt.show()
print('adfashf')
#
#
#plt.contour(data)
#n_r=len(data[0])
#n_z=len(data)
#r=np.linspace(r_min,r_max,n_r)
#plt.figure(figsize=(5,30))
#plt.xlim(r_min,r_max)
#plt.ylim(z_min,z_max)
#z=np.linspace(z_min,z_max,n_z)
#plt.xticks([r_min,r_c,r_max])
#plt.xticks(fontsize=18)
#plt.yticks(fontsize=18)
#plt.xlabel('r [cm]', fontsize=18)
#plt.ylabel('z [cm]', fontsize=18)
#ax = plt.contour(r,z,data,colors='black',levels=5)
#ax.clabel(fontsize=18,inline=True, inline_spacing=1)
#plt.show()
#

