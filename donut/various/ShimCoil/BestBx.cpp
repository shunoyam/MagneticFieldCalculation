
#include"../../src/include.h"
using namespace std;

void MakeBx_SVD()
{
  ShimCoil sc("Br");
  double I0=1.e3;
  int mode=2;
  string NormalOrSkew="normal";
  sc.MakeModes(mode,NormalOrSkew,I0,"svd");
  for(int i=0;i<6;i++)
    {
      auto sh=sc.Get1coil(i*2);
      cout<<(sh.I()/1.3717579624e9 -.5)<<endl;
    }
  return;
  ShimCoil sc2("Br");
  sc2.MakeModes(mode,NormalOrSkew,I0,"simple");
  ofstream of("svd.dat");
  double r=33.3e-2;
  int n=1e3;
  vector<double> bx_vec,bx2_vec;
  for(int i=0;i<n;i++)
    {
      double theta=TMath::Pi()*(-1.+2.*i/(n-1.));
      TVector3 mfep(r*cos(theta),r*sin(theta),0.);
      double bx=sc.B(mfep).x();
      double bx2=sc2.B(mfep).x();
      bx_vec.push_back(bx);
      bx2_vec.push_back(bx2);
    }
  double ave_bx=accumulate(bx_vec.begin(),bx_vec.end(),0.)/(double)bx_vec.size();
  double ave_bx2=accumulate(bx2_vec.begin(),bx2_vec.end(),0.)/(double)bx2_vec.size();
  of<<fixed<<setprecision(15);
  for(int i=0;i<n;i++)
    {
      double theta=TMath::Pi()*(-1.+2.*i/(n-1.));
      of<<theta<<"\t"<<bx_vec[i]/ave_bx<<"\t"<<bx2_vec[i]/ave_bx2<<endl;
    }
}

void AverageProblem()
{
  ShimCoil sc("Br");
  double TargetBx=1.;
  int mode=2;
  string NormalOrSkew="normal";
  sc.MakeModes(mode,NormalOrSkew,TargetBx,"svd");
  int bin=100;
  double Z=5.e-2;
  double factor_hist=1.+1./bin;
  double theta_hist=TMath::Pi()*factor_hist;
  double z_hist=Z*factor_hist;
  auto h_out=new TH2D("h_out","B_{x} [T] - 1 (r=31.8 cm);#theta [rad];z [cm]",
                      bin,-theta_hist,theta_hist,
                      bin,-z_hist*1.e2,z_hist*1.e2);
  auto h_center=new TH2D("h_center","B_{x} [T] - 1  (r=33.3 cm);#theta [rad];z [cm]",
                         bin,-theta_hist,theta_hist,
                         bin,-z_hist*1.e2,z_hist*1.e2);
  auto h_in=new TH2D("h_in","B_{x} [T] - 1  (r=34.8 cm);#theta [rad];z [cm]",
                     bin,-theta_hist,theta_hist,
                     bin,-z_hist*1.e2,z_hist*1.e2);
  SetStyle_TH2Ds(h_in,h_center,h_out);
  double r_center=33.3e-2;
  double r_width=1.5e-2;
  double r_in=r_center-r_width;
  double r_out=r_center+r_width;

  Check1TBx(sc,bin,h_out,r_out);
  Check1TBx(sc,bin,h_center,r_center);
  Check1TBx(sc,bin,h_in,r_in);
  auto c=new TCanvas("c","c");
  SetStyle_TCanvas(c);
  c->Divide(1,3);
  c->Draw();
  const char* option="colz";
  c->cd(1);
  h_in->Draw(option);
  c->cd(2);
  h_center->Draw(option);
  c->cd(3);
  h_out->Draw(option);
  auto f=new TFile("AverageProblem.root","recreate");
  c->Write();
  f->Close();
  delete f;
  delete h_in;
  delete h_center;
  delete h_out;
  delete c;
}

static void Check1TBx(ShimCoil sc,int bin,TH2D* h,double r)
{
  auto a=h->GetXaxis();
  a->SetNdivisions(-502);
  a->ChangeLabel(1,-1,-1,-1,-1,-1,"-#pi");
  a->ChangeLabel(-1,-1,-1,-1,-1,-1,"#pi");
  double Z=5.e-2;
  for(int i_z=0;i_z<bin;i_z++)
    {
      for(int i_theta=0;i_theta<bin;i_theta++)
        {
          double z=Z*(-1.+2.*i_z/((bin-1.)));
          double theta=TMath::Pi()*(-1.+2.*i_theta/(bin-1.));
          TVector3 mfep(r*cos(theta),r*sin(theta),z);
          double bx=sc.B(mfep).x();
          h->Fill(theta,z*1.e2,bx-1.);
        }
    }
  ArrangeRange(h);
}

void AverageProblem2()
{
  ShimCoil sc("Br");
  double TargetBx=1.;
  int mode=2;
  string NormalOrSkew="normal";
  sc.MakeModes(mode,NormalOrSkew,TargetBx,"svd");
  eval probe(sc);
  double min=.97;
  double max=1.06;
  int bin=1e2;
  auto h=new TH1D("h","Bx at MFEP; Bx [T];entries [a.u.]",
                  bin,min,max);
  auto Bs=probe.Bs_MFEP();
  for(auto B:Bs)
    {
      double Bx=B.x();
      h->Fill(Bx);
    }
  auto c=new TCanvas("c","c");
  SetStyle_TCanvas(c);
  c->Draw();
  h->Draw();
  auto f=new TFile("AverageProblem2.root","recreate");
  c->Write();
  f->Close();
  delete f;
  delete h;
  delete c;
}
