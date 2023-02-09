



#include"../../include.h"

using namespace std;
using namespace ROOT::Math;
using namespace Eigen;


void CreateShimsGeometry(const char* path_to_output_file_name)
{
  int n_z=number_shim_pocket;
  int n_theta=number_shim_tray;
  double l=length_shim_tray;
  double r=radius_shim_tray;
  ofstream outputfile(path_to_output_file_name);
  double x,y,z;
  double theta;
  for(int i_z=0;i_z<n_z;i_z++)
    {
      z=-l/2.+l*i_z/(n_z-1.);
      for(int i_theta=0;i_theta<n_theta;i_theta++)
        {
          theta=TMath::TwoPi()*i_theta/n_theta;
          x=r*cos(theta);
          y=r*sin(theta);
          outputfile<<x<<" "<<y<<" "<<z<<endl;
        }
    }
}

MatrixXd construct_response_matrix_mms(Bs Bs_for_MFEP_extraction,
                                       mms mms)
{
  auto shims=(vector<mm>)mms;
  int n_MFEP=Bs_for_MFEP_extraction.MFEPs();
  int n_shim=shims.size();
  MatrixXd ResponseMatrix(n_MFEP,n_shim);
  double ResponseElement;
  TVector3 mfep;
  TVector3 ResponseElement_TVector3;
  for(int i_MFEP=0;i_MFEP<n_MFEP;i_MFEP++)
    {
      mfep=Bs_for_MFEP_extraction.at(i_MFEP).MFEP();
      for(int i_shim=0;i_shim<n_shim;i_shim++)
        {
          ResponseElement_TVector3=shims[i_shim].ResponseElement(mfep);
          ResponseElement=ResponseElement_TVector3.z();
          ResponseMatrix(i_MFEP,i_shim)=ResponseElement;
        }
    }
  return ResponseMatrix;
}

TVector3 get_uni_random_TVector3(TRandom2* random,
                                 double range_x,
                                 double range_y,
                                 double range_z)
{
  TVector3 pos;
  double x,y,z;
  x=get_random_discrete(-range_x,range_x,random);
  y=get_random_discrete(-range_y,range_y,random);
  z=get_random_discrete(-range_z,range_z,random);
  pos.SetXYZ(x,y,z);
  return pos;
}


double get_phi(TRandom2* random)
{
  double phi;
  phi=get_random_discrete(0.,TMath::TwoPi(),random);
  return phi;
}

TVector3 get_dis(TRandom2* random,double tolerance_r,double tolerance_z)
{
  double dis_x,dis_y,dis_z;
  dis_x=get_random_discrete(-tolerance_r,tolerance_r,random);
  dis_y=get_random_discrete(-tolerance_r,tolerance_r,random);
  dis_z=get_random_discrete(-tolerance_z,tolerance_z,random);
  TVector3 dis(dis_x,dis_y,dis_z);
  return dis;
}

double get_deformation_inner(TRandom2* random)
{
  double deformation_inner;
  deformation_inner=get_random_discrete(-tolerance_expansion_inner_radius,
                                        tolerance_expansion_inner_radius,
                                        random);
  return deformation_inner;
}

double get_deformation_outer(TRandom2* random)
{
  double deformation_outer;
  deformation_outer=get_random_discrete(-tolerance_expansion_outer_radius,
                                        tolerance_expansion_outer_radius,
                                        random);
  return deformation_outer;
}

double get_deformation_thickness(TRandom2* random)
{
  double deformation_thickness;
  deformation_thickness=get_random_discrete(-tolerance_expansion_thickness,
                                            tolerance_expansion_thickness,
                                            random);
  return deformation_thickness;
}

double get_random_discrete(double LowerEnd,double UpperEnd,TRandom2* random,int separation)
{
  // double interval=(UpperEnd-LowerEnd)/separation;
  // int i_separation=random->Integer(separation+1);
  // double RandomValue=LowerEnd+i_separation*interval;
  double RandomValue=random->Uniform(LowerEnd,UpperEnd);
  return RandomValue;
}

void Adjustment_MinMode(double factor_adjustment,Eigen::VectorXd &I_eigen,
                        int i_mode_min_ms,SVD result,double ModeStrength)
{
  double sigma_last=result.sigma(i_mode_min_ms);
  auto v_last=result.v(i_mode_min_ms);
  double NormalizationFactor=sqrt(result.n_MFEP());
  auto I_last=v_last;
  double factor=ModeStrength*NormalizationFactor/sigma_last;
  I_last*=factor;
  I_last*=factor_adjustment;
  I_eigen-=I_last;
}

























  
