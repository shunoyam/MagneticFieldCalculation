
#pragma once

#include"../../../src/include.h"
using namespace std;
using namespace ROOT::Math;


class BiotSavart
{
private:
  int _n_point;
  int _n_element;
  vector<XYZVector> _points_coil;
  vector<XYZVector> _MicroElements;
public:
  void UndulatedCoil(int n_bump,
                     double radius,double z,double deviation_radius,
                      double phase_MajorAxis,int n_point=1e2);
  TVector3 ResponseElement(TVector3 mfep);
  void arc(double r,double z,double angle_center,double CentralAngle,int n_point=1e7);
};

