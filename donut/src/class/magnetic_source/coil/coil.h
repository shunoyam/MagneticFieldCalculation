
#pragma once
#include"../../../include.h"
using namespace std;

class coil : public magnetic_source
{//filament loop coil
private:
  double radius;
  double _Z;
  int RampUp=0;//flag
  double RampUpRate=1.;
  int _deformation_undulation=0;//flag
  double _deviation_radius;
  double _phase_MajorAxis;
  BiotSavart coil_ComplexForm;//for calculation in case of complex deformation
public:
  void RampUp_coil(double _RampUpRate);
  void set(double _I,double _r,double _z);
  coil();
  coil(double _I,double _r,double _z);
  void setI(double _I);
  void setr(double _r);
  void setz(double _z);
  double I();
  double r();
  double z();
  TVector3 ResponseElement(TVector3 pos_MFEP) override;
  //element of response matrix A
  TVector3 b(TVector3 pos_MFEP) override;
  double flux(TVector3 pos_MFEP);//magnetic flux(scalar)
  //for showing magnetic field uniformity
  void deformation_undulation(int n_bump,double deviation_radius,double phase_MajorAxis,
                              int n_element=1e4);
  void switchoff_undulation();
  void switchoff_RampUp();
  TVector3 ResponseElement_UndulatedCoil(TVector3 mfep);
};



















