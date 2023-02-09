

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

void Set(TEllipse* e)
{
  e->SetLineWidth(2);
  e->SetFillColor(0);
  e->SetFillStyle(4000);
  e->SetLineColor(0);
  e->SetLineStyle(2);
}

void OverlappedDistribution()
{
  auto f=new TFile("FinalAzimuthalBzDistribution.root");
  auto c=(TCanvas*)f->Get("c");
  c->Draw();
  auto h=(TH2D*)c->GetPrimitive("h_azi_Bz");
  h->GetZaxis()->SetTitle("");
  h->GetXaxis()->SetNdivisions(303);
  gPad->SetLeftMargin(0.4);
  gPad->SetRightMargin(0.4);
  gPad->SetTopMargin(0.1);
  gPad->SetBottomMargin(0.1);
  gPad->Update();
  double r_c=33.3;
  double r_w=1.5;
  double z_w=5.;
  double sigma0_r=.3;
  double sigma0_z=1.;
  
  for(int i=0;i<3;i++)
    {
      double sigma_r=sigma0_r*(i+1.);
      double sigma_z=sigma0_z*(i+1.);      
      string name_up="up"+to_string(i);
      auto up=new TEllipse(r_c,0,sigma_r,sigma_z,0,360);
      Set(up);
      up->Draw("sames");      
      auto low=new TEllipse(r_c,0,sigma_r,sigma_z,0,360);
      Set(low);
      low->Draw("sames");
    }
}
