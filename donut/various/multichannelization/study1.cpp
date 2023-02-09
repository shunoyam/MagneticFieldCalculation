
#include"../../src/include.h"
using namespace std;

void study20221006()
{
  Bs bale("bale");
  ofstream of("20221006.dat");

  for(int i=0;i<bale.MFEPs();i++)
    {
      Bfield b=bale.at(i);
      auto mfep=b.MFEP();
      if(abs(mfep.y())<1.e-6 and mfep.x()>0)
        {
          of<<mfep.x()<<"\t"<<mfep.y()<<"\t"<<mfep.z()<<"\t"<<b.UpperOrLower()<<endl;
        }
    }





  
}


















