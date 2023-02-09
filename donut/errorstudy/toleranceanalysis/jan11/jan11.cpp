

#include"../../../src/include.h"
using namespace std;


void ResidualBx()
{
  int n_trial=1e3;

  vector<XYZVector> dis_vec_XYZ(6);

  const char* ofn_log="ongoing.log";
  rm(ofn_log);
  ofstream log;
  log.open(ofn_log,ios_base::app);

  const char* ofn_dat="U_lc_Bx_0208.dat";
  ofstream of_dat(ofn_dat);
  of_dat<<"#U_lc before iron shimming[ppm] ";
  of_dat<<" U_lc after iron shimming[ppm]  ";
  of_dat<<"residual Bx[uT]"<<endl;
  auto random=new TRandom2();

  ofstream ofn_U_theta_dat("U_theta.dat");
  
  clock_t start=clock();
  log<<n_trial<<"trials will be done"<<endl;
  for(int i_trial=0;i_trial<n_trial;i_trial++)
    {
      dl(2);
      cout<<i_trial<<"trials now"<<endl;
      log<<"--------------------------------------------------------"<<endl;
      log<<i_trial<<"trials now"<<endl;
      OutputElapsedTime(start,ofn_log);
      puts("step 3");
      vector<double> theta_vec=get_theta_vec(random);
      vector<double> phi_vec=get_phi_vec(random);
      vector<TVector3> dis_vec=get_dis_vec(random);

      for(int i=0;i<6;i++)
        {
          dis_vec_XYZ[i]=TVector3_XYZVector(dis_vec[i]);
        }
      vector<double> def_in_vec=get_deformation_inner_vec(random);
      vector<double> def_out_vec=get_deformation_outer_vec(random);
      vector<double> def_thi_vec=get_deformation_thickness_vec(random);

      magnet magnet_g2e;
      magnet_g2e.InputTolerance(theta_vec,phi_vec,dis_vec,
                                def_in_vec,
                                def_out_vec,
                                def_thi_vec);
      eval data_before_shimming(magnet_g2e);
      double average_Bz=data_before_shimming.ave_Bz();
      double min_Bz=data_before_shimming.min_Bz();
      double max_Bz=data_before_shimming.max_Bz();
      double U_lc=data_before_shimming.U_lc()*1.e6;//[ppm]
      double U_theta=data_before_shimming.U_theta()*1.e6;//[ppm]
      ofn_U_theta_dat<<U_theta<<endl;
      mms shims;

      magnet_g2e.shimming_iron(&shims);

      eval data_after_shimming(magnet_g2e,shims);
      data_after_shimming.out();
      average_Bz=data_after_shimming.ave_Bz();
      min_Bz=data_after_shimming.min_Bz();
      max_Bz=data_after_shimming.max_Bz();
      double U_lc_after=data_after_shimming.U_lc()*1.e6;//[ppm]

      double Br_min_residual=data_after_shimming.min_Br();
      double Br_max_residual=data_after_shimming.max_Br();
      double B_penetrate=(Br_max_residual-Br_min_residual)/2.;
      of_dat<<U_lc<<"\t"<<U_lc_after<<"\t"<<B_penetrate*1.e6/*[uT]*/<<endl;
    }
}

void correlation_U_lc_residualBx()
{
  const char* ifn_dat="U_lc_Bx.dat";
  const char* ofn_root="U_lc_Bx_0121.root";
  ifstream ifn(ifn_dat);
  if(ifn.fail())
    {
      cout<<ifn_dat<<" was not found"<<endl;
      return;
    }
  string line;
  double U_lc_before,U_lc_after,residualBx;
  vector<double> U_lc_before_vec,U_lc_after_vec,residualBx_vec;
  for(int row=0;getline(ifn,line);row++)
    {
      istringstream stream(line);
      for(int col=0;stream>> U_lc_before >>U_lc_after >>residualBx ;col++)
        {
          U_lc_before_vec.push_back(U_lc_before);
          U_lc_after_vec.push_back(U_lc_after);
          residualBx_vec.push_back(residualBx);
        }
    }

  const char* ifn_dat_U_theta="U_theta.dat";
  ifstream ifn_U_theta(ifn_dat_U_theta);
  if(ifn_U_theta.fail())
    {
      cout<<ifn_dat_U_theta<<" was not found"<<endl;
      return;
    }
  double U_theta;
  vector<double> U_theta_vec;
  for(int row=0;getline(ifn_U_theta,line);row++)
    {
      istringstream stream(line);
      double U_theta=stod(line);
      U_theta_vec.push_back(U_theta);
    }
  draw_correlation(U_lc_before_vec,residualBx_vec,ofn_root,U_theta_vec);
}

static void draw_correlation(vector<double> vector1,vector<double> vector2,
                             const char* ofn_root,vector<double> U_theta_vec)
{
  string title="uniformity before shimming and penetrating field after shimming";
  string label_x="U_{lc}[ppm]";
  string label_y="B_{x,y}^{res}[Gauss]";
  string _2ndArgument=title+";"+label_x+";"+label_y;
  double max_U_lc_before=897.;//ppm
  double max_residualBx=2.e1;//Gauss
  auto h=new TH2D("h",_2ndArgument.c_str(),100,0.,max_U_lc_before,
                  100,0.,max_residualBx);
  auto h_ratio=new TH1D("h_ratio",";U_{#theta}/U_{lc};entries[a.u]",
                        100,0.,0.8);
  auto h_res=new TH1D("h_res",";B_{x,y}[Gauss];entries[a.u]",
                      100,0.,max_residualBx);
  SetStyle_TH1D(h_ratio);
  SetStyle_TH1D(h_res);
  SetStyle_TH2D(h);
  int n_trial=vector1.size();
  if(n_trial!=vector2.size())
    {
      cout<<"size "<<n_trial<<" and "<<vector2.size()<<" is different"<<endl;
      return;
    }
  for(int i_trial=0;i_trial<n_trial;i_trial++)
    {
      double U_lc=vector1[i_trial];
      double U_theta=U_theta_vec[i_trial];
      double U_rot=U_lc;
      double ratio=U_theta/U_lc;
      h_ratio->Fill(ratio);
      double y=vector2[i_trial]/1.e2;//[Gauss]
      h->Fill(U_rot,y);
      h_res->Fill(y);
    }
  double CF=h->GetCorrelationFactor();
  cout<<"correlation factor: "<<CF<<endl;
  auto c=new TCanvas("c","c");
  SetStyle_TCanvas(c);
  c->Divide(2,2);
  c->cd(1);
  h->Draw("colz");
  c->cd(2);
  h_ratio->Draw();
  c->cd(3);
  h_res->Draw();
  auto f=new TFile(ofn_root,"recreate");
  c->Write();
  f->Close();
  delete f;
  delete c;
  delete h;  
}

void MostBxSearch()
{
  int line_WorstCase=240;
  const char* ofn_root="MostBx.root";


    int n_trial=line_WorstCase+1;

  vector<XYZVector> dis_vec_XYZ(6);


  const char* ofn_dat="U_lc_Bx_done.dat";
  ofstream of_dat(ofn_dat);
  auto random=new TRandom2();
  
  for(int i_trial=0;i_trial<n_trial;i_trial++)
    {
      vector<double> theta_vec=get_theta_vec(random);
      vector<double> phi_vec=get_phi_vec(random);
      vector<TVector3> dis_vec=get_dis_vec(random);

      for(int i=0;i<6;i++)
        {
          dis_vec_XYZ[i]=TVector3_XYZVector(dis_vec[i]);
        }

      vector<double> def_in_vec=get_deformation_inner_vec(random);
      vector<double> def_out_vec=get_deformation_outer_vec(random);
      vector<double> def_thi_vec=get_deformation_thickness_vec(random);

      if(i_trial<line_WorstCase-1)
        {
          continue;
        }

      cout<<i_trial<<" now"<<endl;
      magnet magnet_g2e;
      magnet_g2e.InputTolerance(theta_vec,phi_vec,dis_vec,
                                def_in_vec,
                                def_out_vec,
                                def_thi_vec);

      eval data_before_shimming(magnet_g2e);
      data_before_shimming.out();
      continue;
      double average_Bz=data_before_shimming.ave_Bz();
      double min_Bz=data_before_shimming.min_Bz();
      double max_Bz=data_before_shimming.max_Bz();
      double U_lc=data_before_shimming.U_lc()*1.e6;//[ppm]
      double U_theta=data_before_shimming.U_theta()*1.e6;//[ppm]

      mms shims;

      magnet_g2e.shimming_iron(&shims);
      cout<<"out! 1st volume: "<<shims.volume(0)*1.e6<<endl;
      eval data_after_shimming(magnet_g2e,shims);
      data_after_shimming.out();
      average_Bz=data_after_shimming.ave_Bz();
      min_Bz=data_after_shimming.min_Bz();
      max_Bz=data_after_shimming.max_Bz();
      
      double U_lc_after=data_after_shimming.U_lc()*1.e6;//[ppm]
      double Br_min_residual=data_after_shimming.min_Br();
      double Br_max_residual=data_after_shimming.max_Br();
      double B_penetrate=(Br_max_residual-Br_min_residual)/2.;
    }
}



























