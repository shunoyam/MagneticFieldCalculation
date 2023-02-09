

#include"../../include.h"

void magnetic_source::setdis(TVector3 pos)
{
  pos_displacement=pos;
  displacement=1;
}

void magnetic_source::settilt(double theta,double phi)
{
  theta_tilt=theta;
  phi_tilt=phi;
  tilt=1;
}

TVector3 magnetic_source::ResponseElement(TVector3 pos_MFEP)
{
  TVector3 response_element(0.,0.,0.);
  return response_element;
}

TVector3 magnetic_source::b(TVector3 pos_MFEP)
{
  TVector3 _B=magnetomotive_force*ResponseElement(pos_MFEP);
  return _B;
}

TVector3 magnetic_source::dis()
{
  return this->pos_displacement;
}

double magnetic_source::deformation_r()
{
  return this->_deformation_r;
}

double magnetic_source::deformation_z()
{
  return this->_deformation_z;
}

double magnetic_source::tilt_theta()
{
  return this->theta_tilt;
}

double magnetic_source::tilt_phi()
{
  return this->phi_tilt;  
}



TVector3 magnetic_source::procedure_for_manufacturing_error(TVector3 pos_MFEP)
{
  //procedure for design error
  if(displacement)
    {
      pos_MFEP=pos_MFEP-this->pos_displacement;
    }
  if(deformation)
    {
      TVector3 pos_deformation(0.,0.,this->_deformation_z);
      pos_MFEP=pos_MFEP-pos_deformation;
    }
  if(tilt)
    {
      pos_MFEP.RotateZ(-phi_tilt);
      pos_MFEP.RotateY(-theta_tilt);
    }
  return pos_MFEP;
}

void magnetic_source::SetDeformation(double _delta_r,double _delta_z)
{
  this->_deformation_r=_delta_r;
  this->_deformation_z=_delta_z;
  this->deformation=1;
}

void magnetic_source::switchoff_deformation()
{
  this->deformation=0;
}

void magnetic_source::switchoff_displacement()
{
  this->displacement=0;
}

void magnetic_source::switchoff_tilt()
{
  this->theta_tilt=0.;
  this->phi_tilt=0.;
  this->tilt=0;
}

