
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
  string ofn_root="WideScan.root";
  double target,U,V,rate_min,rate_ok,rate_max;
  int n_target=40;
  double target_min=2.99;
  double target_max=3.;
  int mode;
  int mode_min=50;
  int mode_max=140;
  double mode_width=mode_max-mode_min;
  cout<<mode_width<<endl;
  double target_width=target_max-target_min;
  double U_require[1];
  U_require[0]=.2;// peak-peak [ppm]
  cout<<mode_min-.5<<endl;
  double U_min=1000.;
  cout<<target_min-target_width*(1.+1./(target_width-1.))<<endl;
  auto h=new TH2D("h",
                  ";Truncated mode;(Target field [T]-3)*1.e6",
                  mode_width,mode_min-.5,mode_max-.5,
                  n_target,(target_min-.5*target_width/n_target-3.)*1.e6,
                  (target_max+.5*target_width/n_target-3.)*1.e6);
      ifstream InputFile("cat_WideScan.dat");
      while(InputFile>>target>>mode>>U>>V>>rate_min>>rate_ok>>rate_max)
        {
          double OffsetTarget=(target-3.)*1.e6;
          h->Fill((double)mode+1.e-6,OffsetTarget,U);
          if(U<U_min)
            {
              U_min=U;
            }
        }
      
  cout<<"Best uniformity: "<<U_min<<" ppm"<<endl;
  auto c=new TCanvas("c","");
  c->Draw();
  double FontSize=0.06;
  h->GetXaxis()->SetTitleSize(FontSize);
  h->GetXaxis()->SetLabelSize(FontSize);

  h->GetYaxis()->SetTitleSize(FontSize);
  h->GetYaxis()->SetLabelSize(FontSize);
  h->GetYaxis()->SetTitleOffset(1.2);
  
  h->GetZaxis()->SetTitle("Achieved uniformity [ppm_{p-p}]");
  h->GetZaxis()->SetTitleSize(FontSize);
  h->GetZaxis()->SetLabelSize(FontSize);
  h->GetZaxis()->SetTitleOffset(1.);
  
  gStyle->SetPalette(55);
  gPad->Update();
  auto st=(TPaveStats*)h->FindObject("stats");
  h->SetStats(0);
  h->DrawCopy("colz");
  h->Draw("cont3 same");
  h->SetContour(1,U_require);
  cout<<mode_max-.5<<endl;
  h->GetXaxis()->SetRangeUser(mode_min-.5,mode_max-.5);
  h->SetLineColor(3);
  h->SetLineWidth(3);
  h->SetLineStyle(9);
  c->SetLogz();
  auto f=new TFile(ofn_root.c_str(),"recreate");
  c->Write();
  h->Write();
  f->Close();
  delete f;
  
}







