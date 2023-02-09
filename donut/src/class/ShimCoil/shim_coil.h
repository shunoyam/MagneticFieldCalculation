
#pragma once 

#include"../../include.h"
using namespace std;

class shim_coil
{
private:
  double _radius;
  double _z_top;
  double _z_bottom;
  double _center_angle;
  double _CentralAngle;
  double _I;
  int flag_RadialShift=0;
  int flag_AxialShift=0;
  int flag_RotationalShift=0;
  int flag_tilt=0;
  double _RadialShift;
  double _AxialShift;
  double _RotationalShift;
  double _theta_tilt;
  double _phi_tilt;
  double _psi_tilt;
  TVector3 _CenterOfMass;
public:
  shim_coil();
  shim_coil(double radius,
            double z_top,double z_bottom,
            double _center_angle,double CentralAngle,double current=0.);
  void set(double radius,
           double z_top,double z_bottom,
           double _center_angle,double CentralAngle,double current=0.);
  //arc filament section
  double function_3(TVector3 MFEP,double _phi,double z_arc);
  double function_4(TVector3 MFEP,double _phi,double z_arc);
  double function_5(TVector3 MFEP,double _phi,double z_arc);
  TVector3 IntegralFunction(TVector3 MFEP,double _phi,double z_arc);
  TVector3 ResponseElement_arc(TVector3 MFEP,double z_arc);

  //line segment filament section
  TVector3 ResponseElement_LineSegment(TVector3 MFEP,double phi_line);

  //combine arc and line segment
  TVector3 ResponseElement(TVector3 MFEP);
  TVector3 B(TVector3 MFEP);
  void SetCurrent(double I);
  double GetCurrent();
  double I();
  double GetRadius();
  double GetZtop();
  double GetZbottom();
  double GetPhi_upper();
  double GetPhi_lower();
  
  void InputRadialShift(double shift);
  void InputAxialShift(double shift);
  void InputRotationalShift(double shift);
  void InputTilt(double theta,double phi,double psi);
  void TakeDownFlag();
  TVector3 procedure_for_tilt(TVector3 MFEP);
  void OutputTolerances(int i_shim_coil);
  TVector3 settlement_for_tilt(TVector3 ResponseElement);
  void SetHeight(double LowerEnd,double UpperEnd);
};

