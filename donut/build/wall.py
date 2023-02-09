import sys
sys.path.append("/home/shun/software/python/")

from lib import *

fig=plt.figure(figsize=(20,10))
colors=['#ff4b00','#05af','#03af7a']
    
fontsize=20
scale_linear=1.e-2
margin=.1
plt.subplots_adjust(left=margin,right=1.-margin,bottom=.1,top=1.-margin,hspace=.1)


r_max=375.
r_kicker=360.
r_center=333.
r_capton=300.
r_detector=290.
r_min=200.
r_width=15.
r_max_acc=r_kicker-3.

z_max=240.
z_max_kicker_low=-50.
z_min_kicker_low=-150.
z_max_kicker_up=300.
z_min_kicker_up=200.
w_detector=200.
w_capton=200.
z_acc=w_capton
z_width=50.

ax=plt.axes()
ax.set_xlim(r_min,r_max)
ax.set_ylim(-z_max,z_max)
ax.plot([r_capton,r_capton],[-w_capton,w_capton],color='#a65628',label="Polyimide film",
        alpha=.3,linewidth=5)
ax.plot([r_capton,r_capton],[w_capton,z_max],color='gray',label="Vacuum chamber",
        linewidth=9)
ax.plot([r_capton,r_capton],[-z_max,-w_capton],color='gray',linewidth=9)

ax.plot([r_center-r_width,r_center-r_width],[-z_width,z_width],color='coral',linestyle='--',
        label="Muon storage region")
ax.plot([r_center+r_width,r_center+r_width],[-z_width,z_width],color='coral',linestyle='--')
ax.plot([r_center-r_width,r_center+r_width],[-z_width,-z_width],color='coral',linestyle='--')
ax.plot([r_center-r_width,r_center+r_width],[z_width,z_width],color='coral',linestyle='--')

ax.plot([r_kicker,r_kicker],[z_min_kicker_low,z_max_kicker_low],color='red',linestyle='-',
        linewidth=4,label="Kicker coil")
ax.plot([r_kicker,r_kicker],[z_min_kicker_up,z_max_kicker_up],color='red',linestyle='-',
        linewidth=4)

adjust=5.
ax.plot([r_capton+adjust,r_capton+adjust],[-z_acc,z_acc],
        color='olive',linestyle='--',label="Detectable region")
ax.plot([r_max_acc,r_max_acc],[-z_acc,z_acc],
        color='olive',linestyle='--')
ax.plot([r_capton,r_max_acc],[-z_acc,-z_acc],
        color='olive',linestyle='--')
ax.plot([r_capton,r_max_acc],[z_acc,z_acc],
        color='olive',linestyle='--')

ax.axvspan(0.,r_detector,
           (-w_detector+z_max)/(2.*z_max),(w_detector+z_max)/(2.*z_max),
           color="black",label="Positron detector",alpha=.8,hatch="//")
ax.set_xlabel('r [mm]',fontsize=fontsize)
ax.set_ylabel('z [mm]',fontsize=fontsize)
ax.set_aspect('equal')
#ax.get_xaxis().set_tick_params(pad=5)
ax.xaxis.set_major_locator(MaxNLocator(3))
ax.tick_params(axis='both',labelsize=fontsize)
#ax.legend(fontsize=fontsize,bbox_to_anchor=(0,1),loc='upper left')
plt.show()







