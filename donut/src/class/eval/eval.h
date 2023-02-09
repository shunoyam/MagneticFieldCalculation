#pragma once

#include"../../include.h"
using namespace std;

class eval
{
private:
  int _n_mesh=10;

  double _min_Bz_azi;
  double _max_Bz_azi;
  double _min_Bz;
  double _ave_Bz;
  double _max_Bz;
  double _p2p_Bz;
  
  double _min_Br_azi;
  double _max_Br_azi;
  double _ave_Br;
  double _min_Br;
  double _max_Br;
  double _p2p_Br;

  double _ave_Bx;
  double _min_Bx;
  double _max_Bx;
  double _p2p_Bx;

  double _ave_By;
  double _min_By;
  double _max_By;
  double _p2p_By;
  int OutputInformation=0;
  vector<TVector3> _Bs_MFEP;
public:
  void evaluation(cb coils,int presence_coil=1,
                  int presence_IronShim=0,mms* shim=0,
                  int presence_ShimCoil=0,ShimCoil* sc=0);
  eval(cb coils);
  eval(magnet m);
  eval(cb coils,mms shim);
  eval(magnet m,mms shim);
  eval(ShimCoil sc);
  cb magnet_cb(magnet m);
  double min_Bz();
  double max_Bz();
  double min_Bx();//calculated from sin fit over azimuthal dependence
  double max_Bx();//calculated from sin fit over azimuthal dependence  
  double min_By();
  double max_By();
  double min_Br();
  double max_Br();
  double min_Bz_azi();
  double max_Bz_azi();
  double min_Br_azi();
  double max_Br_azi();
  double p2p_Br();//local
  double p2p_Bz();//local
  double p2p_Bx();
  double p2p_By();
  double p2p_Br_theta();//azimuthally averaged p-p
  double p2p_Bz_theta();//azimuthally averaged p-p
  double U_lc();
  double U_theta();
  double ave_Br();
  double ave_Bx();
  double ave_By();
  double ave_Bz();
  void out();
  int RequirementCheck();
  vector<TVector3> Bs_MFEP();
};



