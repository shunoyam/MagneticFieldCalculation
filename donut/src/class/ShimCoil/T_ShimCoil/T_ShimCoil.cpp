

#include"../../../include.h"
using namespace std;

T_ShimCoil::T_ShimCoil(TRandom2* random)
{
  int n_size=RotationalDivisionNumber*2;
  for(int i_size=0;i_size<n_size;i_size++)
    {
      double theta=get_random_discrete(-this->range_angle,
                                       this->range_angle,
                                       random);
      double psi=get_random_discrete(-this->range_angle,
                                       this->range_angle,
                                       random);
      double phi=get_random_discrete(-this->range_angle,
                                     this->range_angle,
                                     random);
      double RadialShift=get_random_discrete(-this->range_position,
                                             this->range_position,
                                             random);
      double AxialShift=get_random_discrete(-this->range_position,
                                             this->range_position,
                                             random);
      double RotationalShift=get_random_discrete(-this->range_angle,
                                                 this->range_angle,
                                                 random);
      this->_theta_tilt_vec.push_back(theta);
      this->_phi_tilt_vec.push_back(phi);
      this->_psi_tilt_vec.push_back(psi);
      this->_r_shift_vec.push_back(RadialShift);
      this->_z_shift_vec.push_back(AxialShift);
      this->_theta_shift_vec.push_back(RotationalShift);
    }
}

T_ShimCoil::T_ShimCoil()
{
  int n_size=RotationalDivisionNumber*2;
  vector<double> tilt_vec(n_size,0.);
  this->_theta_tilt_vec=tilt_vec;
  this->_phi_tilt_vec=tilt_vec;
  this->_psi_tilt_vec=tilt_vec;
  this->_r_shift_vec=tilt_vec;
  this->_z_shift_vec=tilt_vec;
  this->_theta_shift_vec=tilt_vec;
}

void T_ShimCoil::out()
{
  int n_size=RotationalDivisionNumber*2;
  cout<<"zenith angle[mrad]  azimuthal angle[rad] ";
  cout<<" radial shift[mm]  axial shift[mm] rotational shift[mrad]"<<endl;
  for(int i_size=0;i_size<n_size;i_size++)
    {
      double theta=this->_theta_tilt_vec[i_size];
      double psi=this->_theta_tilt_vec[i_size];
      double phi=this->_theta_tilt_vec[i_size];
      double RadialShift=this->_r_shift_vec[i_size];
      double AxialShift=this->_z_shift_vec[i_size];
      double RotationalShift=this->_theta_shift_vec[i_size];
      cout<<theta*1.e3<<"\t"<<phi<<"\t"<<psi<<"\t";
      cout<<RadialShift*1.e3<<"\t"<<AxialShift*1.e3<<"\t"<<RotationalShift*1.e3<<endl;
    }
}

double T_ShimCoil::GetPhi(int i_shim)
{
  return this->_phi_tilt_vec[i_shim];
}

double T_ShimCoil::GetPsi(int i_shim)
{
  return this->_psi_tilt_vec[i_shim];
}

double T_ShimCoil::GetTheta(int i_shim)
{
  return this->_theta_tilt_vec[i_shim];
}

double T_ShimCoil::z_shift(int i_shim)
{
  return this->_z_shift_vec[i_shim];
}

double T_ShimCoil::r_shift(int i_shim)
{
  return this->_r_shift_vec[i_shim];
}

double T_ShimCoil::theta_shift(int i_shim)
{
  return this->_theta_shift_vec[i_shim];
}

vector<double> T_ShimCoil::phi_tilts()
{
  return this->_phi_tilt_vec;
}

vector<double> T_ShimCoil::psi_tilts()
{
  return this->_psi_tilt_vec;
}

vector<double> T_ShimCoil::theta_tilts()
{
  return this->_theta_tilt_vec;
}

vector<double> T_ShimCoil::z_shifts()
{
  return this->_z_shift_vec;
}

vector<double> T_ShimCoil::r_shifts()
{
  return this->_r_shift_vec;
}

vector<double> T_ShimCoil::theta_shifts()
{
  return this->_theta_shift_vec;
}

void T_ShimCoil::reset()
{
  int n_size=_theta_tilt_vec.size();
  for(int i_size=0;i_size<n_size;i_size++)
    {
      this->_theta_tilt_vec[i_size]=0.;
      this->_phi_tilt_vec[i_size]=0.;
      this->_psi_tilt_vec[i_size]=0.;
      this->_r_shift_vec[i_size]=0.;
      this->_z_shift_vec[i_size]=0.;
      this->_theta_shift_vec[i_size]=0.;
    }
}

















