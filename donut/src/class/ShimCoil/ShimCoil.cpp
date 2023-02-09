

#include"../../include.h"

using namespace std;

ShimCoil::ShimCoil(string type_ShimCoil)
{
  this->_RotationalDivisionNumber=RotationalDivisionNumber;
  double CentralAngle=TMath::TwoPi()/this->_RotationalDivisionNumber;
  if(type_ShimCoil=="Bz")
    {
      for(int i_ShimCoil=0;i_ShimCoil<_RotationalDivisionNumber;i_ShimCoil++)
        {
          double center_theta=TMath::TwoPi()*i_ShimCoil/this->_RotationalDivisionNumber;
          shim_coil shim_top(radius_ShimCoil_Bz,
                             top_ShimCoil_Bz,bottom_ShimCoil_Bz,
                             center_theta,CentralAngle);
          this->_shim_coil.push_back(shim_top);
          shim_coil shim_bottom(radius_ShimCoil_Bz,
                                -bottom_ShimCoil_Bz,-top_ShimCoil_Bz,
                                center_theta,CentralAngle);
          this->_shim_coil.push_back(shim_bottom);
        }
    }
  else if(type_ShimCoil=="Br")
    {
      for(int i_ShimCoil=0;i_ShimCoil<_RotationalDivisionNumber;i_ShimCoil++)
        {
          double center_theta=TMath::TwoPi()*i_ShimCoil/_RotationalDivisionNumber;
          shim_coil shim_top(radius_ShimCoil_Br,
                             top_ShimCoil_Br,bottom_ShimCoil_Br,
                             center_theta,CentralAngle);
          this->_shim_coil.push_back(shim_top);
          shim_coil shim_bottom(radius_ShimCoil_Br,
                                -bottom_ShimCoil_Br,-top_ShimCoil_Br,
                                center_theta,CentralAngle);
          this->_shim_coil.push_back(shim_bottom);
        }
    }
  else
    {
      cout<<"shim coil type '"<<type_ShimCoil<<"' is invalid"<<endl;
      cout<<"choose 'Bz' or 'Br'"<<endl;
      return;
    }
  this->_type_ShimCoil=type_ShimCoil;
}

ShimCoil::operator vector<shim_coil>()
{
  return _shim_coil;
}

TVector3 ShimCoil::B(TVector3 MFEP)
{
  TVector3 _B;
  for(shim_coil shim:this->_shim_coil)
    {
      _B+=shim.B(MFEP);
    }
  return _B;  
}

void ShimCoil::SetRadialShift(double shift_r)
{
  this->_RadialShift=shift_r;
}

double ShimCoil::RadialShift()
{
  return this->_RadialShift;
}

void ShimCoil::ApplySameCurrent(double current)
{
  int n_shim=this->_RotationalDivisionNumber;
  vector<double> currents_ref(n_shim,current);
  ShimCoil::SetCurrents(currents_ref);
}

void ShimCoil::SetCurrents(vector<double> currents)
{
  int n_shim=this->_RotationalDivisionNumber;
  if(currents.size()!=n_shim)
    {
      cout<<"Number of currents is different from that of shim coils";
      return;
    }
  int i_shim_actual=0;
  for(int i_shim=0;i_shim<n_shim;i_shim++)
    {
      this->_shim_coil[i_shim_actual].SetCurrent(currents[i_shim]);
      i_shim_actual++;
      if(this->_type_ShimCoil=="Bz")
        {
          this->_shim_coil[i_shim_actual].SetCurrent(-currents[i_shim]);
        }
      if(this->_type_ShimCoil=="Br")
        {
          this->_shim_coil[i_shim_actual].SetCurrent(currents[i_shim]);
        }
      i_shim_actual++;
    }
}

vector<double> ShimCoil::GetCurrents()
{
  vector<double> currents;
  for(shim_coil saddle: this->_shim_coil)
    {
      currents.push_back(saddle.GetCurrent());
    }
  return currents;
}

void ShimCoil::SetCurrent(int i_coil,double current)
{
  if(i_coil<0 || this->_RotationalDivisionNumber<=i_coil)
    {
      cout<<"No."<<i_coil<<" coil is invalid"<<endl;
      return;
    }
  int i_shim_actual=i_coil*2;
  this->_shim_coil[i_shim_actual].SetCurrent(current);
  if(this->_type_ShimCoil=="Bz")
    {
      this->_shim_coil[i_shim_actual+1].SetCurrent(-current);
    }
  if(this->_type_ShimCoil=="Br")
    {
      this->_shim_coil[i_shim_actual+1].SetCurrent(current);
    }
}

void ShimCoil::InputRadialShift(int i_coil,double shift)
{
  if(i_coil<0 || this->_RotationalDivisionNumber*2<=i_coil)
    {
      cout<<"No."<<i_coil<<" coil is invalid"<<endl;
      return;
    }
  this->_shim_coil[i_coil].InputRadialShift(shift);  
}

void ShimCoil::InputAxialShift(int i_coil,double shift)
{
  if(i_coil<0 || this->_RotationalDivisionNumber*2<=i_coil)
    {
      cout<<"No."<<i_coil<<" coil is invalid"<<endl;
      return;
    }
  this->_shim_coil[i_coil].InputAxialShift(shift);  
}

void ShimCoil::InputRotationalShift(int i_coil,double shift)
{
  if(i_coil<0 || this->_RotationalDivisionNumber*2<=i_coil)
    {
      cout<<"No."<<i_coil<<" coil is invalid"<<endl;
      return;
    }
  this->_shim_coil[i_coil].InputRotationalShift(shift);  
}

void ShimCoil::InputTilt(int i_coil,double theta,double phi,double psi)
{
  if(i_coil<0 || this->_RotationalDivisionNumber*2<=i_coil)
    {
      cout<<"No."<<i_coil<<" coil is invalid"<<endl;
      return;
    }
  this->_shim_coil[i_coil].InputTilt(theta,phi,psi);
}

void ShimCoil::InputRadialShifts(vector<double> shifts)
{
  if(shifts.size()!=this->_RotationalDivisionNumber*2)
    {
      cout<<"size of argument is invalid"<<endl;
      return;
    }
  for(int i_shim=0;i_shim<shifts.size();i_shim++)
    {
      double shift=shifts[i_shim];
      ShimCoil::InputRadialShift(i_shim,shift);
    }
}

void ShimCoil::InputAxialShifts(vector<double> shifts)
{
  if(shifts.size()!=this->_RotationalDivisionNumber*2)
    {
      cout<<"size of argument is invalid"<<endl;
      return;
    }
  for(int i_shim=0;i_shim<shifts.size();i_shim++)
    {
      double shift=shifts[i_shim];
      ShimCoil::InputAxialShift(i_shim,shift);
    }
}

void ShimCoil::InputRotationalShifts(vector<double> shifts)
{
  if(shifts.size()!=this->_RotationalDivisionNumber*2)
    {
      cout<<"size of argument is invalid"<<endl;
      return;
    }
  for(int i_shim=0;i_shim<shifts.size();i_shim++)
    {
      double shift=shifts[i_shim];
      ShimCoil::InputRotationalShift(i_shim,shift);
    }
}

void ShimCoil::InputTilts(vector<double> thetas,vector<double> phis,vector<double> psis)
{
  if(thetas.size()!=this->_RotationalDivisionNumber*2)
    {
      cout<<"size of argument is invalid"<<endl;
      return;
    }
  for(int i_shim=0;i_shim<thetas.size();i_shim++)
    {
      double theta=thetas[i_shim];
      double phi=phis[i_shim];
      double psi=psis[i_shim];
      ShimCoil::InputTilt(i_shim,theta,phi,psi);
    }
}

void ShimCoil::Perfect()
{
  for(shim_coil& shim: this->_shim_coil)
    {
      shim.TakeDownFlag();
    }
}

void ShimCoil::InputTolerances(T_ShimCoil tolerances)
{
  int n_shim=this->_shim_coil.size();

  ShimCoil::InputRadialShifts(tolerances.r_shifts());
  ShimCoil::InputAxialShifts(tolerances.z_shifts());
  ShimCoil::InputTilts(tolerances.theta_tilts(),
                       tolerances.phi_tilts(),
                       tolerances.psi_tilts());
  ShimCoil::InputRotationalShifts(tolerances.theta_shifts());
}

void ShimCoil::InputTolerances_manual()
{
  int n_shim_coil=this->_shim_coil.size();
  for(int i_shim_coil=0;i_shim_coil<n_shim_coil;i_shim_coil++)
    {
      this->_shim_coil[i_shim_coil].OutputTolerances(i_shim_coil);
    }
}

shim_coil ShimCoil::Get1coil(int i_coil)
{
  return _shim_coil[i_coil];
}

void ShimCoil::MakeModes(int mode,string NormalOrSkew,double I_max,string method)
{
  if(method=="simple")
    {
  vector<double> Is;
  for(int i=0;i<RotationalDivisionNumber;i++)
    {
      double theta=TMath::TwoPi()*i/(double)RotationalDivisionNumber*(double)mode/2.;
      if(NormalOrSkew=="skew")
        {
          theta+=TMath::Pi()/2.;
        }
      double I=I_max*cos(theta);
      cout<<i<<"th coil: "<<cos(theta)<<endl;
      Is.push_back(I);
    }
  ShimCoil::SetCurrents(Is);
    }
  if(method=="svd")
    {
      ShimCoil::SimpleModeGeneration(mode,NormalOrSkew,I_max);
    }
}






















