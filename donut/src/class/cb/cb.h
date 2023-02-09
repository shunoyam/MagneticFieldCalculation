
#pragma once

#include"../../include.h"
using namespace std;

class cb//coil block
{
private:
  vector<coil> _coils;
  double _center_r;
  double _center_z;
  double _r_in;
  double _r_out;
  double _z_upper;
  double _z_lower;
  double RampUpRate=0.;
public:
  cb();
  cb(vector<coil> coils);
  void RampUp_cb(double _RampUpRate);
  void set(vector<coil> coils);
  void WriteDownSides(double r_in,double r_out,double z_lower,double z_upper);
  void SetOnMesh(double r_in,double r_out,double height,int mesh_r,int mesh_z,
                 double TotalCurrent=0.,double center_z=0.);
  void push_back(coil coil);
  void Push_Back(cb coilblocks);
  coil at(int index);
  vector<coil> coil_vec();
  TVector3 B(TVector3 MagneticFieldEvaluaionPoint);
  double Bz(TVector3 MFEP);
  double Flux(TVector3 MFEP);
  //scalar magnetic potential for drawing equal magnetic field lines.
  double MMF();// magnetomotive force of the coils.
  //i.e. absolutely sum of the coil durrents
  void tilt(double theta_tilt,double phi_tilt);
  void dis(TVector3 displacement);
  void read(const char* path_to_magnet_data="../config/M20210727-Abe.DAT");
  void read_rz(const char* path_to_magnet_data);
  void draw(const char* outputfilename_png);
  void off_deformation();
  
  void off_displacement();

  void off_tilt();

  void off_deformation_shape();

  void off_RampUp();
  
  void off();
  
  void perfect();

  void deformation(double deformation_inner,double deformation_outer,
                   double deformation_thickness);

  void SetOnGaussNodes(double center_r,double width_r,int nodes_r,
                       double center_z,double width_z,int nodes_z,double CurrentDensity);
  //width is "half width"!!

  void deformation_shape(int n_bump,double deviation_radius,double phase_major);

  double z_lower();

  double TotalCurrent();
};

