

#include"../../../src/include.h"
using namespace std;


void draw_CylindricalSurface(TH2D* h,magnet m,magnet* m2)
{
  SetStyle_TH2D(h);
  ofstream ofn_spare_dat;
  ofn_spare_dat.open("spare.dat",ios_base::app);
  ofn_spare_dat<<fixed<<setprecision(15)<<endl;
  int n=100;
  int n_theta=n;
  int n_z=n;
  for(int i_theta=0;i_theta<n_theta;i_theta++)
    {
      cout<<i_theta<<"/"<<n_theta<<endl;
      double theta=TMath::Pi()*(-1.+2.*i_theta/(n_theta-1.));
      for(int i_z=0;i_z<n_z;i_z++)
        {
          double z=z_width_fiducial*(-1.+2.*i_z/(n_z-1.));
          double x=r_center_fiducial*cos(theta);
          double y=r_center_fiducial*sin(theta);      
          TVector3 mfep(x,y,z);
          double Bz=m.Bz(mfep);
          Bz-=3.;
            if(m2)
              {
                Bz+=3.;
                double Bz2=m2->Bz(mfep);
                Bz=abs(Bz-Bz2);
              }
          h->Fill(theta,100.*z,Bz);
          ofn_spare_dat<<theta<<"\t"<<100.*z<<"\t"<<Bz<<endl;
        }
    }
  ArrangeRange(h);
  ofn_spare_dat<<endl;
  ofn_spare_dat<<endl;
}












