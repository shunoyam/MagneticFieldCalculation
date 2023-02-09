

#pragma once
#include"../../include.h"
using namespace std;

class ShimCoil
{
private:
  int _RotationalDivisionNumber;
  vector<shim_coil> _shim_coil;
  double _RadialShift;
  string _type_ShimCoil;
public:
  ShimCoil(string type_ShimCoil="Bz");// "Bz" or "Br"
  operator vector<shim_coil>();
  TVector3 B(TVector3 MFEP);
  void SetRadialShift(double shift_r);
  double RadialShift();
  void ApplySameCurrent(double current);//apply same currents to 12 coils
  void SetCurrents(vector<double> currents);//specify upper coil currents.
  vector<double> GetCurrents();
  //Sign is positive when the current flow
  //so as to turn right-hand screw against the direction of increasing radius.
  void SetCurrent(int i_coil,double current);
  void InputRadialShift(int i_coil,double shift);
  void InputAxialShift(int i_coil,double shift);
  void InputRotationalShift(int i_coil,double shift);
  void InputTilt(int i_coil,double theta,double phi,double psi);  
  void InputRadialShifts(vector<double> shifts);
  void InputAxialShifts(vector<double> shifts);
  void InputTilts(vector<double> thetas,vector<double> phis,vector<double> psis);
  void InputRotationalShifts(vector<double> thetas);
  void Perfect();
  void InputTolerances(T_ShimCoil tolerances);
  void drawB(const char* ofn_root);
  void InputTolerances_manual();
  shim_coil Get1coil(int i_coil);
  void fill_cylinder(double r,TH2D* h_br,TH2D* h_bz,int n_z,int n_theta);
  void draw_cylinder(TCanvas* c);
  void draw_xz(TCanvas* c);
  void MakeModes(int mode,string NormalOrSkew,double arg,string method);
  //For method "simple", arg=CurrentAmplitude.
  //For method "svd", arg=MagneticFieldAmplitude.
  void make_SVD(int mode,string NormalOrSkew,double TargetMagneticFieldAmplitude);
  Eigen::MatrixXd ResponseMatrix();
  void SimpleModeGeneration(int mode,string NormalOrSkew,double TargetAmplitude);
  void CalculateCurrents_SVD(Bs B_TG);
};




