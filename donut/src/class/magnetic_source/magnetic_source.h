
#ifndef IN_MAGNETIC_SOURCE
#define IN_MAGNETIC_SOURCE

#include"../../include.h"


class magnetic_source
{
protected:
  int tilt=0;//yes=1,no=0
  double phi_tilt,theta_tilt;
  int displacement=0;
  int deformation=0;
  double _deformation_r=0.;
  double _deformation_z=0.;
  TVector3 pos_displacement;
  double magnetomotive_force;
  //current in the coil for "coil" class, volume of the object for "mm" class
public:
  void setdis(TVector3 pos);
  void settilt(double theta,double phi);
  void SetDeformation(double delta_r,double delta_z);
  TVector3 dis();
  double tilt_theta();
  double tilt_phi();
  double deformation_r();
  double deformation_z();
  virtual TVector3 ResponseElement(TVector3 pos_MFEP);
  virtual  TVector3 b(TVector3 pos_MFEP);
  //for showing magnetic field uniformity
  TVector3 procedure_for_manufacturing_error(TVector3 pos_MFEP);
  void switchoff_deformation();
  void switchoff_displacement();
  void switchoff_tilt();

};














#endif




