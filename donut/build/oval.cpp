

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

void oval()
{
  double theta,z,Bz;
  int n_theta=100;
  int n_z=40;
  double theta_min=-(1.+1./(double)n_theta);
  double theta_max=1.;
  double z_min=-5.*(1.+1./(double)n_z);
  double z_max=5.*(1.+1./(double)n_z);
  auto h=new TH2D("h",";#theta (rad/#pi);z (cm)",
                  n_theta,theta_min,theta_max,
                  n_z,z_min,z_max);
  ifstream InputFile("oval_MC30.dat");
  double ave=0.;
  while(InputFile>>theta>>z>>Bz)
    {
      ave+=Bz;
    }
  ave/=(double)(n_theta*n_z);
  InputFile.clear();
  InputFile.seekg(0,ios_base::beg);
  while(InputFile>>theta>>z>>Bz)
    {
      theta/=TMath::Pi();
      z*=1.e2;//[m]->[cm]
      double dif=(Bz-ave)*1.e6 /ave;
      h->Fill(theta,z,dif);
      cout<<theta<<"\t"<<z<<"\t"<<dif<<endl;
    }
  h->Draw("colz");
  h->GetXaxis()->SetTitleSize(.07);
  h->GetYaxis()->SetTitleSize(.07);
  h->GetXaxis()->CenterTitle(true);
  h->GetYaxis()->CenterTitle(true);
  h->GetYaxis()->SetTitleOffset(.4);
  h->GetXaxis()->SetLabelSize(.07);
  h->GetYaxis()->SetLabelSize(.07);
  h->GetZaxis()->SetLabelSize(.07);
  h->GetXaxis()->SetNdivisions(505);
  h->GetYaxis()->SetNdivisions(305);
  h->GetZaxis()->SetNdivisions(3);
  auto st=(TPaveStats*)h->FindObject("stats");
  auto c=new TCanvas("c","");
  c->Draw();
  h->SetStats(0);
  h->Draw("colz");
  auto f=new TFile("oval_MC30.root","recreate");
  c->Write();
  h->Write();
  f->Close();
  delete f;
  delete c;
  delete h;
}
