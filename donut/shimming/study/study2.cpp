 

#include"../../src/include.h"
using namespace std;

void ShimmingCalculation()
{
  int mode=65;
  magnet m;
  double t=.1e-3;
  m.InputTolerances_manual(t);
  Draw before(m,"before.root");
  auto shim=new mms();
  double adjust=1.1e-4 *t/(.1e-3);
  double tg=3. - 1.1e-4;
  m.SetTargetField(tg);
  m.CostCalculation(shim,mode);
  Draw probe(m,*shim,"after.root");
  return;
  double U_lc=m.U_lc();
  double U_theta=m.U_theta();
  double rate_low=(double)shim->count("low")/400.;
  double rate_ok=(double)shim->count("ok")/400.;
  double rate_high=(double)shim->count("high")/400.;
  double TotalVolume=shim->TotalVolume();
  cout<<"tolerance U_lc U_theta rate_min rate_ok rate_high v_total mode target"<<endl;
  cout<<t*1.e3<<"\t"<<U_lc*1.e6<<"\t"<<U_theta*1.e6<<"\t"<<rate_low<<"\t"<<rate_ok;
  cout<<"\t"<<rate_high<<"\t"<<TotalVolume*1.e6<<"\t"<<mode<<"\t"<<setprecision(8)<<tg<<endl;
  shim->draw("placement-100um.root");
  delete shim;
}

void ProperStrength()
{
  string ofn_dat="BestThresholdSearch.dat";
  ofstream f(ofn_dat.c_str());
  f<<"#threshold[T] iteration[a.u] uniformity after shimming[ppm]"<<endl;  
  double factor_threshold;
  int n_point=30;
  
  for(int i_point=0;i_point<n_point;i_point++)
    {
      double power=5.*i_point/(n_point-1);
      factor_threshold=pow(10.,power);
    }  
}

void U_lc_WorstCase()
{
  magnet m;
  m.InputTolerances_manual();
  int n_tr=100;
  ofstream OutputFile("ReachingUniformity2.dat");
  for(int i_tr=0;i_tr<n_tr;i_tr++)
    {
      int n_mode=TruncatedEigenmode+i_tr;
      auto shim=new mms();
      m.shimming_iron(shim,n_mode);
      eval measurement_after_shimming(m,*shim);
      double U_lc=measurement_after_shimming.U_lc();
      double U_theta=measurement_after_shimming.U_theta();
      double UseRate=shim->UseRate();
      OutputFile<<n_mode<<"\t"<<U_lc*1.e6<<"\t"<<U_theta*1.e6<<"\t"<<UseRate<<endl;
      double v_ave=shim->AveragedVolume();
      delete shim;
      break;
    }
}

void TruncatedEigenmodeDicision()
{
  const char* ofn_dat="AchivingUniformity.dat";
  magnet m;
  m.InputTolerances_manual();
  int n_factor_truncated=200;
  ofstream OutputFile(ofn_dat);
  OutputFile<<"#truncated eigen mode[a.u]\t local uniformity[ppm]\t total volume[cc]"<<endl;
  for(int i_factor_truncated=50;i_factor_truncated<n_factor_truncated;i_factor_truncated++)
    {
      dl(2);
      cout<<"now "<<i_factor_truncated<<"/"<<n_factor_truncated<<endl;
      int i_TruncatedEigenmode=i_factor_truncated;
      auto shim=new mms();
      m.shimming_iron(shim,i_TruncatedEigenmode);
      double TotalVolume=shim->TotalVolume();
      cout<<"total volume: "<<TotalVolume*1.e6<<"[cc]"<<endl;
      double UseRate=shim->UseRate();
      cout<<"use rate: "<<UseRate*1.e2<<"%"<<endl;
      eval measurement_after_shimming(m,*shim);
      double U_lc=measurement_after_shimming.U_lc();
      OutputFile<<i_TruncatedEigenmode<<"\t"<<U_lc*1.e6<<"\t"<<TotalVolume*1.e6<<endl;
      delete shim;
    }
}

void v_max_decision()
{
  const char* ofn_dat="MaxVolumeDicision_15cc.dat";
  ofstream OutputFile(ofn_dat);
  OutputFile<<"#error field\t Bz by full power iron shim "<<endl;
  magnet m;
  m.InputTolerances_manual();
  Bs B_MS,B_0,B_ER;
  B_MS.SetOnDonut();
  B_MS.add_coil((vector<cb>)m);

  B_0.SetOnDonut(Target_Magnetic_Field);
  B_ER.SetOnDonut();
  B_ER.addBs(B_0,B_MS,1.,-1.);
  const char* filename_shimdata="../config/shim_20210923.dat";
  CreateShimsGeometry(filename_shimdata);
  mms IronShims;
  IronShims.read(filename_shimdata);
  auto FullPower=Eigen::VectorXd::Constant(IronShims.n_shim(),volume_maximum_iron_shim);
  IronShims.set(FullPower);
  ofstream t2("t2.dat");
  for(Bfield b:(vector<Bfield>)B_ER)
    {
      auto mfep=b.MFEP();
      double bz_er=b.Bz();
      double bz_shim=IronShims.B(mfep).z();
      t2<<bz_er*1.e6<<endl;
      OutputFile<<bz_er<<"\t"<<bz_shim<<endl;
    }
}


void study_20220913()
{
  vector<int> modes{58,65,76,87,87,85,70,70};
  vector<double> TargetFields{1.e-4,1.2e-4,1.5e-4,2.e-4,2.5e-4,2.5e-4,3.3e-4,3.3e-4};
  int n_point=modes.size();
  for(int i_point=0;i_point<n_point;i_point++)
    {
      puts("-------------------");
      magnet m;
      double tolerance=(i_point+1.)*.05e-3;
      cout<<tolerance<<"mm"<<endl;
      m.InputTolerances_manual(tolerance);
      m.SetTargetField(3.-TargetFields[i_point]);
      auto shim=new mms();
      m.CostCalculation(shim,modes[i_point]);
      delete shim;
    }
}

void study_20220914()
{
  int n_target=30;
  double tolerance=.1e-3;//[m]
  string ofn_dat="TargetScan100um.dat";
  ofstream of(ofn_dat.c_str());
  magnet m;
  m.InputTolerances_manual(tolerance);
  double volume_max=20.*20.* 21.e-6;//[m3]
  double Decrease=600.e-6;
  for(int i_target=0;i_target<n_target;i_target++)
    {
      double TargetField=3.-Decrease*(i_target/(n_target-1.));
      m.SetTargetField(TargetField);
      auto shim=new mms();
      m.CostCalculation(shim,65);
      eval probe(m,*shim);
      double U_lc=probe.U_lc();
      double volume=shim->TotalVolume();
      double UseRate=volume/volume_max;
      of<<TargetField<<"\t"<<U_lc<<"\t"<<UseRate<<endl;
      delete shim;
    }
}

void study_20220916()
{
  magnet m;
  auto shim=new mms();
  m.InputTolerances_manual(.1e-3);
  int mode=65;
  double target=3.-120.e-6;
  m.SetTargetField(target);
  m.CostCalculation(shim,mode);
  Draw probe(m,*shim,"after_20220916.root");  
}

void study_20220930()
{
  ofstream of("capacity_20220930.dat");
  double tolerance_min=.1e-3;
  double tolerance_max=1.5e-3;
  double tolerance_div=.1e-3;
  int n_point=(tolerance_max-tolerance_min)/tolerance_div;
  vector<int> modes{65,75,87,88,88,88,90,99,99,99,99,99,99,99};
  for(int i_point=0;i_point<n_point;i_point++)
  {
    double tolerance=tolerance_min+tolerance_div*i_point;
    magnet m;
    m.InputTolerances_manual(tolerance);
    double target=3.-(i_point+1.)*1.1e-4;
    m.SetTargetField(target);
    auto shim=new mms();
    m.CostCalculation(shim,modes[i_point]);
    eval probe(m,*shim);
    double U_lc=probe.U_lc();
    double U_theta=probe.U_theta();
    double rate_low=shim->count("low")/400.;
    double rate_ok=shim->count("ok")/400.;
    double rate_high=shim->count("high")/400.;
    double TotalVolume=shim->TotalVolume();
    of<<tolerance*1.e3<<"\t"<<U_lc*1.e6<<"\t"<<U_theta*1.e6;
    of<<"\t"<<rate_low<<"\t"<<rate_ok<<"\t"<<rate_high<<"\t"<<TotalVolume*1.e6<<"\t";
    of<<modes[i_point]<<"\t"<<target<<endl;
    delete shim;
  }
  
}





