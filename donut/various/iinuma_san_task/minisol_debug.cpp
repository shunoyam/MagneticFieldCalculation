

#include"../../src/include.h"
using namespace std;

void geometry_minisol(const char* ofn_dat)
{
  Bs B_target;
  const char* path_to_MFEP_configlation_file="../config/MFEP_miniSol.dat";
  B_target.read(path_to_MFEP_configlation_file);

  cb Coil;
  set_coil_miniSol(Coil);
  cb IronYoke;

  const char* path_to_iron_yoke_config_file="../config/iron_shape_1mm.dat";
  IronYoke.read_rz(path_to_iron_yoke_config_file);

  string ofn_dat_str=ofn_dat;
  string ofn_dat_MFEP="MFEP_"+ofn_dat_str;
  string ofn_dat_coil="coil_"+ofn_dat_str;
  string ofn_dat_yoke="yoke_"+ofn_dat_str;
  
  ofstream ofn_MFEP(ofn_dat_MFEP);
  ofstream ofn_coil(ofn_dat_coil);
  ofstream ofn_yoke(ofn_dat_yoke);


  for(int i_MFEP=0;i_MFEP<B_target.MFEPs();i_MFEP++)
    {
      double r=B_target.at(i_MFEP).MFEP().x();
      double z=B_target.at(i_MFEP).MFEP().z();
      ofn_MFEP<<r*1.e3<<"\t"<<z*1.e3<<endl;
    }

  for(int i_coil=0;i_coil<Coil.coil_vec().size();i_coil++)
    {
      double r=Coil.at(i_coil).r();
      double z=Coil.at(i_coil).z();
      ofn_coil<<r*1.e3<<"\t"<<z*1.e3<<endl;
    }
  
  for(int i_flc=0;i_flc<IronYoke.coil_vec().size();i_flc++)
    {
      double r=IronYoke.at(i_flc).r();
      double z=IronYoke.at(i_flc).z();
      ofn_yoke<<r*1.e3<<"\t"<<z*1.e3<<endl;
    }





  
}





