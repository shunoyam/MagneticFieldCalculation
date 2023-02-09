

#include"../../include.h"
using namespace std;

shim_coil::shim_coil()
{

}

shim_coil::shim_coil(double radius,
                     double z_top,double z_bottom,
                     double center_angle,double CentralAngle,double current)
{
  shim_coil::set(radius,z_top,z_bottom,center_angle,CentralAngle,current);
}

void shim_coil::set(double radius,
               double z_top,double z_bottom,
               double center_angle,double CentralAngle,double current)
{
    if(z_top<z_bottom)
    {
      cout<<"Auguments about axial placement are invalid"<<endl;
      cout<<"2nd augument "<<z_top<<"must be larger than 3rd augument "<<z_bottom<<endl;
      return;
    }
  this->_radius=radius;
  shim_coil::SetHeight(z_bottom,z_top);
  this->_center_angle=center_angle;
  this->_CentralAngle=CentralAngle;
  this->_I=current;
  //calculate center of mass(COM) of shim coil
  double numerator_x_COM=_radius*(_radius+cos(_CentralAngle/2.)*(_z_top-_z_bottom));
  double dominator_x_COM=_CentralAngle*_radius+(_z_top-_z_bottom);
  double x_COM=numerator_x_COM/dominator_x_COM;
  TVector3 COM(x_COM*cos(_center_angle),x_COM*sin(_center_angle),(_z_top+_z_bottom)/2.);
  _CenterOfMass=COM;
}

double shim_coil::function_3(TVector3 MFEP,double _phi,double z_arc)
{
  double r_=shim_coil::GetRadius();
  double z=MFEP.z()-z_arc;
  double rho_MFEP=hypot(MFEP.x(),MFEP.y());
  double phi_MFEP=atan2(MFEP.y(),MFEP.x());
  double dominator=pow(r_,2)+pow(rho_MFEP,2)+pow(z,2);
  dominator-=2.*rho_MFEP*r_*cos(_phi-phi_MFEP);
  double _function_3=pow(dominator,-0.5);
  return _function_3;
}

double shim_coil::function_4(TVector3 MFEP,double _phi,double z_arc)
{
  double r_=shim_coil::GetRadius();
  double z=MFEP.z()-z_arc;
  double phi_MFEP=atan2(MFEP.y(),MFEP.x());
  double rho_MFEP=hypot(MFEP.x(),MFEP.y());
  double dominator=pow(pow(r_+rho_MFEP,2)+z*z,1.5);
  double numerator=pow(rho_MFEP,2)+pow(r_,2)+z*z;
  double coefficient_3rd=numerator/dominator;
  double arg_phase=(_phi-phi_MFEP-TMath::Pi())/2.;
  double m=4.*rho_MFEP*r_/(pow(rho_MFEP+r_,2)+z*z);
  m=sqrt(m);
  dominator=hypot(r_+rho_MFEP,z);
  double coefficient_1st=1/dominator;
  double _function_4=coefficient_3rd*ellint_3_lib(m,m*m,arg_phase);
  _function_4-=coefficient_1st*ellint_1_lib(m,arg_phase);
  return _function_4;
}

double shim_coil::function_5(TVector3 MFEP,double _phi,double z_arc)
{
  double r_=shim_coil::GetRadius();
  double z=MFEP.z()-z_arc;
  double phi_MFEP=atan2(MFEP.y(),MFEP.x());
  double rho_MFEP=hypot(MFEP.x(),MFEP.y());
  double dominator=pow(pow(r_+rho_MFEP,2)+z*z,1.5);
  double numerator=pow(rho_MFEP,2)-pow(r_,2)+z*z;
  double coefficient_3rd=-numerator/dominator;
  double arg_phase=(_phi-phi_MFEP-TMath::Pi())/2.;
  double m=4.*rho_MFEP*r_/(pow(rho_MFEP+r_,2)+z*z);
  m=sqrt(m);
  dominator=hypot(r_+rho_MFEP,z);
  double coefficient_1st=1/dominator;
  double _function_5=coefficient_3rd*ellint_3_lib(m,m*m,arg_phase);
  _function_5+=coefficient_1st*ellint_1_lib(m,arg_phase);
  return _function_5;
}

TVector3 shim_coil::IntegralFunction(TVector3 MFEP,double _phi,double z_arc)
{
  double z=MFEP.z()-z_arc;
  double rho_MFEP=hypot(MFEP.x(),MFEP.y());
  double phi_MFEP=atan2(MFEP.y(),MFEP.x());
  double Bx,By,Bz;
  Bx=cos(phi_MFEP)*function_4(MFEP,_phi,z_arc)+sin(phi_MFEP)*function_3(MFEP,_phi,z_arc);
  Bx*=z/rho_MFEP;
  By=-cos(phi_MFEP)*function_3(MFEP,_phi,z_arc)+sin(phi_MFEP)*function_4(MFEP,_phi,z_arc);
  By*=z/rho_MFEP;
  Bz=function_5(MFEP,_phi,z_arc);
  TVector3 _IntegralFunction(Bx,By,Bz);
  return _IntegralFunction;
}

TVector3 shim_coil::ResponseElement_arc(TVector3 MFEP,double z_arc)
{
  TVector3 top=shim_coil::IntegralFunction(MFEP,
                                           this->_center_angle+this->_CentralAngle/2.,
                                           z_arc);
  TVector3 bottom=shim_coil::IntegralFunction(MFEP,
                                              this->_center_angle-this->_CentralAngle/2.,
                                              z_arc);
  TVector3 _ResponseElement=mu_0*(top-bottom);
  _ResponseElement=_ResponseElement*(1/(4.*TMath::Pi()));
  return _ResponseElement;
}

TVector3 shim_coil::ResponseElement_LineSegment(TVector3 MFEP,double phi_line)
{
  double r_=shim_coil::GetRadius();
  double z_top=shim_coil::GetZtop();
  double z_bottom=shim_coil::GetZbottom();
  double x_coil=r_*cos(phi_line);
  double y_coil=r_*sin(phi_line);
  TVector3 NearestVector(x_coil,y_coil,MFEP.z());
  TVector3 distance_TVector3=MFEP-NearestVector;
  double distance=distance_TVector3.Mag();
  TVector3 I_TVector3(0.,0.,1.);
  TVector3 direction=I_TVector3.Cross(distance_TVector3);
  direction=direction.Unit();
  TVector3 RelativeVector_top=MFEP-TVector3(x_coil,y_coil,z_top);
  TVector3 RelativeVector_bottom=MFEP-TVector3(x_coil,y_coil,z_bottom);
  double theta_top=distance_TVector3.Angle(RelativeVector_top);
  double theta_bottom=distance_TVector3.Angle(RelativeVector_bottom);
  double magnitude=mu_0*(sin(theta_top)+sin(theta_bottom))/(4.*TMath::Pi()*distance);
  TVector3 _ResponseElement=magnitude*direction;
  return _ResponseElement;
}

TVector3 shim_coil::ResponseElement(TVector3 MFEP)
{
  MFEP=shim_coil::procedure_for_tilt(MFEP);
  double z_top=shim_coil::GetZtop();
  double z_bottom=shim_coil::GetZbottom();
  auto a_top=shim_coil::ResponseElement_arc(MFEP,z_top);
  auto a_bottom=shim_coil::ResponseElement_arc(MFEP,z_bottom);
  double phi_upper=shim_coil::GetPhi_upper();
  double phi_lower=shim_coil::GetPhi_lower();
  auto a_UpperLine=shim_coil::ResponseElement_LineSegment(MFEP,phi_upper);
  auto a_LowerLine=shim_coil::ResponseElement_LineSegment(MFEP,phi_lower);
  auto _ResponseElement=-a_top+a_bottom+a_UpperLine-a_LowerLine;
  _ResponseElement=shim_coil::settlement_for_tilt(_ResponseElement);
  return _ResponseElement;
}

TVector3 shim_coil::settlement_for_tilt(TVector3 ResponseElement)
{
  if(flag_tilt)
    {
      ResponseElement.RotateZ(-this->_phi_tilt);
      ResponseElement.RotateY(-this->_theta_tilt);
      ResponseElement.RotateX(-this->_psi_tilt);
   }
  return ResponseElement;
}

TVector3 shim_coil::B(TVector3 MFEP)
{
  double shift=0.;//1.e-7;
  TVector3 ShiftToAvoidDivergence(shift,shift,shift);
  MFEP+=ShiftToAvoidDivergence;
  TVector3 _B=shim_coil::ResponseElement(MFEP);
  _B*=this->_I;
  return _B;
}

void shim_coil::SetCurrent(double I)
{
  this->_I=I;
}

double shim_coil::GetCurrent()
{
  return this->_I;
}

double shim_coil::I()
{
  return this->_I;
}

double shim_coil::GetRadius()
{
  double OriginalRadius=this->_radius;
  if(flag_RadialShift)
    {
      OriginalRadius+=this->_RadialShift;//or -=
    }
  return OriginalRadius;
}

double shim_coil::GetZtop()
{
  double z=this->_z_top;
  if(flag_AxialShift)
    {
      z+=this->_AxialShift;//or -=
    }
  return z;
}

double shim_coil::GetZbottom()
{
    double z=this->_z_bottom;
  if(flag_AxialShift)
    {
      z+=this->_AxialShift;//or -=
    }
  return z;
}

double shim_coil::GetPhi_upper()
{
  double phi=this->_center_angle+this->_CentralAngle/2.;
  if(flag_RotationalShift)
    {
      phi+=_RotationalShift;
    }
  return phi;
}

double shim_coil::GetPhi_lower()
{
  double phi=this->_center_angle-this->_CentralAngle/2.;
  if(flag_RotationalShift)
    {
      phi+=_RotationalShift;
    }
  return phi;
}

void shim_coil::InputRadialShift(double shift)
{
  this->flag_RadialShift=1;
  this->_RadialShift=shift;
}

void shim_coil::InputAxialShift(double shift)
{
  this->flag_AxialShift=1;
  this->_AxialShift=shift;
}

void shim_coil::InputRotationalShift(double shift)
{
  this->flag_RotationalShift=1;
  this->_RotationalShift=shift;
}

void shim_coil::InputTilt(double theta,double phi,double psi)
{
  this->flag_tilt=1;
  this->_theta_tilt=theta;
  this->_phi_tilt=phi;
  this->_psi_tilt=psi;
}

void shim_coil::TakeDownFlag()
{
  this->flag_RadialShift=0;
  this->flag_AxialShift=0;
  this->flag_RotationalShift=0;
  this->flag_tilt=0;
}

TVector3 shim_coil::procedure_for_tilt(TVector3 MFEP)
{
 if(flag_tilt)
   {
     TVector3 Relative=MFEP-this->_CenterOfMass;
     Relative.RotateX(this->_psi_tilt);
     Relative.RotateY(this->_theta_tilt);
     Relative.RotateZ(this->_phi_tilt);
     MFEP=_CenterOfMass+Relative;
   }
  return MFEP;
}

void shim_coil::OutputTolerances(int i_shim_coil)
{
  cout<<"this->_shim_coil["<<i_shim_coil<<"].";
  cout<<"InputRadialShift(";
  cout<<this->_RadialShift<<");"<<endl;

  cout<<"this->_shim_coil["<<i_shim_coil<<"].";
  cout<<"InputAxialShift(";
  cout<<this->_AxialShift<<");"<<endl;

  cout<<"this->_shim_coil["<<i_shim_coil<<"].";
  cout<<"InputTilt(";
  cout<<this->_theta_tilt<<","<<this->_phi_tilt<<");"<<endl;
}

void shim_coil::SetHeight(double LowerEnd,double UpperEnd)
{
  this->_z_bottom=LowerEnd;
  this->_z_top=UpperEnd;
}








