
#pragma once

#include"../../../include.h"
using namespace std;

class T_ShimCoil
{
private:
  double range_position=0.1e-3;
  double range_angle=0.1e-3;

  vector<double> _theta_tilt_vec;
  vector<double> _phi_tilt_vec;
  vector<double> _psi_tilt_vec;

  vector<double> _r_shift_vec;
  vector<double> _z_shift_vec;
  vector<double> _theta_shift_vec;
public:
  T_ShimCoil(TRandom2* random);
  T_ShimCoil();
  void out();

  vector<double> psi_tilts();
  vector<double> phi_tilts();
  vector<double> theta_tilts();

  vector<double> z_shifts();
  vector<double> r_shifts();
  vector<double> theta_shifts();

  double GetPhi(int i_shim);
  double GetPsi(int i_shim);
  double GetTheta(int i_shim);

  double z_shift(int i_shim);
  double r_shift(int i_shim);
  double theta_shift(int i_shim);

  void reset();
};

