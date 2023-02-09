
#ifndef _IN_NOV15
#define _IN_NOV15

#include"../../../src/include.h"
using namespace std;

void ToleranceAnalysis_nov15(const char* ifn_root)
{
  string ifn_root_str=ifn_root;
  string extension_root=".root";
  int size_extension=extension_root.size();
  for(int i_size=0;i_size<size_extension;i_size++)
    {
      ifn_root_str.pop_back();
    }
  string AdditionalCharactor="_ToleAna_nov15";
  string ofn_root_str=ifn_root_str+AdditionalCharactor+extension_root;
  const char* ofn_root_char=ofn_root_str.c_str();
  TFile *fin = new TFile(ifn_root, "read");
  TTree *tin = (TTree*)fin->Get("uni");
  int option_stat=1110;
  vector<double> *theta_vec=0,*phi_vec=0,*def_in_vec=0,*def_out_vec=0,*def_thi_vec=0;
  vector<XYZVector>* dis_vec_XYZ=0;
  TBranch *bra_theta_vec=0,*bra_phi_vec=0,*bra_def_in_vec=0,*bra_def_out_vec=0;
  TBranch *bra_def_thi_vec=0,*bra_dis_vec_XYZ=0;
  double size_font=0.05;
  double min_Br,max_Br,min_Bz,max_Bz,average_Bz,average_Br,uniformity;
  double max_Bz_azi,min_Bz_azi;

  tin->SetBranchAddress("theta_vec",&theta_vec,&bra_theta_vec);
  tin->SetBranchAddress("phi_vec",&phi_vec,&bra_phi_vec);
  tin->SetBranchAddress("def_thi_vec",&def_thi_vec,&bra_def_thi_vec);
  tin->SetBranchAddress("def_in_vec",&def_in_vec,&bra_def_in_vec);
  tin->SetBranchAddress("def_out_vec",&def_out_vec,&bra_def_out_vec);
  tin->SetBranchAddress("min_Br",&min_Br);
  tin->SetBranchAddress("max_Br",&max_Br);
  tin->SetBranchAddress("min_Bz",&min_Bz);
  tin->SetBranchAddress("max_Bz",&max_Bz);
  tin->SetBranchAddress("min_Bz_azi",&min_Bz_azi);
  tin->SetBranchAddress("max_Bz_azi",&max_Bz_azi);
  tin->SetBranchAddress("average_Bz",&average_Bz);
  tin->SetBranchAddress("average_Br",&average_Br);
  tin->SetBranchAddress("uniformity",&uniformity);
  tin->SetBranchAddress("dis_vec_XYZ",&dis_vec_XYZ,&bra_dis_vec_XYZ);

  string title_h="B_{z}^{err} averaged in fiducial region;";
  string title_x="<B_{z}^{err}>_{3D}[#muT];";
  string title_y="entries[a.u]";
  string augument=title_h+title_x+title_y;
  int bin=100;
  double Bz_err_0=4.05842;
  double ave_Bz_err;
  double min_ave_Bz_err=-3.e3;
  double max_ave_Bz_err=-min_ave_Bz_err;
  auto* h_ave_Bz=new TH1D("h_ave_Bz",augument.c_str(),bin,min_ave_Bz_err,max_ave_Bz_err);
  SetStyle_TH1D(h_ave_Bz);
  h_ave_Bz->GetXaxis()->SetTitleSize(size_font);
  h_ave_Bz->GetYaxis()->SetTitleSize(size_font);
  h_ave_Bz->GetXaxis()->SetLabelSize(size_font);
  h_ave_Bz->GetYaxis()->SetLabelSize(size_font);

  TH2D* h_reasoning=new TH2D("h_reason","correlation between measured uniformity and Tolerance factor;azimuthally averaged uniformity [ppm];Tolerance factor [a.u]",bin,0.,810.,
                             bin,0.,820);
  SetStyle_TH2D(h_reasoning);
  title_h="B_{r} averaged in fiducial region;";
  title_x="<B_{r}>_{3D}[#muT];";
  title_y="entries[a.u]";
  augument=title_h+title_x+title_y;
  double ave_Br;
  double min_ave_Br=-1.e3;
  double max_ave_Br=-min_ave_Br;
  TH1D* h_ave_Br=new TH1D("h_ave_Br",augument.c_str(),bin,min_ave_Br,max_ave_Br);
  SetStyle_TH1D(h_ave_Br);
  h_ave_Br->GetXaxis()->SetTitleSize(size_font);
  h_ave_Br->GetYaxis()->SetTitleSize(size_font);
  h_ave_Br->GetXaxis()->SetLabelSize(size_font);
  h_ave_Br->GetYaxis()->SetLabelSize(size_font);
  title_h="p-p of Br in the fiducial region;";
  title_x="p-p of Br[#muT];";
  augument=title_h+title_x+title_y;
  double p2p_Br;
  double min_p2p_Br=0.;
  double max_p2p_Br=4.e3;
  TH1D* h_p2p_Br=new TH1D("h_p2p_Br",augument.c_str(),bin,min_p2p_Br,max_p2p_Br);
  SetStyle_TH1D(h_p2p_Br);
  h_p2p_Br->GetXaxis()->SetTitleSize(size_font);
  h_p2p_Br->GetYaxis()->SetTitleSize(size_font);
  h_p2p_Br->GetXaxis()->SetLabelSize(size_font);
  h_p2p_Br->GetYaxis()->SetLabelSize(size_font);

  
  title_h="azimuthally averaged uniformity;";
  title_x="uniformity[ppm];";
  augument=title_h+title_x+title_y;
  double unif_theta;
  double min_unif=0.;
  double max_unif=300.;
  TH1D* h_unif=new TH1D("h_unif",augument.c_str(),bin,min_unif,max_unif);
  SetStyle_TH1D(h_unif);

  title_h="local uniformity in the fiducial region;";
  title_x="uniformity [ppm];";
  augument=title_h+title_x+title_y;
  double max_unif_local=900.;
  TH1D* h_unif_local=new TH1D("h_unif_local",
                              augument.c_str(),bin,min_unif,max_unif_local);
  SetStyle_TH1D(h_unif_local);
  double unif_local;


  double worst_unif=0.,worst_p2p_Br=0.,worst_ave_Br=0.,worst_ave_Bz=0.;
  double min_index=-200.,max_index=200.;
  double index;
  TH1D* h_index=new TH1D("h_index","Some index should be max at worse case;x;y",
                         bin,min_index,max_index);  
  SetStyle_TH1D(h_index);
  int N = tin->GetEntries();
  
  for (int ientry=0;ientry<N;ientry++)
    {
      tin->GetEntry(ientry);
      int tentry=tin->LoadTree(ientry);
      bra_theta_vec->GetEntry(tentry);
      bra_phi_vec->GetEntry(tentry);
      bra_dis_vec_XYZ->GetEntry(tentry);
      bra_def_in_vec->GetEntry(tentry);
      bra_def_out_vec->GetEntry(tentry);
      p2p_Br=(max_Br-min_Br)*1.e6;//[uT]
      unif_theta=(max_Bz_azi-min_Bz_azi)*1.e6/average_Bz;//[ppm]
      ave_Bz_err=(average_Bz-3.)*1.e6-Bz_err_0;//[uT]
      ave_Br=average_Br*1.e6;//[uT]
      h_unif->Fill(unif_theta);
      h_ave_Bz->Fill(ave_Bz_err);
      h_ave_Br->Fill(ave_Br);
      h_p2p_Br->Fill(p2p_Br);
      //     index=index_axial(*dis_vec_XYZ,*def_in_vec,*def_out_vec);
           index=ToleranceFactor_local(*dis_vec_XYZ,*def_in_vec,*def_out_vec,
                                *theta_vec,*phi_vec);
      unif_local=(max_Bz-min_Bz)*1.e6/average_Bz;//[ppm]
      h_unif_local->Fill(unif_local);
      h_reasoning->Fill(unif_local,index);
      //      h_reasoning->Fill(unif_local,abs(index));
      if(worst_unif<unif_theta || worst_ave_Bz<abs(ave_Bz_err) || 
         worst_ave_Br<abs(ave_Br) || worst_p2p_Br<p2p_Br)
        {
          puts("----------------------------------");
          if(worst_unif<unif_theta)
            {
              worst_unif=unif_theta;
              cout<<"Worst uniformity is updated at "<<ientry<<"th trial."<<endl;
              cout<<"index :"<<index<<endl;
              cout<<"azimuthally averaged uniformity :"<<unif_theta<<" ppm"<<endl;
              cout<<"local uniformity :"<<unif_local<<"[ppm]"<<endl;
              coutInputParameters(dis_vec_XYZ,def_in_vec,def_out_vec,def_thi_vec,
                                  theta_vec,phi_vec);
            }
          if(worst_ave_Bz<abs(ave_Bz_err))
            {
              worst_ave_Bz=abs(ave_Bz_err);
              cout<<"Worst <Bz> is updated at "<<ientry<<"th trial."<<endl;
            }
          if(worst_ave_Br<abs(ave_Br))
            {
              worst_ave_Br=abs(ave_Br);
              cout<<"Worst <Br> is updated at "<<ientry<<"th trial."<<endl;
            }
          if(worst_p2p_Br<p2p_Br)
            {
              worst_p2p_Br=p2p_Br;
              cout<<"Worst p-p of Br is updated at "<<ientry<<"th trial."<<endl;
            }

        }
    }
  puts("-----------------");
  double Correlation_unif_index=h_reasoning->GetCorrelationFactor();
  cout<<"Correlation factor: "<<Correlation_unif_index<<endl;
  auto* c=new TCanvas("c","c");
  c->Divide(2,2);
  c->cd(1);
  SetStyle_TCanvas(c);
  h_unif->Draw();
  c->cd(2);
  SetStyle_TCanvas(c);
  h_ave_Bz->Draw();
  c->cd(3);
  SetStyle_TCanvas(c);
  h_ave_Br->Draw();
  c->cd(4);
  SetStyle_TCanvas(c);
  h_p2p_Br->Draw();
  TCanvas* c2=new TCanvas("c2","c2");
  c2->Divide(2,2);
  c2->cd(1);
  SetStyle_TCanvas(c2);
  h_index->Draw();
  c2->cd(2);
  SetStyle_TCanvas(c2);
  h_reasoning->Draw("colz");
  c2->cd(3);
  SetStyle_TCanvas(c2);
  h_unif_local->Draw();
  TFile* file_root=new TFile(ofn_root_char,"recreate");
  c->Write();
  c2->Write();
  file_root->Close();
  delete c;
  delete file_root;
}

void mesh_worstcase()
{
  const char* path_to_configfile="../config/ConfigurationFile/global.h";

  SetSeparation(3);
  ReadConfigFile(path_to_configfile);
  string ifn_root="allpara_10000trial_1115_3mesh.root";
  ToleranceGenerator_nov5(ifn_root.c_str());

  SetSeparation(10);
  ReadConfigFile(path_to_configfile);
  ifn_root="allpara_10000trial_1115_10mesh.root";
  ToleranceGenerator_nov5(ifn_root.c_str());

  SetSeparation(100);
  ReadConfigFile(path_to_configfile);
  ifn_root="allpara_10000trial_1115_100mesh.root";
  ToleranceGenerator_nov5(ifn_root.c_str());

  SetSeparation(1000);
  ReadConfigFile(path_to_configfile);
  ifn_root="allpara_10000trial_1115_1000mesh.root";
  ToleranceGenerator_nov5(ifn_root.c_str());
}



#endif
