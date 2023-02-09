#pragma once

#include"../../include.h"
using namespace std;

class Bs 
{
private:
  vector<Bfield> _B;
  double _r_center;
  double _r_width;
  double _z_center;
  double _z_width;
  int mesh1;
  int mesh2;
  double _min_Bz;
  double _max_Bz;
  double _min_Br;
  double _max_Br;
  double _min_Bx;
  double _max_Bx;
public:
  Bs();
  Bs(string shape);//shape="donut"
  Bs(string path,string unit_pos,string unit_B);
  operator vector<Bfield>();
  int MFEPs();
  int m1();
  int m2();
  double r_center();
  double r_width();
  double z_center();
  double z_width();
  double min(char component);
  double max(char component);
  Bfield at(int index);
  //return ith Bfield
  void push_back(TVector3 MFEP,TVector3 MagneticField=TVector3(0.,0.,0.),
                 double theta=0.);
  void push_back(TVector3 MFEP,int channel,double theta=0.,
                 TVector3 MagneticField=TVector3(0.,0.,0.));
  void setUniform(double Bz_target);
  void setonDSS(double diameter_DSS,int n_theta_MFEP,int n_phi_MFEP,
                double Bz_target=0.);
  double uniformity();
  double average();
  void subtract(cb coils);
  void addBs(Bs B_added,Bs B_add,double coefficient_added,double coefficient_add);
  void setonXZ(double r_center,double r_width,double z_center,double z_width,
               double phi=0.,int mesh_r=50,int mesh_z=50);
  //set magnetic field evaluation points gridly in rectangular on xz plane.
  void setonXY(double r_center,double r_width);
  void setonCS(double r_center,double z_width,int n_theta_MFEP=50,int n_z_MFEP=50,
               double Bz_target=0.);
  void peak2peakSubstitution();
  void add_coil(cb coils,double factor=1.);
  void add_coil(vector<cb> CBs,double factor=1.);
  void Add_MM(mms mms);
  void Add_MM_RM(Eigen::MatrixXd ReponseMatrix,mms mms);//RM means response matrix
  //from magnetic moment to magnetic field at MFEP
  
  void clear();
  Eigen::VectorXd Bs_eigen(string element="B");
  void Eigen_Bs(Eigen::VectorXd B_eigen);
  operator vector<double>();
  void read(const char* path_to_MFEP_data);
  void SetOnDonut(double Bz=0.);
  void add_shim_coil(shim_coil coil);
  void add_ShimCoil(ShimCoil sc);
  void DirectionBlinding();
  void ChangeB(int i_mfep,TVector3 NewB);
  void out(const char* ofn_dat);
  void OutOperaMap(string ofn_root);
  void reverse();
  void SetOnBale();
};


