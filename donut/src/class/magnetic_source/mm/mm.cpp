

#include"../../../include.h"
using namespace std;

void mm::set(TVector3 _pos_mm,double volume,string material)
{
  set_pos(_pos_mm);
  set_volume(volume);
  magnetomotive_force=volume;
  set_material(material);
}

void mm::set_volume(double volume)
{
  this->magnetomotive_force=volume;
  magnetomotive_force=volume;
}

void mm::set_material(string material)
{
  if(material=="iron")
    {
      this->magnetization=magnetization_iron;
    }
  else
    {
      cout<<"What's this???"<<endl;
      cout<<"I don't know the material"<<endl;
    }
}

void mm::set_pos(TVector3 _pos_mm)
{
  this->pos_mm=_pos_mm;
}

void mm::setmagnetization(double _magnetization)
{
  this->magnetization=_magnetization;  
}

double mm::volume()
{
  return this->magnetomotive_force;  
}

TVector3 mm::pos()
{
  TVector3 _pos=this->pos_mm;
  return _pos;
}

TVector3 mm::ResponseElement(TVector3 pos_MFEP)
{
  pos_MFEP=procedure_for_manufacturing_error(pos_MFEP);
  TVector3 r_vec=pos_MFEP-this->pos_mm;
  double r=sqrt(r_vec.Mag2());
  double z=r_vec.z();
  TVector3 ez(0.,0.,1.);
  double coefficient=-mu_0/(4.*TMath::Pi()*pow(r,3));
  TVector3 _ResponseElement=(3.*z/pow(r,2))*r_vec;
  _ResponseElement=ez-_ResponseElement;
  _ResponseElement*=coefficient;
  _ResponseElement*=this->magnetization;
  return _ResponseElement;
}

double mm::ResponseElement_z(TVector3 pos_MFEP)
{
  TVector3 _ResponseElement=ResponseElement(pos_MFEP);
  double _ResponseElement_z=_ResponseElement.z();
  return _ResponseElement_z;
}

double mm::bz(TVector3 pos_MFEP)
{
  double _bz=this->b(pos_MFEP).z();
  return _bz;
}

double mm::flux(TVector3 pos_MFEP)
{
  pos_MFEP=procedure_for_manufacturing_error(pos_MFEP);
  TVector3 r_vec=pos_MFEP-this->pos_mm;
  double r=sqrt(r_vec.Mag2());
  if(r==0.)
    {
      return 0.;
    }
  double rho=sqrt(pow(r_vec.x(),2)+pow(r_vec.y(),2));
  double Aphi=mu_0*this->magnetization*magnetomotive_force*rho/(4.*TMath::Pi()*pow(r,3));
  double _flux=2*TMath::Pi()*rho*Aphi;
  return _flux;
}

void mm::VolumeRounding(double unit_iron_shim)
{
  int RoundedPlateNumber=this->magnetomotive_force/unit_iron_shim;
  double RoundOff=RoundedPlateNumber*unit_iron_shim;
  double RoundUp=RoundOff+unit_iron_shim;
  double distance_roundoff=abs(this->magnetomotive_force-RoundOff);
  double distance_roundup=abs(this->magnetomotive_force-RoundUp);
  if(distance_roundoff<=distance_roundup)
    {
      this->magnetomotive_force=RoundOff;
      return;
    }
  this->magnetomotive_force=RoundUp;
  return;  
}

void mm::clear()
{
  this->magnetomotive_force=0.;
}





















