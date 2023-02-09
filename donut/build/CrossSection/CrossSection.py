
import matplotlib.pyplot as plt
import numpy as np


def draw_bar(ifn):
    InputFile=open(ifn)
    lines=InputFile.readlines()
    fig=plt.figure()
    ax=fig.add_subplot(projection='3d',xlabel='r [m]',ylabel='z [m]',zlabel='current [kA]')
    r=[]
    z=[]
    I=[]
    for line in lines:
        vals=line.split()
        r.append(float(vals[0]))#[m]
        z.append(float(vals[1]))#[m]
        I.append(float(vals[2])*1.e-3)#[kA]
    
    base_I=3.#[kA]
    bottom=np.ones_like(I)*base_I
    width=0.005#[m]
    ave_r=sum(r)/len(r)
    ave_z=sum(z)/len(z)
    ave_I=sum(I)/len(I)
    W=.1
    ax.set_xlim(ave_r-W,ave_r+W)
    ax.set_ylim(ave_z-W,ave_z+W)
    bar=ax.bar3d(r,z,bottom,width,width,I)
    ax.view_init(elev=40,azim=-50)
    MagnetName=ifn.replace('.dat','')
    font_dict = dict(family="serif",
                     color='black',
                     weight="bold",
                     size=16)
    adj=0
    if(ifn=='MC20.dat' or ifn=='MC21.dat' or ifn=='MC30.dat'):
        adj=10
    ax.text(ave_r,ave_z-W/2.,max(I)+20-adj,MagnetName,fontdict=font_dict)
    ofn_pdf=ifn.replace('dat','pdf')
    plt.subplots_adjust(left=0, right=.8, bottom=0.1, top=0.95)
    plt.savefig(ofn_pdf)

def main():
    numbers=['10','11','20','21','30']
    for number in numbers:
        ifn='MC'+number+'.dat'
        draw_bar(ifn)
        

main()
