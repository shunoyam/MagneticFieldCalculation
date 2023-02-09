

#include"../../include.h"
using namespace std;

info_SVD::info_SVD(SVD result_SVD,Bs B_TG,const char* ofn,
                   int n_mode,string element)
{
  this->_n_mode=n_mode;
  int n_MFEP=result_SVD.n_MFEP();
  int n_source=result_SVD.n_source();
  double NormalizationFactor=sqrt(n_MFEP);

  auto B_TG_eigen=B_TG.Bs_eigen(element);
  VectorXd I_eigen=VectorXd::Zero(n_source);
  VectorXd I_i_mode(n_source);
  VectorXd u(n_MFEP);
  VectorXd v(n_source);
  VectorXd u0(n_MFEP);
  int MinimumMode;
  double min_ms=1.;
  for(int i_mode=0;i_mode<n_mode;i_mode++)
    {
      double sigma=result_SVD.sigma(i_mode);
      this->_SingularValue_vec.push_back(sigma);
      auto v=result_SVD.v(i_mode);
      auto u=result_SVD.u(i_mode);
      double udotB=u.dot(B_TG_eigen);
      double ModeStrength=udotB/NormalizationFactor;
      this->_ModeStrength_vec.push_back(abs(ModeStrength));
      I_i_mode=ModeStrength*NormalizationFactor*v/sigma;
      if(abs(ModeStrength)<min_ms)
        {
          min_ms=abs(ModeStrength);
          MinimumMode=i_mode;
        }
      I_eigen+=I_i_mode;
    }
  this->_source_vec=I_eigen;
}


 Eigen::VectorXd info_SVD::info_svd_eigen()
{
  return this->_source_vec;
}

void info_SVD::out(string ofn_dat)
{
  ofstream of(ofn_dat.c_str());
  string tab="\t";
  for(int i_mode=0;i_mode<this->_n_mode;i_mode++)
    {
      of<<i_mode<<tab<<this->_ModeStrength_vec[i_mode];
      of<<tab<<this->_SingularValue_vec[i_mode]<<endl;
    }
}


