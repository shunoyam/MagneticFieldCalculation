

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

double intensity(double r,double z,double sigma_r,double sigma_z)
{
  double r_center=33.3e-2;
  double shoulder=pow((r-r_center)/sigma_r,2)+pow(z/sigma_z,2);
  shoulder*=-1./2.;
  double numerator=exp(shoulder);
  return numerator;
}

void EachDistribution(TCanvas* c_all,int i,string sigma_r_cm,string sigma_z_cm)
{
  double sigma_r=stod(sigma_r_cm)/100.;
  double sigma_z=stod(sigma_z_cm)/100.;
  string ofn_dat=to_string(i)+"distribution.dat";
  string ofn_root=to_string(i)+"distribution.root";
  string HistName="h"+to_string(i);
  double r_c=33.3e-2;
  double r_w=1.5e-2;
  double z_w=20.e-2;
  int n_z=100;
  int n_r=300;
  ofstream of(ofn_dat.c_str());
  double r_min=r_c-r_w*(1.+1./(double)n_r);
  double r_max=r_c+r_w*(1.+1./(double)n_r);
  double z_max=z_w*(1.+1./(double)n_z);
  string title="(#sigma_{r}, #sigma_{z}) =("+sigma_r_cm+" cm, "+sigma_z_cm+" cm); r [cm]; z [cm]";
  auto h=new TH2D(HistName.c_str(),
                  title.c_str(),n_r,r_min*1.e2,r_max*1.e2,n_z,-z_max*1.e2,z_max*1.e2);
  for(int i_r=0;i_r<n_r;i_r++)
    {
      for(int i_z=0;i_z<n_z;i_z++)
        {
          double z=z_w*(-1.+2.*i_z/(n_z-1.));
          double r=r_c+r_w*(-1.+2.*i_r/(n_r-1.));
          double I=intensity(r,z,sigma_r,sigma_z);
          of<<r<<"\t"<<z<<"\t"<<I<<endl;
          h->Fill(r*1.e2,z*1.e2,I);
        }
    }
  double max=h->GetMaximum();
  cout<<"max: "<<max<<endl;
  h->Scale(1./max);
  auto c=new TCanvas("c","c");
  int fontid=42;
  gStyle->SetStatFont(fontid);
  gStyle->SetLabelFont(fontid,"XYZ");
  gStyle->SetLabelFont(fontid,"");
  gStyle->SetTitleFont(fontid,"XYZ");
  gStyle->SetTitleFont(fontid,"");
  gStyle->SetTextFont(fontid);
  gStyle->SetLegendFont(fontid);
  double size_font=.07;
  double xLabelOffset=-0.02;
  double title_offset_x=.6;
  double title_offset_y=.7;
  int option_stat=0;
  int color=103;
  gStyle->SetPalette(color);
  TColor::InvertPalette();
  gPad->Update();
  auto st=(TPaveStats*)h->FindObject("stats");
  h->GetXaxis()->CenterTitle(true);
  h->GetYaxis()->CenterTitle(true);
  h->GetXaxis()->SetLabelSize(0.07);
  h->GetXaxis()->SetTitleSize(size_font);
  //  h->SetTitleSize(size_font*3.);
  h->GetXaxis()->SetTitleOffset(title_offset_x);
  h->GetXaxis()->SetLabelOffset(xLabelOffset);
  h->GetYaxis()->SetLabelSize(size_font);
  h->GetYaxis()->SetTitleSize(size_font);
  h->GetYaxis()->SetTitleOffset(title_offset_y);
  h->GetZaxis()->SetTitleSize(size_font*4.);
  h->GetZaxis()->SetLabelSize(0.07);
  h->SetStats(option_stat);
  h->SetLineWidth(2);
  h->SetTitleSize(2.1,"z");//0.8,"z");
  h->Draw("colz");
  h->GetXaxis()->SetNdivisions(305);
  gStyle->SetPalette(color);
  gPad->Update();
  c_all->cd(i+1);
  c_all->Draw();
  gPad->SetLeftMargin(0.2);
  gPad->SetRightMargin(0.2);
  gPad->SetTopMargin(0.1);
  gPad->SetBottomMargin(0.1);
  gPad->Update();
  //  auto f=new TFile(ofn_root.c_str(),"recreate");
  //  c->Write(); 
  h->Draw("colz");
  //  f->Close();
  //  delete c;
  //  delete h;
  //  delete f;
}

void ThreeDistributions()
{
  auto f=new TFile("3distributions.root","recreate");
  auto c=new TCanvas("c3","c3");
  vector<string> sigmas_r{"0.1","0.1","0.5"};//cm
  vector<string> sigmas_z{"10.0","2.0","2.0"};//cm
  c->Divide(sigmas_r.size(),1);
  for(int i=0;i<sigmas_r.size();i++)
    {
      EachDistribution(c,i,sigmas_r[i],sigmas_z[i]);
    }
  c->Write();
  f->Close();
  delete c;
  delete f;
}







