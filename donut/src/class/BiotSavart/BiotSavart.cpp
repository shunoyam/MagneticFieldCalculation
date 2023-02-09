

#include"../../../src/include.h"

using namespace std;
using namespace ROOT::Math;




void BiotSavart::UndulatedCoil(int n_bump,
                               double radius,double z,double deviation_r,
                                double phase_MajorAxis,int n_point)
{
  this->_n_point=n_point;
  this->_n_element=n_point-1;
  XYZVector point_coil_previous;
  for(int i_point=0;i_point<n_point;i_point++)
    {
      double theta=phase_MajorAxis+TMath::TwoPi()*i_point/(n_point-1.);
      double distance=radius+deviation_r*cos(n_bump*(theta-phase_MajorAxis));
      double x=distance*cos(theta);
      double y=distance*sin(theta);
      XYZVector point_coil(x,y,z);
      if(i_point)
        {
          XYZVector interval=point_coil-point_coil_previous;
          XYZVector midpoint=(point_coil+point_coil_previous)/2.;
          this->_points_coil.push_back(midpoint);
          this->_MicroElements.push_back(interval);
        }
      point_coil_previous=point_coil;
    }
}

TVector3 BiotSavart::ResponseElement(TVector3 mfep)
{
  auto mfep_XYZVector=TVector3_XYZVector(mfep);
  XYZVector _ResponseElement;
  for(int i_element=0;i_element<this->_n_element;i_element++)
    {
      XYZVector distance=mfep_XYZVector-this->_points_coil[i_element];
      double distance_mag=distance.Mag2();
      XYZVector _response_element=this->_MicroElements[i_element].Cross(distance);
      _response_element*=pow(distance_mag,-1.5);
      _ResponseElement+=_response_element;
    }
  double MagneticConstant=mu_0/(4.*TMath::Pi());
  _ResponseElement*=MagneticConstant;
  auto _ResponseElement_TVector3=XYZVector_TVector3(_ResponseElement);
  return _ResponseElement_TVector3;
}

void BiotSavart::arc(double r,double z,double angle_center,double CentralAngle,
                     int n_point)
{
  this->_n_point=n_point;
  this->_n_element=n_point-1;
  XYZVector point_coil_previous;
  for(int i_point=0;i_point<n_point;i_point++)
    {
      double theta=angle_center+CentralAngle*(-.5+(double)i_point/(n_point-1.));
      double x=r*cos(theta);
      double y=r*sin(theta);
      XYZVector point_coil(x,y,z);
      if(i_point)
        {
          XYZVector interval=point_coil-point_coil_previous;
          XYZVector midpoint=(point_coil+point_coil_previous)/2.;
          this->_points_coil.push_back(midpoint);
          this->_MicroElements.push_back(interval);
        }
      point_coil_previous=point_coil;
    }
}





