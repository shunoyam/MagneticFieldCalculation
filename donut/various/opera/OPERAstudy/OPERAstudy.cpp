
#include"../../../src/include.h"
using namespace std;

void OnlyCoilFieldMap()
{
  PrepareOPERAlikeFile("CoilPerfect.table");
  PrepareOPERAlikeFile("CoilShift.table",1.e-3);
}

void PrepareOPERAlikeFile(string ofn_table,double shift)
{
  ifstream InputFile("CentralCylinder100-61.table");
  ofstream of(ofn_table.c_str());
  int n_prefix=5;
  for(int i_prefix=0;i_prefix<n_prefix;i_prefix++)
    {
      string line;
      getline(InputFile,line);
      if(i_prefix==n_prefix-1)
        {
          of<<"4 BX [GAUSS]\n5 BY [GAUSS]\n6 BZ [GAUSS]"<<endl;
        }
      of<<line<<endl;
    }
  double x,y,z;
  magnet m;
  of<<fixed<<setprecision(10);
  shift*=1.e2;//[m]->[cm]
  while(InputFile>>x>>y>>z)
    {
      TVector3 mfep(x-shift,y,z);//or +shift
      TVector3 mfep_m=mfep*1.e-2;//[cm]->[m]
      TVector3 b(0.,0.,0.);
      for(int i_coil=1;i_coil<6;i_coil++)
        {
          b-=m.at(i_coil).B(mfep_m)*1.e4;//[Gauss]
          //"-=" because B direction is inverse
        }
      of<<x<<"\t"<<y<<"\t"<<z<<"\t"<<b.x()<<"\t"<<b.y()<<"\t"<<b.z()<<endl;
    }
}












