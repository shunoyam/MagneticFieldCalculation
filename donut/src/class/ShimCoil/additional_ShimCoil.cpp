
#include"../../include.h"
using namespace std;

Eigen::MatrixXd ShimCoil::ResponseMatrix()
{
  Bs B("donut");
  int n_mfep=B.MFEPs();
  Eigen::MatrixXd RM(n_mfep,this->_RotationalDivisionNumber);
  for(int i_rotation=0;i_rotation<_RotationalDivisionNumber;i_rotation++)
    {
      vector<double> currents(this->_RotationalDivisionNumber,0.);
      currents[i_rotation]=1.;//ampere
      ShimCoil::SetCurrents(currents);
      for(shim_coil SaddleCoil:this->_shim_coil)
        {
          B.add_shim_coil(SaddleCoil);
        }
      RM.col(i_rotation)=B.Bs_eigen("Br");
      B.clear();
    }
  return RM;
}

void ShimCoil::SimpleModeGeneration(int mode,string NormalOrSkew,double TargetAmplitude)
{
  Bs B_TG("donut");
  int n_mfep=B_TG.MFEPs();
  ofstream of("check.dat");
  for(int i_mfep=0;i_mfep<n_mfep;i_mfep++)
    {
      auto MFEP=B_TG.at(i_mfep).MFEP();
      double AzimuthalAngle=atan2(MFEP.y(),MFEP.x());
      double theta=AzimuthalAngle*mode/2.;
      if(NormalOrSkew=="skew")
        {
          theta+=TMath::Pi()/2.;
        }
      double br=TargetAmplitude*cos(theta);
      double bphi=-sin(theta);
      TVector3 b(br*cos(AzimuthalAngle)-bphi*sin(AzimuthalAngle),
                 br*sin(AzimuthalAngle)+bphi*cos(AzimuthalAngle),0.);
      B_TG.ChangeB(i_mfep,b);
    }
  ShimCoil::CalculateCurrents_SVD(B_TG);
}


void ShimCoil::CalculateCurrents_SVD(Bs B_TG)
{
  Eigen::MatrixXd A=ShimCoil::ResponseMatrix();
  int n_truncation=this->_RotationalDivisionNumber;
  SVD result(A);
  info_SVD solution(result,B_TG,"mode.dat",n_truncation,"Br");  
  auto I_eigen=(VectorXd)solution.info_svd_eigen();
  vector<double> currents;
  for(int i=0;i<this->_RotationalDivisionNumber;i++)
    {
      currents.push_back(I_eigen(i));
    }
  ShimCoil::SetCurrents(currents);
}




