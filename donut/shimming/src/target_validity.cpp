
#include"../../src/include.h"

using namespace std;


void ValidityCheck(mms shims,Bs B)
{
  double min_Bz=0.;
  TVector3 mfep_min;
  for(int i=0;i<B.MFEPs();i++)
    {
     Bfield b=B.at(i);
      double Bz=b.Bz();
      if(min_Bz>Bz)
        {
          min_Bz=Bz;
          mfep_min=b.MFEP();
        }
    }
  int size_shim=((vector<mm>)shims).size();
  auto ConstantVector=Eigen::VectorXd::Constant(size_shim,volume_maximum_iron_shim);
  shims.set(ConstantVector);
  double battle_FullPowerIronShim=-shims.B(mfep_min).z();
  if(battle_FullPowerIronShim<-min_Bz)
    {
      cout<<"Bz by full power iron shim: "<<battle_FullPowerIronShim*1.e6<<endl;
      cout<<"minimum of error field: "<<-min_Bz*1.e6<<endl;
      cout<<"iron power is not potentially enough."<<endl;
      cout<<"Think about target field or max volume of iron shim."<<endl;
    }
}

void compare_target_and_measured_field(mms shims,Bs B)
{
  ValidityCheck(shims,B);
  const char* ofn_dat="valid.dat";
  ofstream of(ofn_dat);
  double v_max=volume_maximum_iron_shim;
  Bfield B_center;
  int size_shim=shims.n_shim();
  auto ConstantVector=Eigen::VectorXd::Constant(size_shim,v_max);
  shims.set(ConstantVector);
  TVector3 B_full;
  double Bz_full;
  int n_mfep=B.MFEPs();
  double Bz_error;
  TVector3 mfep;
  int PositiveTarget=0;
  int DecreasingLimit=0;
  of<<"#number of MFEP,target field[uT],limit field which iron shims can generate[uT]"<<endl;
  for(int i_mfep=0;i_mfep<n_mfep;i_mfep++)
    {
      mfep=B.at(i_mfep).MFEP();
      B_full=shims.B(mfep);
      Bz_full=B_full.z();
      Bz_error=B.at(i_mfep).Bz();
      if(Bz_error>0.)
        {
          PositiveTarget=1;
        }
      if(Bz_error<Bz_full)
        {
          DecreasingLimit=1;
        }
      of<<i_mfep<<"\t"<<-Bz_error*1.e6<<"\t"<<-Bz_full*1.e6<<endl;//[uT]
    }
  if(PositiveTarget)
    {
      cout<<"Passive shimming cannot increase magnetic field in fiducial region"<<endl;
    }
  if(DecreasingLimit)
    {
      cout<<"The geometry of iron shim may be not enough to reduce large error field."<<endl;
    }
}














