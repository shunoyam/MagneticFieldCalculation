

#include"../../include.h"
using namespace std;
using namespace ROOT::Math;



void Bs::add_shim_coil(shim_coil sc)
{
  for(Bfield& b:this->_B)
    {
      TVector3 pos=b.MFEP();
      TVector3 B_add=sc.B(pos);
      TVector3 B_old=b.B();
      TVector3 B_new=B_old+B_add;
      b.setB(B_new);
    }
  Bs::peak2peakSubstitution();
}


void Bs::add_ShimCoil(ShimCoil sc)
{
  auto SaddleCoils=(vector<shim_coil>)sc;
  for(shim_coil SaddleCoil:SaddleCoils)
    {
      Bs::add_shim_coil(SaddleCoil);
    }
}




