
#pragma once
#include"../../include.h"
using namespace std;

class Draw
{
private:
  double z_w=5.e-2;
  double r_w=1.5e-2;
  double r_c=33.3e-2;
  double z_c=0.;
  string ofn_root;
  int n_z=100;
  int n_theta=200;
  int n_r=30;
  int n_z_light=30;
  int n_theta_light=40;
  int n_r_light=5;
  TCanvas* c_xz;
  TCanvas* c_cylinder;
  cb _cb;
  ShimCoil _ShimCoil;
  magnet _magnet;
  mms _mms;
  string sources;
public:
  Draw(cb cb,string _ofn_root);
  Draw(ShimCoil _SC,string _ofn_root);
  Draw(magnet _m,string _ofn_root);
  Draw(magnet _m,mms _mms,string _ofn_root);
  Draw(magnet _m,mms _mms,ShimCoil _SC,string _ofn_root);
  Draw(mms _mms,string _ofn_root);
  void drawing();
  void cylinder(TH2D* h_in_bz,TH2D* h_center_bz,TH2D* h_out_bz,
                TH2D* h_in_br,TH2D* h_center_br,TH2D* h_out_br);
  void fill_cylinder(double r,TH2D* h_br,TH2D* h_bz);
  void xz(TH2D* h_xz_Br,TH2D* h_xz_Bz,TH2D* h_azi_Br,TH2D* h_azi_Bz);
  void fill_xz(TH2D* h_br,TH2D* h_bz);
  void fill_azi(TH2D* h_br,TH2D* h_bz);
  TVector3 B(TVector3 mfep);
  void T_ppm(TH2D* h);
};

