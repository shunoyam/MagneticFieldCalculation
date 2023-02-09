

#include"../src/include.h"
using namespace std;

void ShimmingOperation3()
{

  //2
  magnet g2emagnet;
  g2emagnet.InputTolerance_notsmart();
  
  //3
  Bs B_MS,B_0,B_ER,B_MM,B_TG;
  B_MS.SetOnDonut();
  B_MS.Add_Coil(g2emagnet.cb_vec());

  //4
  g2emagnet.drawB("BeforeShimming.root");
  if(B_MS.NeedRampUp(Target_Magnetic_Field))
    {
      double RampUpRate=B_MS.RampUpRate(Target_Magnetic_Field);
      g2emagnet.RampUp(RampUpRate);
      B_MS.clear();
      B_MS.Add_Coil(g2emagnet.cb_vec());
    }
  B_0.SetOnDonut(Target_Magnetic_Field);

  //5
  B_ER.SetOnDonut();
  B_ER.subtractBs(B_0,B_MS);

  //6
  const char* filename_shimdata="../config/shim_20210923.dat";
  CreateShimsGeometry(filename_shimdata);
  mms ironshims,ironshims_ref,delta_ironshims;
  ironshims.read(filename_shimdata);
  ironshims_ref.read(filename_shimdata);
  g2emagnet.drawB("AfterRampUp.root");
  auto v_tmp=Eigen::VectorXd::Constant(ironshims.mm_vec().size(),0.);
  ironshims_ref.set_volume_eigen(v_tmp);
  auto data_before=g2emagnet.Fiducial("both");
  puts("before shimming");
  OutputImportantValues(data_before);
  int n_shim=ironshims.mm_vec().size();
  delta_ironshims.read(filename_shimdata);
  compare_target_and_measured_field(delta_ironshims,B_ER);
  
  //7
  Eigen::MatrixXd ResponseMatrix;
  ResponseMatrix=construct_response_matrix_mms(B_MS,ironshims);
  //8
  auto Data_SVD=singular_values_and_basis_vectors(ResponseMatrix);
  
  //9
  double TotalPocketSize=volume_maximum_iron_shim*n_shim;
  double B_RE_max,B_RE_min;
  vector<double> ironshims_vec,delta_ironshims_vec,ironshims_before_vec;
  B_MM.SetOnDonut();
  B_TG.SetOnDonut();
  
  int n_iteration_tmp=90;
  //number_of_iteration_shimming
  //10 
  for(int i_iteration=0;i_iteration<n_iteration_tmp;i_iteration++)
    {
      if(i_iteration%10==0)
        {
          dl();
          cout<<i_iteration<<"th iteration"<<endl;
        }
      B_MM.Add_MM_RM(ResponseMatrix,ironshims);
      B_TG.subtractBs(B_ER,B_MM);
      //11
      setVfromTSVDdata(delta_ironshims,Data_SVD,B_TG);
      delta_ironshims_vec=delta_ironshims.mms_vec();
      ironshims.add_mms(ironshims,delta_ironshims);
      ironshims_before_vec=ironshims.mms_vec();
      
      //12
      int count_lower_limit=ironshims.count("low");
      int count_OK=ironshims.count("ok");
      int count_upper_limit=ironshims.count("high");
      double rate_OK=(double)count_OK/n_shim;
      double rate_lower=(double)count_lower_limit/n_shim;
      double rate_upper=(double)count_upper_limit/n_shim;
      //13
      ironshims.check_constraints(volume_minimum_iron_shim,volume_maximum_iron_shim
                                  ,unit_iron_shim_plate);

          cout<<"negative volume:"<<rate_lower*100.<<"%"<<endl;
          cout<<"over volume:"<<rate_upper*100.<<"%"<<endl;
          cout<<"under constraint:"<<rate_OK*100.<<"%"<<endl;
          auto data_tmp=g2emagnet.Fiducial_shimming("both",ironshims);
          OutputImportantValues(data_tmp);


      if(rate_OK>0.2)
        {
          if(RequirementCheck(data_tmp))
            {
              break;
            }
          dl();
        }
      B_MM.clear();
      B_MM.Add_MM(ironshims);
      ironshims_vec=ironshims.mms_vec();
    }
  auto data_after=g2emagnet.Fiducial_shimming("both",ironshims);
  OutputImportantValues(data_after);
  
  //14
   ironshims.draw("placement.root");
   g2emagnet.drawB("residual.root",&ironshims);
   cout<<"average volume of iron shim:"<<ironshims.AveragedVolume()*1.e6<<"[cc]"<<endl;
}

  
    














