

#include"../../../src/include.h"
using namespace std;

void RippleStudy()
{
  magnet m;
  ofstream f("ripple_tilt.dat");
  double phi;//azimuthal angle
  double Bz;
  double Bz_error;
  double x,y;
  int n_point=100;
  TVector3 mfep;
  for(int i_point=0;i_point<n_point;i_point++)
    {
      phi=TMath::Pi()*(-1.+2.*i_point/(n_point-1.));
      x=r_center_fiducial*cos(phi);
      y=r_center_fiducial*sin(phi);      
      mfep.SetXYZ(x,y,0.);
      f<<phi<<"\t";
      for(int i_cb=0;i_cb<6;i_cb++)
        {
          m.tilt(i_cb,1.e-3,0.);
          Bz=m.B(mfep).z();
          Bz_error=Bz-3.;//[T]
          f<<Bz_error*1.e6<<"\t";
          m.Perfect();
        }
      f<<endl;  
    }
}

void DFT_ripple(Bs B,const char* ofn,int n_mode)
{
  int n_point=B.MFEPs();
  ofstream f(ofn);
  f<<fixed<<setprecision(15);
  ofstream ripple("RowRipple.dat");
  ofstream test("test.dat");
  for(int i_mode=0;i_mode<n_mode;i_mode++)
    {
      f<<i_mode<<"\t";
      double amp_normal=0.;
      double amp_skew=0.;
      for(int i_point=0;i_point<n_point;i_point++)
        {
          double theta=B.at(i_point).theta();
          double delta_Bz=B.at(i_point).B().z()-3.;
          amp_normal+=delta_Bz*cos(i_mode*theta);
          if(i_mode==8)
            {
              cout<<i_point<<"contribution :"<<delta_Bz*cos(i_mode*theta)<<endl;
              test<<delta_Bz*cos(i_mode*theta)<<endl;
            }
          amp_skew+=delta_Bz*sin(i_mode*theta);
          if(!i_mode)
            {
              ripple<<theta<<"\t"<<delta_Bz<<endl;
            }
        }
      amp_normal/=n_point;
      amp_skew/=n_point;
      double amp_absolute=hypot(amp_normal,amp_skew);
      f<<amp_normal<<"\t"<<amp_skew<<"\t"<<amp_absolute<<endl;
    }
}

void MultipoleExpansion_ripple()
{
  magnet m;
  m.dis(n_MC10,TVector3(1.e-3,0.,0.));
  int n_point=round(1.e3);
  Bs B_ripple;
  double r=r_center_fiducial;
  double phi;
  int n_mode=10;

  for(int i_point=0;i_point<n_point;i_point++)
    {
      phi=TMath::TwoPi()*i_point/(n_point-1);
      TVector3 mfep(r*cos(phi),r*sin(phi),0.);
      TVector3 B=m.B(mfep);
      B_ripple.push_back(mfep,B,phi);
    }
  const char* ofn="MultipoleExpansion_ripple.dat";
  ofstream f(ofn);
  DFT_ripple(B_ripple,ofn,n_mode);








  
}


