


#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<vector>
#include<fstream>
#include<iostream>
#include<iomanip>
#include<cstdio>
#include<numeric>
#include<string.h>

using namespace std;



void ParaScan()
{
  string ifn_dat="ParaScan.dat";
  ifstream InputFile(ifn_dat);
  string line;
  int mode;
  double target,U;
  int n_bin=30;
  int mode_min=30;
  int div_mode=3;
  int mode_max=mode_min+div_mode*(n_bin-1);
  double div_target=20.;//[uT]
  double target_max=0.;
  double target_min=-div_target*(n_bin-1.);
  double U_require[1];
  U_require[0]=.2;// peak-peak [ppm]
  auto h=new TH2D("h",
                  "Reaching local uniformity(p-p) [ppm];Truncated mode;(Target field [T]-3)*1.e6",
                  n_bin,mode_min-div_mode/2.,mode_max+div_mode/2.,
                  n_bin,target_min-div_target/2.,target_max+div_target/2.);
  while(getline(InputFile,line))
    {
      istringstream stream(line);
      stream>>mode>>target>>U;
      double OffsetTarget=(target-3.)*1.e6;
      h->Fill(mode,OffsetTarget,U*1.e6);
    }
  auto c=new TCanvas("c","");
  gStyle->SetPalette(53);
  gPad->Update();
  auto st=(TPaveStats*)h->FindObject("stats");
  h->SetStats(0);
  h->DrawCopy("colz");
  h->SetContour(1,U_require);
  h->Draw("cont3 same");
  h->SetLineColor(3);
  h->SetLineWidth(3);
  h->SetLineStyle(9);
  string ofn_root=ifn_dat.substr(0,ifn_dat.size()-3);
  ofn_root+="root";
  cout<<ofn_root<<endl;
  auto f=new TFile(ofn_root.c_str(),"recreate");
  c->Write();
  h->Write();
  f->Close();
  delete f;
  delete c;
  delete h;
  
}







