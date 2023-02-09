
#ifndef _IN_MODELING
#define _IN_MODELING

#include"../../../src/include.h"
using namespace std;

void modeling_def()
{
  const char* filename_root="def_ta_5000_1120.root";
  int n_trial=5;
  magnet MainCoilsAndYoke;
  vector<TVector3> dis_vec;
  vector<XYZVector> dis_vec_XYZ(6);
  vector<double> def_in_vec,def_out_vec;
  vector<double> null(5,0.);
  const char* ofn_log="ongoing.log";
  rm(ofn_log);
  ofstream log;
  log.open(ofn_log,ios_base::app);
  TTree* uniform_tolerance=new TTree("uni","");
  uniform_tolerance->Branch("dis_vec_XYZ",&dis_vec_XYZ);
  uniform_tolerance->Branch("def_in_vec",&def_in_vec);
  uniform_tolerance->Branch("def_out_vec",&def_out_vec);
  TRandom2* random=new TRandom2();
  vector<double> data;
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
      dis_vec=get_dis_vec(random);
      for(int i=0;i<6;i++)
        {
          dis_vec_XYZ[i]=TVector3_XYZVector(dis_vec[i]);
        }
      vector<double> theta_vec(6,0.);
      vector<double> phi_vec(6,0.);
      vector<double> def_thi_vec(6,0.);
      def_in_vec=null;
      def_out_vec=null;
      MainCoilsAndYoke.InputTolerance(theta_vec,phi_vec,dis_vec,
                                      def_in_vec,
                                      def_out_vec,
                                      def_thi_vec);
      eval data(MainCoilsAndYoke);
      average_Bz=data.ave_Bz();
      average_Br=data.ave_Br();
      min_Bz_azi=data.min_Bz_azi();
      max_Bz_azi=data.max_Bz_azi();
      min_Br_azi=data.min_Br_azi();
      max_Br_azi=data.max_Br_azi();
      min_Bz=data.min_Bz();
      max_Bz=data.max_Bz();
      min_Br=data.min_Br();
      max_Br=data.max_Br();
      uniformity=data.U_theta()*1.e6;//[ppm]

      uniform_tolerance->Fill();
      MainCoilsAndYoke.Perfect();
    }

  TFile* f=new TFile(filename_root,"recreate");
  uniform_tolerance->Write();
  f->Close();
  delete f;
  delete random;  
}


void modeling_dis()
{
  const char* filename_root="dis_TA_5000.root";
  int n_trial=10000;
  magnet MainCoilsAndIronYoke;
  vector<TVector3> dis_vec;
  vector<XYZVector> dis_vec_XYZ(6);
  vector<double> def_in_vec,def_out_vec;
  TVector3 nullvector(0.,0.,0.);
  vector<TVector3> null(6,nullvector);
  const char* ofn_log="ongoing.log";
  rm(ofn_log);
  ofstream log;
  log.open(ofn_log,ios_base::app);
  TTree* uniform_tolerance=new TTree("uni","");
  uniform_tolerance->Branch("dis_vec_XYZ",&dis_vec_XYZ);
  uniform_tolerance->Branch("def_in_vec",&def_in_vec);
  uniform_tolerance->Branch("def_out_vec",&def_out_vec);
  TRandom2* random=new TRandom2();
  vector<double> data;
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
      //        dis_vec=null;
      for(int i=0;i<6;i++)
        {
          dis_vec_XYZ[i]=TVector3_XYZVector(dis_vec[i]);
        }
      vector<double> theta_vec(6,0.);
      vector<double> phi_vec(6,0.);
      vector<double> def_thi_vec(6,0.);
      def_in_vec=get_deformation_inner_vec(random);
      def_out_vec=get_deformation_outer_vec(random);
      MainCoilsAndIronYoke.InputTolerance(theta_vec,phi_vec,dis_vec,
                                          def_in_vec,
                                          def_out_vec,
                                          def_thi_vec);
      eval data(MainCoilsAndIronYoke);
      average_Bz=data.ave_Bz();
      average_Br=data.ave_Br();
      min_Bz_azi=data.min_Bz_azi();
      max_Bz_azi=data.max_Bz_azi();
      min_Br_azi=data.min_Br_azi();
      max_Br_azi=data.max_Br_azi();
      min_Bz=data.min_Bz();
      max_Bz=data.max_Bz();
      min_Br=data.min_Br();
      max_Br=data.max_Br();
      uniformity=data.U_theta()*1.e6;//[ppm]
      uniform_tolerance->Fill();
      MainCoilsAndIronYoke.Perfect();
    }

  TFile* f=new TFile(filename_root,"recreate");
  uniform_tolerance->Write();
  f->Close();
  delete f;
  delete random;  
}





#endif
