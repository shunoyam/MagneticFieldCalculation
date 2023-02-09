
#pragma once

#include"../../include.h"
using namespace std;
using namespace Eigen;

class SVD;
class Bs;

class info_SVD
{
private:
  VectorXd _source_vec;
  vector<double> _ModeStrength_vec;
  vector<double> _SingularValue_vec;
  int _n_mode;
public:
  info_SVD(SVD result_SVD,Bs B_TG,const char* ofn_intermediate,
           int n_mode=TruncatedEigenmode,string element="B");
  VectorXd info_svd_eigen();
  void out(string ofn_dat);








  
};
















  


