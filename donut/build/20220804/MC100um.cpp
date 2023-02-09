

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



void MC100um()
{
  ifstream i("MC.dat");
  double U;
  string line;
  double max=920/5.;
  auto h=new TH1D("h",";B_{z} uniformity (ppm_{p-p});Events",
                  100,0,max);
  gStyle->SetNdivisions(305);
  double U_max=0.;
  while(getline(i,line))
    {
      istringstream stream(line);
      stream>>U;
      U=U*1.e6 /5.;
      if(U_max<U)
        {
          U_max=U;
        }
      h->Fill(U);
    }
  gStyle->SetNdivisions(305);
  h->SetNdivisions(305);
  h->SetNdivisions(305);
  h->GetYaxis()->SetNdivisions(305);
  auto ar=new TArrow(U_max-2,1.3,U_max-10,4,0.02,"<|-");
  ar->SetLineColor(1);
  ar->SetFillColor(1);
  ar->SetLineWidth(4);
  auto c=new TCanvas("c","");
  c->SetLogy();
  double FontSize=.07;
  h->GetXaxis()->SetTitleSize(FontSize);
  h->GetYaxis()->SetTitleSize(FontSize);
  h->GetXaxis()->SetLabelSize(FontSize);
  h->GetYaxis()->SetLabelSize(FontSize);
  h->GetYaxis()->SetTitleOffset(.8);
  h->SetLineWidth(3);
  h->SetLineColor(4);//blue
  c->SetLeftMargin(.12);
  c->SetLineWidth(2);
  c->SetBottomMargin(.17);
  h->Draw();
  auto st=(TPaveStats*)h->FindObject("stats");
  h->SetStats(0);
  gPad->Update();
  c->Draw();
  h->Draw();
  ar->Draw();
  auto f=new TFile("100um.root","recreate");
  c->Write();
  h->Write();
  f->Close();
  

}







