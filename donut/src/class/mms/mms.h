

#pragma once

#include"../../include.h"
using namespace std;

class mms
//a set of magnetic moment like a lot of iron pieces for passive field shimming
{
 private:
  vector<mm> _mms;
  vector<double> _ModeStrength_vec;
  int _n_mode;
 public:
  mms(string ifn_dat="../config/shim_20220913.dat");
  void set(vector<mm> mms);
  void set(info_SVD info);
  void set(Eigen::VectorXd volume_vec);
  void set_volume(int i_shim,double volume);
  Eigen::VectorXd mms_eigen();
  void push_back(mm mm);
  void Push_Back(mms mms);
  operator vector<mm>();
  int n_shim();
  mm at(int index);
  double volume(int index);
  TVector3 position(int index);
  TVector3 B(TVector3 MagneticFieldEvaluaionPoint);
  double Flux(TVector3 MFEP);
  //scalar magnetic potential for drawing equal magnetic field lines.
  double TotalVolume(string unit="m3");// magnetomotive force of the mms.
  //i.e. absolutely sum of the mm durrents
  double AveragedVolume();
  void tilt(double theta_tilt,double phi_tilt);
  void dis(double x_displacement,double y_dis,double z_dis);
  void read(const char* path_to_magnet_data="../config/???");
  // geometry of shim tray here
  void draw(string ofn_root);
  void check_constraints(double v_min,double v_max,double unit_shim=unit_iron_shim_plate);
  int count(string range,
            double volume_min=volume_minimum_iron_shim+1.e-10,
            double volume_max=volume_maximum_iron_shim-1.e-10);
  //Choose "low","ok",or"high" as range 
  vector<double> mms_vec();
  void add_mms(mms mms1,mms mms2);
  void clear();
  double UseRate();
  void load(string ifn_dat);
  int n_stuck(string HighOrLow);
};

