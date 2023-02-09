
#ifndef IN_NOV5
#define IN_NOV5
#include"../../src/include.h"
using namespace std;
using namespace ROOT::Math;

void ToleranceGenerator_nov5(const char* filename_root)
{
  int n_trial=10000;

  magnet cay;
  
  //input parameters
  vector<double> theta_vec,phi_vec;
  vector<TVector3> dis_vec;
  vector<XYZVector> dis_vec_XYZ(6);
  vector<double> def_in_vec,def_out_vec,def_thi_vec;

  const char* ofn_log="ongoing.log";
  rm(ofn_log);
  ofstream log;
  log.open(ofn_log,ios_base::app);
  TTree* uniform_tolerance=new TTree("uni","");
  uniform_tolerance->Branch("theta_vec",&theta_vec);
  uniform_tolerance->Branch("phi_vec",&phi_vec);
  uniform_tolerance->Branch("dis_vec_XYZ",&dis_vec_XYZ);
  uniform_tolerance->Branch("def_in_vec",&def_in_vec);
  uniform_tolerance->Branch("def_out_vec",&def_out_vec);
  uniform_tolerance->Branch("def_thi_vec",&def_thi_vec);
  TRandom2* random=new TRandom2();
  //output parameters
  //data is consisted as below
  double max_Bz,min_Bz;
  double max_Br,min_Br;
  double max_Bz_azi,min_Bz_azi;
  double max_Br_azi,min_Br_azi;
  double average_Bz,average_Br;
  double uniformity;
  uniform_tolerance->Branch("min_Bz",&min_Bz);
  uniform_tolerance->Branch("max_Bz",&max_Bz);
  uniform_tolerance->Branch("min_Br",&min_Br);
  uniform_tolerance->Branch("max_Br",&max_Br);
  uniform_tolerance->Branch("min_Bz_azi",&min_Bz_azi);
  uniform_tolerance->Branch("max_Bz_azi",&max_Bz_azi);
  uniform_tolerance->Branch("min_Br_azi",&min_Br_azi);
  uniform_tolerance->Branch("max_Br_azi",&max_Br_azi);
  uniform_tolerance->Branch("average_Br",&average_Br);
  uniform_tolerance->Branch("average_Bz",&average_Bz);
  uniform_tolerance->Branch("uniformity",&uniformity);
  
  clock_t start=clock();
  log<<n_trial<<"trials will be done"<<endl;
  for(int i_trial=0;i_trial<n_trial;i_trial++)
    {
      cout<<i_trial<<"trials now"<<endl;
      log<<"--------------------------"<<endl;
      log<<i_trial<<"trials now"<<endl;
      OutputElapsedTime(start,ofn_log);
      //3
      puts("step 3");
      theta_vec=get_theta_vec(random);
      phi_vec=get_phi_vec(random);
      dis_vec=get_dis_vec(random);

      for(int i=0;i<6;i++)
        {
          //          dis_vec[i].SetZ(0.);          
          dis_vec_XYZ[i]=TVector3_XYZVector(dis_vec[i]);
        }
      def_in_vec=get_deformation_inner_vec(random);
      def_out_vec=get_deformation_outer_vec(random);
      def_thi_vec=get_deformation_thickness_vec(random);
      vector<double> null_vec(5,0.);
      cay.InputTolerance(theta_vec,phi_vec,dis_vec,
                         def_in_vec,
                         def_out_vec,
                         def_thi_vec);

      //4
      puts("step 4");

      uniform_tolerance->Branch("min_Bz",&min_Bz);
      uniform_tolerance->Branch("max_Bz",&max_Bz);
      uniform_tolerance->Branch("min_Br",&min_Br);
      uniform_tolerance->Branch("max_Br",&max_Br);
      uniform_tolerance->Branch("min_Bz_azi",&min_Bz_azi);
      uniform_tolerance->Branch("max_Bz_azi",&max_Bz_azi);
      uniform_tolerance->Branch("min_Br_azi",&min_Br_azi);
      uniform_tolerance->Branch("max_Br_azi",&max_Br_azi);
      uniform_tolerance->Branch("average_Br",&average_Br);
      uniform_tolerance->Branch("average_Bz",&average_Bz);
      uniform_tolerance->Branch("uniformity",&uniformity);


      eval data(cay);
      min_Bz=data.min_Bz();
      max_Bz=data.max_Bz();
      min_Br=data.min_Br();
      max_Br=data.max_Br();
      min_Bz=data.min_Bz_azi();
      max_Bz=data.max_Bz_azi();
      min_Br=data.min_Br_azi();
      max_Br=data.max_Br_azi();
      average_Bz=data.ave_Bz();
      uniformity=data.U_theta();
      log<<uniformity*1.e6<<endl;
      uniform_tolerance->Fill();
      cay.Perfect();
    }

  TFile* f=new TFile(filename_root,"recreate");
  uniform_tolerance->Write();
  f->Close();
  delete f;
  delete random;
}  

void underachiever()
{
  magnet cay;
  cay.InputTolerances_manual();
  eval data(cay);
  cout<<(data.max_Bz_azi()-3.)*1.e6<<"uT"<<endl;
  cay.drawB("fiducialfield_RandomTolerance.root");
  double unif=data.U_theta()*1.e6;//[ppm]
  cout<<"uniformity "<<unif<<endl;
}





#endif
