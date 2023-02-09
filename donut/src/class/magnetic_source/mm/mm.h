
#ifndef IN_MM
#define IN_MM

#include"../../../include.h"
using namespace std;

class mm : public magnetic_source
{
  //magnetic moment
private:
  double magnetization;//[A/m]
  TVector3 pos_mm;
  string _material;
  //magnetic moment of the object is (magnetization)*(volume)
  //Magnetization is peculiar to the material of the material
public:
  void set(TVector3 _pos_mm,double volume=0.,string material="iron");
  void set_volume(double volume);
  void set_material(string material);
  void set_pos(TVector3 _pos_mm);
  void setmagnetization(double _magnetization);
  double volume();
  TVector3 pos();
  TVector3 ResponseElement(TVector3 pos_MFEP) override;
  double ResponseElement_z(TVector3 pos_MFEP);
  double bz(TVector3 pos_MFEP);
  double flux(TVector3 pos_MFEP);
  void VolumeRounding(double unit_iron_shim);
  void clear();
};



//For now, magnetic moment is supposed to be parallel to +z, i.e magnetic field.



#endif













