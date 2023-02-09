

#include"../../include.h"
using namespace std;

void magnet::CostCalculation(mms* IronShims,int n_mode)
{
  Bs B_MS,B_0,B_ER,B_TG,B_MM;
  B_MS.SetOnDonut();
  B_MS.add_coil(this->MainCoilsAndIronYoke);
  B_MS.DirectionBlinding();
  B_0.SetOnDonut(this->_TargetField);
  B_ER.SetOnDonut();
  B_ER.addBs(B_0,B_MS,1.,-1.);
  mms IronShims_ref,delta_IronShims;
  int n_shim=IronShims->n_shim();
  auto ResponseMatrix=construct_response_matrix_mms(B_MS,*IronShims);
  SVD DecomposedMatrix(ResponseMatrix);
  B_MM.SetOnDonut();
  B_TG.SetOnDonut();
  IronShims->clear();
  for(int i_iteration=0;i_iteration<number_of_iteration_shimming;i_iteration++)
    {
      if((i_iteration%10)==0)
        {
          cout<<i_iteration<<" now"<<endl;
        }
      B_MM.Add_MM_RM(ResponseMatrix,*IronShims);
      B_TG.addBs(B_ER,B_MM,1.,-1.);
      setVfromTSVDdata(delta_IronShims,DecomposedMatrix,B_TG,
                       1.e-15,n_mode);
      IronShims->add_mms(*IronShims,delta_IronShims);
      IronShims->check_constraints(volume_minimum_iron_shim,volume_maximum_iron_shim,
                                   unit_iron_shim_plate);
    }
  cout<<"total volume: "<<IronShims->TotalVolume()*1.e6<<"[cc]"<<endl;
  TotalVolume_iron=IronShims->TotalVolume();
  eval data_tmp(magnet::CombinedCB(),*IronShims);
  data_tmp.out();
  _U_theta=data_tmp.U_theta();
  _U_lc=data_tmp.U_lc();
  this->_cost=magnet::CostFunction();
}

void magnet::shimming_iron(mms* IronShims,int n_mode)
{
  string ofn_dat="TargetFieldDetermination_"+to_string(n_mode)+"mode.dat";
  ofstream OutputFile(ofn_dat.c_str());
  //  OutputFile<<"#target field - 3 [uT]\t U_theta[ppm]\t U_lc[ppm]\t cost[ppm]";
  //  OutputFile<<"\t total volume[cc]"<<endl;
  int flag_optimal=0;
  for(int i_scan_target=0;i_scan_target<n_scan_target;i_scan_target++)
    {
      dl(2);
      double TargetField=Target_Magnetic_Field-div_scan_target*i_scan_target;//[T]
      magnet::SetTargetField(TargetField);
      cout<<"magnet::TargetField: 3+("<<TargetField-3<<") [T]"<<endl;
      magnet::CostCalculation(IronShims,n_mode);
      OutputFile<<(_TargetField-3.)*1.e6<<"\t"<<_U_theta*1.e6<<"\t"<<_U_lc*1.e6<<"\t";
      OutputFile<<_cost*1.e6;
      OutputFile<<"\t"<<TotalVolume_iron*1.e6<<endl;
      bool increase_cost=(_cost>_cost_previous);
      if(increase_cost)
        {
          best_i_scan_target=i_scan_target-1;
          cout<<"Optimal target field was found"<<endl;
          flag_optimal++;
          //          break;
          puts("not break at this time");
        }
      else
        {
          _cost_previous=_cost;
        }
    }
  if(!flag_optimal)
    {
      cout<<"\033[31m Minimum of cost function was not found \033[m"<<endl;
    }
}

double magnet::CostFunction()
{
  double value=Target_Magnetic_Field/_TargetField;
  value+=kappa_CostFunction/upsilon_CostFunction*_U_theta;
  return value;
}

void magnet::OptimizeIronShim(mms* IronShims,int n_mode)
{
  Bs B_MS,B_0,B_ER,B_TG,B_MM;
  B_MS.SetOnDonut();
  B_MS.add_coil(this->MainCoilsAndIronYoke);
  B_MS.DirectionBlinding();
  B_0.SetOnDonut(Target_Magnetic_Field);
  B_ER.SetOnDonut();
  B_ER.addBs(B_0,B_MS,1.,-1.);
  const char* filename_shimdata="../config/shim_20210923.dat";
  mms IronShims_ref,delta_IronShims;
  IronShims->read(filename_shimdata);
  IronShims_ref.read(filename_shimdata);
  int n_shim=IronShims->n_shim();
  delta_IronShims.read(filename_shimdata);
  auto ResponseMatrix=construct_response_matrix_mms(B_MS,*IronShims);
  SVD DecomposedMatrix(ResponseMatrix);
  B_MM.SetOnDonut();
  B_TG.SetOnDonut();
  IronShims->clear();
  for(int i_iteration=0;i_iteration<number_of_iteration_shimming;i_iteration++)
    {
      B_MM.Add_MM_RM(ResponseMatrix,*IronShims);
      B_TG.addBs(B_ER,B_MM,1.,-1.);
      setVfromTSVDdata(delta_IronShims,DecomposedMatrix,B_TG,
                       1.e-15,n_mode);
      IronShims->add_mms(*IronShims,delta_IronShims);
      IronShims->check_constraints(volume_minimum_iron_shim,volume_maximum_iron_shim,
                                   unit_iron_shim_plate);
    }
}

void magnet::SetTargetField(double target)
{
  this->_TargetField=target;
}

void magnet::ActiveShimming(mms mms,ShimCoil* sc)
{
  Bs B_tg("donut");
  B_tg.Add_MM(mms);
  B_tg.add_coil(this->MainCoilsAndIronYoke);
  B_tg.reverse();
  sc->CalculateCurrents_SVD(B_tg);
}




