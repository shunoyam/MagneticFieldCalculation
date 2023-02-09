
#pragma once

#include"../../include.h"
using namespace std;

class magnet
{
private:
  vector<cb> MainCoilsAndIronYoke;
  double _TargetField;
  double _cost_previous=1.e10;
  double _cost=1.e10;
  int n_scan_target=30;
  double best_i_scan_target;
  double _U_lc;
  double _U_theta;
  double TotalVolume_iron;
  mms* _IronShim=0;
public:
  void ReadCBsData(const char* path_to_magnet_data="../config/M20210727-Abe.DAT");
  void ReadCBsData_sub();
  magnet();
  operator vector<cb>();
  cb CombinedCB();
  cb at(int i_cb);
  cb GetIronYoke();
  cb GetMC10();
  cb GetMC11();
  cb GetMC20();
  cb GetMC21();
  cb GetMC30();
  void Perfect();
  void tilt(int i_coil,double theta_tilt,double phi_tilt);
  void dis(int i_coil,TVector3 displacement);
  void deformation(int i_coil,//coil number begins from 1 and ends at 6
                   double deformation_in,double deformation_out,double deformation_thick);
  //equal expansion at both ends is input like
  //  _magnet.deformation(1,-dw,dw,0);
  //radius error is input like
  // _magnet.deformation(1,dw,dw,0);
  void InputTolerance(vector<double> theta_vec,
                      vector<double> phi_vec,
                      vector<TVector3> dis_vec,
                      vector<double> deformation_inner_vec,
                      vector<double> deformation_outer_vec,
                      vector<double> deformation_thickness_vec);
  void InputTolerances_manual(double level=.1e-3);
  //tolerance is +/-level m or rad
  ~magnet();
  TVector3 B(TVector3 mfep);
  double Bz(TVector3 mfep);
  void deformation_elliptical(int i_coil,double deviation_radius,double phase_MajorAxis);
  //major axis :r+deviation_radius
  //minor axis :r-deviation_radius
  void deformation_benz(int i_coil,double deviation_radius,double phase_MajorAxis);
  void deformation_cross(int i_coil,double deviation_radius,double phase_MajorAxis);
  void OvalDeformation(int i_coil,double deviation_radius,double phase_MajorAxis,int mode);
  double TotalCurrent();
  void CostCalculation(mms*,int n_mode=TruncatedEigenmode);
  void shimming_iron(mms*,int n_mode=TruncatedEigenmode);
  double CostFunction();
  double TotalVolume();
  void OptimizeIronShim(mms*,int n_mode=TruncatedEigenmode);
  void SetTargetField(double target);
  double U_lc();
  double U_theta();
  void ActiveShimming(mms,ShimCoil*);
  void DrawBzOnCylinder(TCanvas*,int i_canvas);
  void InputRandomTolerances(TRandom2*);
};




