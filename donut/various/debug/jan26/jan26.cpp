

#include"../../../src/include.h"
using namespace std;

void compare_1000DividedCoil_FLC()
{
  const char* ofn_root="compare_1e5.root";
  magnet m_FLC;
  magnet m_1000DividedCoil;
  m_1000DividedCoil.deformation_elliptical(n_MC10,0.,0.);
  double r_in=r_center_fiducial-r_width_fiducial;
  double r_out=r_center_fiducial+r_width_fiducial;
  double z_low=-z_width_fiducial;
  double z_high=z_width_fiducial;
  auto h_FLC=new TH2D("h_FLC","(B_{z}-3T) to calculate all coils as FLCs;r[cm];#theta[rad]",
                      100,-TMath::Pi(),TMath::Pi(),
                      100,z_low*1.e2,z_high*1.e2);
  
  auto h_div=new TH2D("h_div",
                      "(B_{z}-3T) to calculate MC10 as a set of lines ;r[cm];#theta[rad]",
                      100,-TMath::Pi(),TMath::Pi(),
                      100,z_low*1.e2,z_high*1.e2);
  
  auto h_dif=new TH2D("h_dif","difference;r[cm];#theta[rad]",
                      100,-TMath::Pi(),TMath::Pi(),
                      100,z_low*1.e2,z_high*1.e2);
  puts("drawing start");
  draw_CylindricalSurface(h_FLC,m_FLC);
  puts("drawed 1");
  draw_CylindricalSurface(h_div,m_1000DividedCoil);
  puts("drawed 2");
  draw_CylindricalSurface(h_dif,m_FLC,&m_1000DividedCoil);
  puts("drawed 3");
  const char* option="colz";
  auto c=new TCanvas("c","c");
  SetStyle_TCanvas(c);
  c->Divide(2,2);
  c->cd(1);
  h_FLC->Draw(option);
  c->cd(2);
  h_div->Draw(option);
  c->cd(3);
  c->SetLogz();
  h_dif->Draw(option);
  auto f=new TFile(ofn_root,"recreate");
  c->Write();
  f->Close();
  delete c;
  delete f;
  delete h_FLC;
  delete h_div;
  delete h_dif;
}















