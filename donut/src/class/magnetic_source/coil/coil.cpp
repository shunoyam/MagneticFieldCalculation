

#include"../../../include.h"

coil::coil()
{
  
}

void coil::RampUp_coil(double _RampUpRate)
{
  this->RampUpRate=_RampUpRate;
  this->RampUp=1;
}

void coil::set(double _I,double _r,double _z)
{
  this->radius=_r;
  magnetomotive_force=_I;
  this->_Z=_z;
}

coil::coil(double _I,double _r,double _z)
{
  coil::set(_I,_r,_z);
}

void coil::setI(double _I)
{
  magnetomotive_force=_I;
}

void coil::setr(double _r)
{
  this->radius=_r;
}

void coil::setz(double _z)
{
  this->_Z=_z;
}

double coil::I()
{
  double _I=magnetomotive_force;
  if(this->RampUp)
    {
      _I*=this->RampUpRate;
    }
  return _I;
}

double coil::r(){
  double _r=this->radius;
  if(deformation)
    {
      _r+=this->_deformation_r;
    }
  return _r;
}

double coil::z()
{
  double _z=this->_Z;
  if(displacement)
    {
      _z+=pos_displacement.z();
    }
  _z+=this->_deformation_z;
  return _z;
}


TVector3 coil::ResponseElement(TVector3 pos_MFEP)
{
  if(this->_deformation_undulation)
    {
      TVector3 res_UndulatedCoil=coil::ResponseElement_UndulatedCoil(pos_MFEP);
      return res_UndulatedCoil;
    }
  pos_MFEP=coil::procedure_for_manufacturing_error(pos_MFEP);
  //think with cylindrical coordinate
  double rho=sqrt(pow(pos_MFEP.x(),2)+pow(pos_MFEP.y(),2));
  double radi=coil::r();
  double  _z=pos_MFEP.z()-this->_Z;
  //exception handling for magnetic field on the z axis
  if (rho<1.e-8)
    {
      double dominator=2*pow(pow(radi,2)+pow(_z,2),1.5);
      double Response_element_z=mu_0*pow(radi,2)/dominator;
      TVector3 _ResponseElement(0.,0.,Response_element_z);
      if(tilt)
        {
          _ResponseElement.RotateY(theta_tilt);
          _ResponseElement.RotateZ(phi_tilt);
        }
      return _ResponseElement;
    }
  //calculate a_z and a_r by polynomial approximation for elliptic integral
  double k=(4.*radi*rho)/(pow(radi+rho,2)+pow(_z,2));
  double coef1_z=mu_0*sqrt(k)/(4.*TMath::Pi()*sqrt(radi*rho)); 
  double coef2_z_nume=(pow(radi,2)-pow(rho,2)-pow(_z,2));
  double coef2_z_domi=(pow(radi-rho,2)+pow(_z,2));
  double coef2_z=coef2_z_nume/coef2_z_domi;
  double a_z=coef1_z*(coef2_z*elliptic_integral_2(k)+elliptic_integral_1(k)); 
  double coef1_r=mu_0*_z*sqrt(k)/(4.*TMath::Pi()*rho*sqrt(radi*rho));
  double coef2_r=(pow(radi,2)+pow(rho,2)+pow(_z,2))/(pow(radi-rho,2)+pow(_z,2));
  double a_r=coef1_r*(coef2_r*elliptic_integral_2(k)-elliptic_integral_1(k));
 
  //from a_z and a_r to 3D
  TVector3 _Response_element(a_r*pos_MFEP.x()/rho,a_r*pos_MFEP.y()/rho,a_z);
  if(tilt)
    {
      _Response_element.RotateY(theta_tilt);
      _Response_element.RotateZ(phi_tilt);
    }
  return _Response_element;
}

TVector3 coil::b(TVector3 pos_MFEP)
{
  auto ResponseElement=coil::ResponseElement(pos_MFEP);
  TVector3 _B=this->magnetomotive_force*ResponseElement;
  if(this->RampUp)
    {
      _B*=this->RampUpRate;
    }
  return _B;
}

double coil::flux (TVector3 pos_MFEP)
{
  pos_MFEP=procedure_for_manufacturing_error(pos_MFEP);
  pos_MFEP.SetZ(pos_MFEP.z()-this->_Z);
  double rho=sqrt(pow(pos_MFEP.x(),2)+pow(pos_MFEP.y(),2));
  if(rho==0.)
    {
      return 0.;
    }
  double Delta=sqrt(pow(rho+this->radius,2.)+pow(pos_MFEP.z(),2.));
  double k=4.*this->radius*rho/pow(Delta,2.); 
  double coef=mu_0*magnetomotive_force*this->radius/(TMath::Pi()*Delta*k);
  double Aphi=-coef*((k-2.)*elliptic_integral_1(k)+2.*elliptic_integral_2(k));
  double flux=2.*TMath::Pi()*rho*Aphi;
  return flux;
}

void coil::deformation_undulation(int n_bump,
                                  double deviation_radius,double phase,int n_element)
{
  this->_deviation_radius=deviation_radius;
  this->_phase_MajorAxis=phase;
  this->_deformation_undulation=1;
  this->coil_ComplexForm.UndulatedCoil(n_bump,this->radius,this->_Z,
                                        this->_deviation_radius,this->_phase_MajorAxis,
                                        n_element);
}

void coil::switchoff_undulation()
{
  this->_deformation_undulation=0;
}

TVector3 coil::ResponseElement_UndulatedCoil(TVector3 mfep)
{
  TVector3 ResponseElement=this->coil_ComplexForm.ResponseElement(mfep);
  return ResponseElement;
}

void coil::switchoff_RampUp()
{
  this->RampUp=0;
}

