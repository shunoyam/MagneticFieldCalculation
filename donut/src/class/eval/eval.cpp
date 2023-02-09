

#include"../../include.h"
using namespace std;

void eval::evaluation(cb coils,int presence_coil,int presence_IronShim,mms* shim,
                      int presence_ShimCoil,ShimCoil* ShimCoil)
{
  int n_mesh=this->_n_mesh;
  // int r_mesh=n_mesh;
  // int z_mesh=n_mesh*3;
  // int theta_mesh=n_mesh*5;
  int r_mesh=number_of_MFEPs_radius;
  int z_mesh=number_of_MFEPs_z;
  int theta_mesh=number_of_MFEPs_theta;
  if(theta_mesh<11)
    {
      cout<<"theta mesh :"<<theta_mesh<<" is too short!"<<endl;
    }
  double Bz_ave_tmp,Br_ave_tmp;
  vector<double> Bz_finalist,Br_finalist;
  double Br_min,Br_max,Bz_min,Bz_max;
  vector<double> Bx_vec,By_vec;
  vector<double> Bz_azi,Br_azi;
  double r=r_center_fiducial-r_width_fiducial;
  for(int out=0;out<2;out++)
    {
      for(int i_z_mesh=0;i_z_mesh<z_mesh;i_z_mesh++)
        {
          double z=z_center_fiducial-z_width_fiducial;
          z+=2.*z_width_fiducial*i_z_mesh/(z_mesh-1.);
          double Bz_ave_tmp=0.;
          double Br_ave_tmp=0.;
          for(int i_theta_mesh=0;i_theta_mesh<theta_mesh;i_theta_mesh++)
            {
              double theta=TMath::TwoPi()*i_theta_mesh/theta_mesh;
              double x=r*cos(theta);
              double y=r*sin(theta);
              TVector3 mfep(x,y,z);
              TVector3 B;
              if(presence_coil)
                {
                 B+=coils.B(mfep);
                }
              if(presence_IronShim)
                {
                  TVector3 B_shim=shim->B(mfep);
                  B+=B_shim;
                }
              if(presence_ShimCoil)
                {
                  TVector3 B_shim=ShimCoil->B(mfep);
                  B+=B_shim;
                }
              this->_Bs_MFEP.push_back(B);
              double Bz=B.z();
              Bz_finalist.push_back(Bz);
              Bz_ave_tmp+=Bz;
              double Bx=B.x();
              Bx_vec.push_back(Bx);
              double By=B.y();
              By_vec.push_back(By);
              double Br=Bx*cos(theta)+By*sin(theta);
              Br_finalist.push_back(Br);
              Br_ave_tmp+=Br;
            }
          Bz_ave_tmp/=theta_mesh;
          Br_ave_tmp/=theta_mesh;
          Bz_azi.push_back(Bz_ave_tmp);
          Br_azi.push_back(Br_ave_tmp);
        }
      r=r_center_fiducial+r_width_fiducial;
    }

  double z=z_center_fiducial-z_width_fiducial;
  for(int top=0;top<2;top++)
    {
      for(int i_r_mesh=0;i_r_mesh<r_mesh;i_r_mesh++)
        {
          r=r_center_fiducial-r_width_fiducial+2.*r_width_fiducial*i_r_mesh/(r_mesh-1.);
          Bz_ave_tmp=0.;
          Br_ave_tmp=0.;
          for(int i_theta_mesh=0;i_theta_mesh<theta_mesh;i_theta_mesh++)
            {
              double theta=TMath::TwoPi()*i_theta_mesh/theta_mesh;
              double x=r*cos(theta);
              double y=r*sin(theta);
              TVector3 mfep(x,y,z);
              TVector3 B=coils.B(mfep);
              if(presence_IronShim)
                {
                  TVector3 B_shim=shim->B(mfep);
                  B+=B_shim;
                }
              if(presence_ShimCoil)
                {
                  TVector3 B_shim=ShimCoil->B(mfep);
                  B+=B_shim;
                }
              this->_Bs_MFEP.push_back(B);
              double Bz=B.z();
              Bz_finalist.push_back(Bz);
              Bz_ave_tmp+=Bz;
              double Bx=B.x();
              Bx_vec.push_back(Bx);
              double By=B.y();
              By_vec.push_back(By);
              double Br=Bx*cos(theta)+By*sin(theta);
              Br_finalist.push_back(Br);
              Br_ave_tmp+=Br;
            }
          Bz_ave_tmp/=theta_mesh;
          Br_ave_tmp/=theta_mesh;
          Bz_azi.push_back(Bz_ave_tmp);
          Br_azi.push_back(Br_ave_tmp);
        }
      z=z_center_fiducial+z_width_fiducial;
    }

  double min_Bz_azi=*min_element(Bz_azi.begin(),Bz_azi.end());
  double max_Bz_azi=*max_element(Bz_azi.begin(),Bz_azi.end());
  double min_Bz=*min_element(Bz_finalist.begin(),Bz_finalist.end());
  double max_Bz=*max_element(Bz_finalist.begin(),Bz_finalist.end());
  double average_Bz=Average(Bz_azi);
     
  double min_Br_azi=*min_element(Br_azi.begin(),Br_azi.end());
  double max_Br_azi=*max_element(Br_azi.begin(),Br_azi.end());
  double min_Br=*min_element(Br_finalist.begin(),Br_finalist.end());
  int arg_min_Br=distance(Br_finalist.begin(),
                          min_element(Br_finalist.begin(),Br_finalist.end()));
  double max_Br=*max_element(Br_finalist.begin(),Br_finalist.end());
  int arg_max_Br=distance(Br_finalist.begin(),
                          max_element(Br_finalist.begin(),Br_finalist.end()));
  double average_Br=Average(Br_azi);

  double min_Bx=*min_element(Bx_vec.begin(),Bx_vec.end());
  int arg_min_Bx=distance(Bx_vec.begin(),min_element(Bx_vec.begin(),Bx_vec.end()));
  double max_Bx=*max_element(Bx_vec.begin(),Bx_vec.end());
  int arg_max_Bx=distance(Bx_vec.begin(),max_element(Bx_vec.begin(),Bx_vec.end()));
  double average_Bx=Average(Bx_vec);
  double min_By=*min_element(By_vec.begin(),By_vec.end());
  double max_By=*max_element(By_vec.begin(),By_vec.end());
  double average_By=Average(By_vec);

  //refering MFEP
  r=r_center_fiducial-r_width_fiducial;
  int i_mfep=0;
  for(int out=0;out<2;out++)
    {
      for(int i_z_mesh=0;i_z_mesh<z_mesh;i_z_mesh++)
        {
          double z=z_center_fiducial-z_width_fiducial;
          z+=2.*z_width_fiducial*i_z_mesh/(z_mesh-1.);
          for(int i_theta_mesh=0;i_theta_mesh<theta_mesh;i_theta_mesh++)
            {
              double theta=TMath::TwoPi()*i_theta_mesh/theta_mesh;
              double x=r*cos(theta);
              double y=r*sin(theta);
              TVector3 mfep(x,y,z);
              if(this->OutputInformation)
                {
                  if(i_mfep==arg_min_Br)
                    {
                      puts("MFEP at min of Br");
                      cout<<r<<"\t"<<theta<<"\t"<<z<<endl;
                      puts("-----------------");
                    }
                  if(i_mfep==arg_max_Br)
                    {
                      puts("MFEP at max of Br");
                      cout<<r<<"\t"<<theta<<"\t"<<z<<endl;
                      puts("-----------------");
                    }
                  if(i_mfep==arg_min_Bx)
                    {
                      puts("MFEP at min of Bx");
                      cout<<r<<"\t"<<theta<<"\t"<<z<<endl;
                      puts("-----------------");
                    }
                  if(i_mfep==arg_max_Bx)
                    {
                      puts("MFEP at max of Bx");
                      cout<<r<<"\t"<<theta<<"\t"<<z<<endl;
                      puts("-----------------");
                    }
                }
              i_mfep++;
            }
        }
      r=r_center_fiducial+r_width_fiducial;
    }

  z=z_center_fiducial-z_width_fiducial;
  for(int top=0;top<2;top++)
    {
      for(int i_r_mesh=0;i_r_mesh<r_mesh;i_r_mesh++)
        {
          r=r_center_fiducial-r_width_fiducial+2.*r_width_fiducial*i_r_mesh/(r_mesh-1.);
          for(int i_theta_mesh=0;i_theta_mesh<theta_mesh;i_theta_mesh++)
            {
              double theta=TMath::TwoPi()*i_theta_mesh/theta_mesh;
              double x=r*cos(theta);
              double y=r*sin(theta);
              TVector3 mfep(x,y,z);
              if(this->OutputInformation)
                {
                  if(i_mfep==arg_min_Br)
                    {
                      puts("MFEP at min of Br");
                      cout<<r<<"\t"<<theta<<"\t"<<z<<endl;
                      puts("-----------------");
                    }
                  if(i_mfep==arg_max_Br)
                    {
                      puts("MFEP at max of Br");
                      cout<<r<<"\t"<<theta<<"\t"<<z<<endl;
                      puts("-----------------");
                    }
                  if(i_mfep==arg_min_Bx)
                    {
                      puts("MFEP at min of Bx");
                      cout<<r<<"\t"<<theta<<"\t"<<z<<endl;
                      puts("-----------------");
                    }
                  if(i_mfep==arg_max_Bx)
                    {
                      puts("MFEP at max of Bx");
                      cout<<r<<"\t"<<theta<<"\t"<<z<<endl;
                      puts("-----------------");
                    }
                }
              i_mfep++;
            }
          z=z_center_fiducial+z_width_fiducial;
        }
    }
  this->_ave_Bz=average_Bz;
  this->_min_Bz_azi=min_Bz_azi;
  this->_max_Bz_azi=max_Bz_azi;
  this->_min_Bz=min_Bz;
  this->_max_Bz=max_Bz;
  this->_p2p_Bz=_max_Bz-_min_Bz;
      
  this->_ave_Br=average_Br;
  this->_min_Br_azi=min_Br_azi;
  this->_max_Br_azi=max_Br_azi;
  this->_min_Br=min_Br;
  this->_max_Br=max_Br;
  this->_p2p_Br=_max_Br-_min_Br;

  this->_ave_Bx=average_Bx;
  this->_min_Bx=min_Bx;
  this->_max_Bx=max_Bx;
  this->_p2p_Bx=_max_Bx-_min_Bx;

  this->_ave_By=average_By;
  this->_min_By=min_By;
  this->_max_By=max_By;
  this->_p2p_By=_max_By-_min_By;
}

eval::eval(cb coils)
{
  eval::evaluation(coils);  
}

eval::eval(magnet m)
{
  cb coils=eval::magnet_cb(m);
  eval::evaluation(coils);
}

eval::eval(cb coils,mms shim)
{
  mms* _shim=&shim;
  int presence_IronShim=1;
  eval::evaluation(coils,1,presence_IronShim,_shim);  
}

eval::eval(magnet m,mms shim)
{
  cb coils=eval::magnet_cb(m);
  mms* _shim=&shim;
  int presence_IronShim=1;
  eval::evaluation(coils,1,presence_IronShim,_shim);
}

eval::eval(ShimCoil sc)
{
  double current=0.;
  coil flc_empty(current,1.,0.);
  vector<coil> FLCs_empty{flc_empty};
  cb coil_empty(FLCs_empty);
  mms* IronShim_empty;
  int presence_ShimCoil=1;
  int absence_coil=0;
  ShimCoil* _sc=&sc;
  eval::evaluation(coil_empty,absence_coil,0,IronShim_empty,presence_ShimCoil,_sc);
}

cb eval::magnet_cb(magnet m)
{
  cb combinedCB;
  for(cb _cb:(vector<cb>)m)
    {
      combinedCB.Push_Back(_cb);
    }
  return combinedCB;
}

double eval::min_Bx()
{
  return this->_min_Bx;
}
double eval::ave_Bx()
{
  return this->_ave_Bx;
}
double eval::max_Bx()
{
  return this->_max_Bx;
}
double eval::p2p_Bx()
{
  return this->_p2p_Bx;
}

double eval::min_By()
{
  return this->_min_By;
}
double eval::ave_By()
{
  return this->_ave_By;
}
double eval::max_By()
{
  return this->_max_By;
}
double eval::p2p_By()
{
  return this->_p2p_By;
}

double eval::min_Br()
{
  return this->_min_Br;
}
double eval::ave_Br()
{
  return this->_ave_Br;
}
double eval::max_Br()
{
  return this->_max_Br;
}
double eval::p2p_Br()
{
  return this->_p2p_Br;
}

double eval::min_Bz()
{
  return this->_min_Bz;
}
double eval::ave_Bz()
{
  return this->_ave_Bz;
}
double eval::max_Bz()
{
  return this->_max_Bz;
}
double eval::p2p_Bz()
{
  return this->_p2p_Bz;
}

double eval::min_Bz_azi()
{
  return this->_min_Bz;
}

double eval::max_Bz_azi()
{
  return this->_max_Bz;
}
double eval::min_Br_azi()
{
  return this->_min_Br;
}

double eval::max_Br_azi()
{
  return this->_max_Br;
}

double eval::p2p_Bz_theta()
{
  double p2p=this->_max_Bz_azi-this->_min_Bz_azi;
  return p2p;
}

double eval::p2p_Br_theta()
{
  double p2p=this->_max_Br_azi-this->_min_Br_azi;
  return p2p;
}

double eval::U_theta()
{
  double AzimuthallyAceragedUniformity=eval::p2p_Bz_theta()/this->_ave_Bz;
  return AzimuthallyAceragedUniformity;
}  

double eval::U_lc()
{
  double AzimuthallyAveragedUniformity=eval::p2p_Bz()/this->_ave_Bz;
  return AzimuthallyAveragedUniformity;
}

void eval::out()
{
  cout<<"local uniformity:"<<eval::U_lc()*1.e6<<"[ppm]"<<endl;
  cout<<"azimuthal averaged uniformity:"<<eval::U_theta()*1.e6<<"[ppm]"<<endl;
  // cout<<"p-p of Br:"<<eval::p2p_Br()*1.e6<<"[uT]"<<endl;
  // cout<<"\t \t min \t ave\t max"<<endl;
  // if(abs(_ave_Bz-3)<.1)
  //   {
  //     cout<<"(Bz[T]-3)*1.e6: "<<(_min_Bz-3.)*1.e6<<"\t"<<(_ave_Bz-3.)*1.e6;
  //     cout<<"\t"<<(_max_Bz-3.)*1.e6<<endl;
  //   }
  // else
  //   {
  //     cout<<"Bz[T]*1.e6: "<<_min_Bz*1.e6<<"\t"<<_ave_Bz*1.e6;
  //     cout<<"\t"<<_max_Bz*1.e6<<endl;
  //   }
  // cout<<"Br[T]*1.e6: "<<_min_Br*1.e6<<"\t"<<_ave_Br*1.e6;
  // cout<<"\t"<<_max_Br*1.e6<<endl;
  // cout<<"Bx[T]*1.e6: "<<_min_Bx*1.e6<<"\t"<<_ave_Bx*1.e6;
  // cout<<"\t"<<_max_Bx*1.e6<<endl;
  // cout<<"By[T]*1.e6: "<<_min_By*1.e6<<"\t"<<_ave_By*1.e6;
  // cout<<"\t"<<_max_By*1.e6<<endl;
}

int eval::RequirementCheck()
{
  bool result_U_lc=(eval::U_lc()<RequiredLocalUniformity);
  bool result_U_theta=(eval::U_theta()<RequiredAzimuthallyAveragedUniformity);
  if(result_U_lc&&result_U_theta)
    {
      return 1;
    }
  return 0;
}

vector<TVector3> eval::Bs_MFEP()
{
  return this->_Bs_MFEP;  
}




