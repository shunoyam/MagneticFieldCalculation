
#pragma once

#include"../../src/include.h"
using namespace std;

void ToleranceAnalysis2()
{
  const char* filename_root="2para_toleana_1109.root";
  int n_trial=10000;

  vector<cb> MainCoilsAndYoke;
  read_g2e_magnet(MainCoilsAndYoke);
  
  //input parameters
  vector<double> theta_vec,phi_vec;
  vector<double> deformation_inner_vec(5,0.);
  vector<double> deformation_outer_vec(5,0.);
  vector<double> deformation_thickness_vec(5,0.);
  TVector3 null(0.,0.,0.);
  vector<TVector3> dis_vec(6,null);
  vector<double> def_in_vec,def_out_vec,def_thi_vec;
  TVector3 dis_yoke,dis_MC11,dis_MC20,dis_MC21,dis_MC30;

  const char* ofn_log="ongoing.log";
  rm(ofn_log);
  ofstream log(ofn_log,ios_base::app);
  TTree* uniform_tolerance=new TTree("uni","");
  uniform_tolerance->Branch("theta_vec",&theta_vec);
  uniform_tolerance->Branch("phi_vec",&phi_vec);
  TRandom2* random=new TRandom2();

  //output parameters
  vector<double> data;
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
      theta_vec[n_YOKE]=0.;
      theta_vec[n_MC11]=0.;
      theta_vec[n_MC21]=0.;
      theta_vec[n_MC30]=0.;
      phi_vec[n_YOKE]=0.;
      phi_vec[n_MC20]=0.;
      phi_vec[n_MC21]=0.;
      phi_vec[n_MC30]=0.;
      InputTolerance(MainCoilsAndYoke,
                     theta_vec,phi_vec,dis_vec,
                     deformation_inner_vec,
                     deformation_outer_vec,
                     deformation_thickness_vec);
      MainCoilsAndYoke[n_YOKE].perfect();
      MainCoilsAndYoke[n_MC11].perfect();
      MainCoilsAndYoke[n_MC21].perfect();
      MainCoilsAndYoke[n_MC30].perfect();
      //4
      puts("step 4");
      data=Fiducial(MainCoilsAndYoke,"both");
      
      average_Bz=data[0];
      min_Bz_azi=data[1];
      max_Bz_azi=data[2];
      min_Bz=data[3];
      max_Bz=data[4];
      average_Br=data[5];
      min_Br_azi=data[6];
      max_Br_azi=data[7];
      min_Br=data[8];
      max_Br=data[9];
      uniformity=(max_Bz_azi-min_Bz_azi)*1.e6/average_Bz;//[ppm]
      log<<uniformity<<endl;
      uniform_tolerance->Fill();
      Perfect(MainCoilsAndYoke);
    }

  TFile* f=new TFile(filename_root,"recreate");
  uniform_tolerance->Write();
  f->Close();
  delete f;
  delete random;
}  

static double uniformity_LinearResponse(vector<double> tilt_vec)
{
  double uniformity_perfect=0.1;
  vector<double> coefficient_tilt_uniformity{
                                             0.145708,
                                             0.145708,
                                             0.147446,
                                             0.147446,
                                             0.0726915};
  double uniformity=0.;
  double added;

  for(int i_cb=0;i_cb<5;i_cb++)
    {
      added=tilt_vec[i_cb]*coefficient_tilt_uniformity[i_cb];
      uniformity+=added;
    }
  uniformity=abs(uniformity+uniformity_perfect);
  return uniformity; 
}

void analysis_2para_tolerance(const char* ifn_root)
{
  string ifn_root_str=ifn_root;
  string extension=".root";
  int size_extension=extension.size();
  for(int i_size=0;i_size<size_extension;i_size++)
    {
      ifn_root_str.pop_back();
    }
  string AdditionalCharactor="_analysed";
  string ofn_root_str=ifn_root_str+AdditionalCharactor+extension;
  const char* ofn_root_char=ofn_root_str.c_str();
  TFile *fin = new TFile(ifn_root, "read");
  TTree *tin = (TTree*)fin->Get("uni");

  vector<double> *theta_vec=0;
  vector<double> *phi_vec=0;
  vector<double>* def_in_vec=0;
  vector<double>* def_out_vec=0;
  vector<double>* def_thi_vec=0;
  vector<XYZVector>* dis_vec_XYZ=0;
  TBranch *bra_theta_vec=0;
  TBranch *bra_phi_vec=0;
  TBranch *bra_def_in_vec=0;
  TBranch *bra_def_out_vec=0;
  TBranch *bra_def_thi_vec=0;
  TBranch *bra_dis_vec_XYZ=0;
  double size_font=0.05;
  ofstream f_dat("test.dat");     
  double min_Br,max_Br,min_Bz,max_Bz,average_Bz,average_Br,uniformity;
  double max_Bz_azi,min_Bz_azi;
  tin->SetBranchAddress("theta_vec",&theta_vec,&bra_theta_vec);
  tin->SetBranchAddress("phi_vec",&phi_vec,&bra_phi_vec);  
  tin->SetBranchAddress("def_in_vec",&def_in_vec,&bra_def_in_vec);
  tin->SetBranchAddress("def_out_vec",&def_out_vec,&bra_def_out_vec);
  tin->SetBranchAddress("def_thi_vec",&def_thi_vec,&bra_def_thi_vec);
  tin->SetBranchAddress("min_Br",&min_Br);
  tin->SetBranchAddress("max_Br",&max_Br);
  tin->SetBranchAddress("min_Bz",&min_Bz);
  tin->SetBranchAddress("max_Bz",&max_Bz);
  tin->SetBranchAddress("min_Bz_azi",&min_Bz_azi);
  tin->SetBranchAddress("max_Bz_azi",&max_Bz_azi);
  tin->SetBranchAddress("average_Bz",&average_Bz);
  tin->SetBranchAddress("uniformity",&uniformity);
  tin->SetBranchAddress("dis_vec_XYZ",&dis_vec_XYZ,&bra_dis_vec_XYZ);  

  string title_input="input parameters are really choosen randomly?;";
  string xtitle_input="#theta_{MC10}[mrad];";
  string ytitle_input="#theta_{MC11}[mrad]";
  string titles_input=title_input+xtitle_input+ytitle_input;
  int mesh_x=separation_random_tolerance*4.;
  int mesh_y=separation_random_tolerance*4.;
  double tolerance=0.5e-3;
  int bin_output=100;
  double max_unif_main=300.;
  double max_unif_outliers=4000;
  TH2D *h_input=new TH2D("h_input",titles_input.c_str(),
                         mesh_x,-tolerance*1000.,tolerance*1000.,
                         mesh_y,-tolerance*1000.,tolerance*1000.);

  h_input->GetXaxis()->SetTitleSize(size_font);
  h_input->GetYaxis()->SetTitleSize(size_font);
  h_input->GetXaxis()->SetLabelSize(size_font);
  h_input->GetYaxis()->SetLabelSize(size_font);
  TH1D* h_unif_main=new TH1D("h_unif_main",
                             "uniformity;p-p uniformity(azimuthal average)[ppm];entries[a.u]",
                             bin_output,0.,max_unif_main);
  h_unif_main->GetXaxis()->SetTitleSize(size_font);
  h_unif_main->GetYaxis()->SetTitleSize(size_font);
  h_unif_main->GetXaxis()->SetLabelSize(size_font);
  h_unif_main->GetYaxis()->SetLabelSize(size_font);
  TH1D* h_unif_outliers=new TH1D("h_unif_outliers",
                                 "outliers;p-p uniformity(azimuthal average)[ppm];entries[a.u]",
                                 bin_output,max_unif_main,max_unif_outliers);
  h_unif_outliers->GetXaxis()->SetTitleSize(size_font);
  h_unif_outliers->GetYaxis()->SetTitleSize(size_font);
  h_unif_outliers->GetXaxis()->SetLabelSize(size_font);
  h_unif_outliers->GetYaxis()->SetLabelSize(size_font);
  int interval=100.;
  int bin_enough=interval*10.;
  TH1D* sharp=new TH1D("h_sharp","discretized to 100 values;parameter;entries[a.u]",
                       bin_enough,-0.6e-3,0.6e-3); 
  int N = tin->GetEntries();
  ofstream f_ave("ave.dat");
  cout<<fixed<<setprecision(15)<<endl;
  ///////////pick up now!!!!!//////////////////////////////
  for (int ientry=0;ientry<N;ientry++)
    {
      tin->GetEntry(ientry);
      int tentry=tin->LoadTree(ientry);
      bra_theta_vec->GetEntry(tentry);
      bra_phi_vec->GetEntry(tentry);
      bra_dis_vec_XYZ->GetEntry(tentry);
      h_input->Fill(theta_vec->at(n_MC10)*1000.,theta_vec->at(n_MC11)*1000.);
      h_unif_main->Fill(uniformity);
      f_dat<<uniformity<<endl;
      sharp->Fill(theta_vec->at(2));
      // cout<<"Uniformity : ("<<max_Bz_azi<<"-"<<min_Bz_azi<<")/"<<average_Bz<<"=";
      // cout<<uniformity<<"[ppm]"<<endl;
      f_ave<<average_Bz<<endl;
      if(ientry==300)
        {
          cout<<"Uniformity : ("<<max_Bz_azi<<"-"<<min_Bz_azi<<")/"<<average_Bz<<"=";
          cout<<uniformity<<"[ppm]"<<endl;
          coutInputParameters(dis_vec_XYZ,def_in_vec,def_out_vec,def_thi_vec,
                              theta_vec,phi_vec);
        }
      if(max_unif_main<uniformity && uniformity<max_unif_outliers)
        {
          puts("---------------------");
          cout<<"Outlier appeared at "<<ientry<<"th entry"<<endl;
          cout<<"Uniformity : ("<<max_Bz_azi<<"-"<<min_Bz_azi<<")/"<<average_Bz<<"=";
          cout<<uniformity<<"[ppm]"<<endl;
          h_unif_outliers->Fill(uniformity);
          coutInputParameters(dis_vec_XYZ,def_in_vec,def_out_vec,def_thi_vec,
                              theta_vec,phi_vec);
        }
      if(isnan(uniformity))
        {
          puts("NaN is filled!!");
        }
    }
  double CorrelationFactor=h_input->GetCorrelationFactor();
  cout<<"Correlation factor between 2 parameters :"<<CorrelationFactor<<endl;
 
  TCanvas* c=new TCanvas("c","c");
  c->Divide(2,2);
  c->cd(1);
  const char* option_TH2D="colz";
  h_input->Draw(option_TH2D);
  c->cd(2);
  h_unif_main->Draw();
  c->cd(3);
  h_unif_outliers->Draw();
  c->cd(4);
  sharp->Draw();
  TFile* file_root=new TFile(ofn_root_char,"recreate");
  c->Write();
  file_root->Close();
  delete c;
}

void analytically()
{
  double uniformity_perfect=0.1;
  vector<double> coefficient_tilt_uniformity{
                                             0.145708,
                                             0.145708,
                                             0.147446,
                                             0.147446,
                                             0.0726915};
  double tilt_max=0.5;
  double tilt_square=pow(tilt_max,2);
  double dominator=3./tilt_square;
  double SumOfSensitivitySquare=0;
  for(double sensitivity: coefficient_tilt_uniformity)
    {
      SumOfSensitivitySquare+=pow(sensitivity,2);
    } 

  double sd_uniformity=sqrt(pow(uniformity_perfect,2)+SumOfSensitivitySquare/dominator);
  cout<<"standard deviation of uniformity should be "<<sd_uniformity<<endl;
  cout<<"with accuracy of 1 order"<<endl;

  int n_bin=100;
  int n_trial=1000000;
  double uni;
  TRandom2* random=new TRandom2();
  vector<double> tilt_vec; 
  string title="analysis of uniformity considering tilt of coils";
  string XYlabel=";azimuthally averaged uniformity [ppm];count[a.u]";
  string _2ndArgument=title+XYlabel;
  double tilt;
  TH1D* hist=new TH1D("h",_2ndArgument.c_str(),n_bin,0.,.6);
  //  hist->SetFillColor(kGreen);
  
  for(int i_trial=0;i_trial<n_trial;i_trial++)
    {
      tilt_vec.clear();
      for(int i_cb=0;i_cb<5;i_cb++)
        {
          tilt=random->Uniform(-tilt_max,tilt_max);
          tilt_vec.push_back(tilt);
        }
      uni=uniformity_LinearResponse(tilt_vec);
      hist->Fill(uni);
    }
  string title_sim="distribution of azimuthally averaged uniformity in the fiducial region";
  string XYlabel_sim=";azimuthally averaged uniformity [ppm];count[a.u]";
  string _2ndArgument_sim=title_sim+XYlabel;
  TH1D* h_sim=new TH1D("simple",_2ndArgument_sim.c_str(),n_bin,0.,.6);
  h_sim->SetFillColor(kRed);
  h_sim->Draw();
  double probability;
  double BinWidth=h_sim->GetBinWidth(0);
  double TotalArea=n_trial*BinWidth;

  for(int i_bin=1;i_bin<n_bin+1;i_bin++)
    {
      uni=h_sim->GetXaxis()->GetBinCenter(i_bin);
      probability=TMath::Gaus(uni,uniformity_perfect,sd_uniformity);
      probability*=(TotalArea*2.);
      if(uni<uniformity_perfect)
        {
          probability=0.;
        }
      h_sim->SetBinContent(i_bin,probability);
    }
  
  TFile* f=new TFile("analytic.root","recreate");
  hist->Write();
  h_sim->Write();
  TCanvas* c=new TCanvas("c","c");
  hist->Draw();
  TF1* gaus=new TF1("gausss","gaus",0.11,0.3);
  hist->Fit(gaus);
  gStyle->SetOptFit();
  gStyle->SetOptFit(1111);
  gPad->Update();
  hist->Draw();
  //  h_sim->Draw("sames");
  c->Update();
  c->Write();
  f->Close();
  double chi2=gaus->GetChisquare();
  double DoF=gaus->GetNDF();
  cout<<fixed<<setprecision(15)<<"p value :"<<TMath::Prob(chi2,DoF)<<endl;
}










