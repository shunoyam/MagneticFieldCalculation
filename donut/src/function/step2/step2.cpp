
#include"../../include.h"
using namespace std;
using namespace Eigen;

vector<double> solution_inverse_TSVD(vector<vector<double>> ResponseMatrix,
                                     Bs B_TG,int n_mode)
{
  auto B_TG_vec=(vector<Bfield>)B_TG;
  int n_MFEP_check=B_TG_vec.size();
  int n_MFEP=ResponseMatrix.size();
  int n_source=ResponseMatrix[0].size();
  if(n_MFEP!=n_MFEP_check)
    {
      cout<<"Error!!"<<endl;
      puts("Element counts are not consistent with matrix A and vector B.");
    }

  MatrixXd ResponseMatrix_eigen(n_MFEP,n_source);
  for(int i_MFEP=0;i_MFEP<n_MFEP;i_MFEP++)
    {
      for(int i_source=0;i_source<n_source;i_source++)
        {
          ResponseMatrix_eigen(i_MFEP,i_source)=ResponseMatrix[i_MFEP][i_source];
        }
    }
  SVD result(ResponseMatrix_eigen);
  MatrixXd I_mode(n_source,n_mode);
  //
  info_SVD solution(result,B_TG,"mode.dat",n_mode);
  auto I_eigen=(VectorXd)solution.info_svd_eigen();
  //
  vector<double> I_vec;
  for(int i_source=0;i_source<n_source;i_source++)
    {
     double I=I_eigen(i_source);
      I_vec.push_back(I);
    }
  return I_vec;
}

int evalerror(cb FilamentLoopCoil,Bs B_target,int output,
              const char* outputfilename_Bdistribution)
{
  double Bz_mean_TG=B_target.average();
  B_target.subtract(FilamentLoopCoil);
  if(output)
    {
      Bs B_draw;
      B_draw.setonXZ(0.6,0.59,0.,0.8);
      B_draw.add_coil(FilamentLoopCoil);
    }
  double Bz_error_amp=B_target.max('z')-B_target.max('z');
  cout<<"Amplitude of error field(peak to peak) is ";
  cout<<Bz_error_amp*1.e4<<"(Gauss)"<<endl;
  double p2p=Bz_error_amp*1.e6/Bz_mean_TG;//ppm
  cout<<"mean :"<<Bz_mean_TG*1.e4<<"(Gauss)"<<endl;
  
  double p2pppt=p2p*1.e-3;//ppt
  if (p2p<1.){
    cout<<"Amplitude of error field from target field(peak to peak) is ";
    cout<<p2p<<"ppm."<<endl;
    cout<<"Under 1ppm error. Congrats!"<<endl;
    cout<<"------------------------------------------------"<<endl;
    return 1;
  }
  else if(1.e3<p2p){
    cout<<"Amplitude of error field from target field(peak to peak) is ";
    cout<<p2pppt<<"ppt."<<endl;
    cout<<"Over 1ppm error. Think twice."<<endl;
    cout<<"------------------------------------------------"<<endl;
    return 0;
  }

  else {
    cout<<"Amplitude of error field from target field(peak to peak) is "<<p2p<<"ppm."<<endl;
    cout<<"Over 1ppm error. Almost!"<<endl;
    cout<<"------------------------------------------------"<<endl;
    return 0;
  }
}

void DesignFLCandDrawB(cb& FilamentLoopCoil,int n_coil,double length,
                       Bs B_target,
                       const char* outputfilename_Idistribution,
                       const char* outputfilename_Bdistribution)
{
  double z;
  double r_coil=0.525;
  coil coil_tmp;
  for(int i_coil=0;i_coil<n_coil;i_coil++)
    {
      z=-length/2.+length*i_coil/(n_coil-1);
      coil_tmp.set(0.,r_coil,z);
      FilamentLoopCoil.push_back(coil_tmp);
    }
  B_target.setonDSS(diameter_of_DSS,number_of_MFEPs_on_DSS_theta,
                    number_of_MFEPs_on_DSS_phi,Target_Magnetic_Field);
  int ev;
  for(int n_mode=3;n_mode<20;n_mode++)
    {
      optimize_coil(FilamentLoopCoil,n_mode,B_target,0);
      ev=evalerror(FilamentLoopCoil,B_target,0);
      if(n_mode==20)
        {
          cout<<"Truncated mode is too large. Something is wrong.."<<endl;
          break;
        }
      if(ev)
        {
          if(outputfilename_Bdistribution!="")
            {
              Bs B_draw;
              B_draw.setonXZ(.6,.59,0.,.8);
              B_draw.add_coil(FilamentLoopCoil);
            }
          if(outputfilename_Idistribution!="")
            {
              FilamentLoopCoil.draw(outputfilename_Idistribution);
            }
          break;
        }
    }
}

void setCB(cb& CB,int n_r,int n_z,double lowerend,double upperend,
           double r_in,double r_out)
{
  int n=n_r*n_z,i_cb=0;
  double r,z;
  coil coil_tmp;

  for(int i_r=0;i_r<n_r;i_r++)
    {
      r=r_in+(r_out-r_in)*i_r/(double)(n_r-1);
      for(int i_z=0;i_z<n_z;i_z++)
        {
          z=lowerend+(upperend-lowerend)*i_z/(double)(n_z-1);
          coil_tmp.setr(r);
          coil_tmp.setz(z);
          CB.push_back(coil_tmp);
          coil_tmp.setr(r);
          coil_tmp.setz(-z);
          CB.push_back(coil_tmp);
        }
    }
}

void setVfromTSVDdata(mms& mms,
                      SVD data_SVD,
                      Bs B_TG,double threshold_strength,int n_mode)
{
  const char* fn_intermediate="mode.dat";
  info_SVD solution(data_SVD,B_TG,fn_intermediate,n_mode);
  solution.out("information_0731.dat");
  mms.set(solution);
}

vector<double> get_theta_vec(TRandom2* random)
{
  vector<double> theta_vec(n_YOKEandMainCoils);
  theta_vec[n_YOKE]=get_random_discrete(-tolerance_tilt_MC10,tolerance_tilt_MC10,random);
  theta_vec[n_MC10]=get_random_discrete(-tolerance_tilt_MC11,tolerance_tilt_MC11,random);
  theta_vec[n_MC11]=get_random_discrete(-tolerance_tilt_MC20,tolerance_tilt_MC20,random);
  theta_vec[n_MC20]=get_random_discrete(-tolerance_tilt_MC21,tolerance_tilt_MC21,random);
  theta_vec[n_MC21]=get_random_discrete(-tolerance_tilt_MC30,tolerance_tilt_MC30,random);
  theta_vec[n_MC30]=get_random_discrete(-tolerance_tilt_yoke,tolerance_tilt_yoke,random);
  return theta_vec;
}

vector<double> get_phi_vec(TRandom2* random)
{
  double phi;
  vector<double> phi_vec;
  for(int i=0;i<n_YOKEandMainCoils;i++)
    {
      phi=get_phi(random);
      phi_vec.push_back(phi);
    }
  return phi_vec;
}

vector<double> get_deformation_inner_vec(TRandom2* random)
{
  int NumberOfCoils=5;
  double deformation;
  vector<double> def_vector;
  for(int i=0;i<NumberOfCoils;i++)
    {
      deformation=get_deformation_inner(random);
      def_vector.push_back(deformation);
    }
  return def_vector;
}

vector<double> get_deformation_outer_vec(TRandom2* random)
{
  int NumberOfCoils=5;
  double deformation;
  vector<double> def_vector;
  for(int i=0;i<NumberOfCoils;i++)
    {
      deformation=get_deformation_outer(random);
      def_vector.push_back(deformation);
    }
  return def_vector;
}

vector<double> get_deformation_thickness_vec(TRandom2* random)
{
  int NumberOfCoils=5;
  double deformation;
  vector<double> def_vector;
  for(int i=0;i<NumberOfCoils;i++)
    {
      deformation=get_deformation_thickness(random);
      def_vector.push_back(deformation);
    }
  return def_vector;
}

vector<TVector3> get_dis_vec(TRandom2* random)
{
  vector<TVector3> dis_vec(6);
  dis_vec[n_YOKE]=get_dis(random,tolerance_radial_yoke,tolerance_axial_yoke);
  dis_vec[n_MC10]=get_dis(random,tolerance_radial_MC10,tolerance_axial_MC10);
  dis_vec[n_MC11]=get_dis(random,tolerance_radial_MC11,tolerance_axial_MC11);
  dis_vec[n_MC20]=get_dis(random,tolerance_radial_MC20,tolerance_axial_MC20);
  dis_vec[n_MC21]=get_dis(random,tolerance_radial_MC21,tolerance_axial_MC21);
  dis_vec[n_MC30]=get_dis(random,tolerance_radial_MC30,tolerance_axial_MC30);

  return dis_vec;
}















