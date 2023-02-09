
#include"../../../src/include.h"
using namespace std;

void PrepareFieldMap()
{
  Bs donut;
  donut.SetOnDonut();
  vector<Bfield> donut_vec=donut;
  ofstream of("FieldMap.table");
  int n_ShimmingPoint=donut.MFEPs();
  int n_r=30;
  int n_theta=50;
  int n_z=50;
  double r_center=33.3e-2;//m
  double r_width=2.e-2;
  double z_width=8.e-2;
  int n_point_vane=n_r*n_theta*n_theta;
  int n_point=n_ShimmingPoint+n_point_vane;
  of<<n_point<<" 1 1 2"<<endl;
  of<<"1 X [cm]"<<endl;
  of<<"2 Y [cm]"<<endl;
  of<<"3 Z [cm]"<<endl;
  of<<"0"<<endl;
  for(int i_ShimmingPoint=0;i_ShimmingPoint<n_ShimmingPoint;i_ShimmingPoint++)
    {
      TVector3 mfep=donut_vec[i_ShimmingPoint].MFEP();
      mfep*=1.e2;//m->cm
      of<<mfep.x()<<" "<<mfep.y()<<" "<<mfep.z()<<endl;
    }
  for(int i_r=0;i_r<n_r;i_r++)
    {
      for(int i_theta=0;i_theta<n_theta;i_theta++)
        {
          for(int i_z=0;i_z<n_z;i_z++)
            {
              double r=r_center+r_width*(-1.+2.*i_r/(n_r-1.));
              double theta=TMath::Pi()*(-1.+2.*i_theta/n_theta);
              double z=z_width*(-1.+2.*i_z/(n_z-1.));
              double x=r*cos(theta);
              double y=r*sin(theta);
              x*=1.e2;//m->cm
              y*=1.e2;//m->cm
              z*=1.e2;//m->cm
              of<<x<<" "<<y<<" "<<z<<endl;
            }
        }      
    }
}

void CenterPole()
{
  Bs IronDisc("../data/IronDisc.dat","cm","gauss");
  Bs AirDisc("../data/AirDisc.dat","cm","gauss");
  IronDisc.reverse();
  AirDisc.reverse();
  AirDisc.OutOperaMap("air.root");
  IronDisc.OutOperaMap("iron.root");
  Bs difference("../data/AirDisc.dat","cm","gauss");
  difference.addBs(IronDisc,AirDisc,1,-1);
  difference.OutOperaMap("difference.root");
  
}















