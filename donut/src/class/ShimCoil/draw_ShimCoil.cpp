
#include"../../include.h"

using namespace std;

void ShimCoil::drawB(const char* ofn_root)
{
  auto c_cylinder=new TCanvas("cy","cy");
  auto c_xz=new TCanvas("xz","xz");
  ShimCoil::draw_cylinder(c_cylinder);
  ShimCoil::draw_xz(c_xz);
  auto outfile=new TFile(ofn_root,"recreate");
  c_cylinder->Write();
  c_xz->Write();
  outfile->Close();
  delete c_cylinder;
  delete c_xz;
  delete outfile;
}

void ShimCoil::draw_cylinder(TCanvas* c)
{
  double r_c=r_center_fiducial;
  double r_w=r_width_fiducial;
  double z_c=z_center_fiducial;
  double z_w=z_width_fiducial;
  int n_z=100;
  int n_theta=400;
  double range_theta=TMath::Pi()*n_theta/(n_theta-1.);
  double buffer_z=.1;//[cm]
  double range_z=buffer_z+1.e2*z_w*n_z/(n_z-1.);//[cm]
  auto h_out_bz=new TH2D("h_out_bz",
                         "Bz[T] on the outer surface;#theta[rad];z[cm];",
                         n_theta,-range_theta,range_theta,
                         n_z,-range_z,range_z);
  auto h_in_bz=new TH2D("h_in_bz",
                        "Bz[T] on the inner surface;#theta[rad];z[cm];",
                        n_theta,-range_theta,range_theta,
                        n_z,-range_z,range_z);
  auto h_center_bz=new TH2D("h_center_bz",
                            "Bz[T] on the center surface;#theta[rad];z[cm];",
                            n_theta,-range_theta,range_theta,
                            n_z,-range_z,range_z);

  auto h_out_br=new TH2D("h_out_br",
                         "Br[T] on the outer surface;#theta[rad];z[cm];",
                         n_theta,-range_theta,range_theta,
                         n_z,-range_z,range_z);
  auto h_in_br=new TH2D("h_in_br",
                        "Br[T] on the inner surface;#theta[rad];z[cm];",
                        n_theta,-range_theta,range_theta,
                        n_z,-range_z,range_z);
  auto h_center_br=new TH2D("h_center_br",
                            "Br[T] on the center surface;#theta[rad];z[cm];",
                            n_theta,-range_theta,range_theta,
                            n_z,-range_z,range_z);
  ShimCoil::fill_cylinder(r_c+r_w,h_out_br,h_out_bz,n_z,n_theta);
  ShimCoil::fill_cylinder(r_c,h_center_br,h_center_bz,n_z,n_theta);
  ShimCoil::fill_cylinder(r_c-r_w,h_in_br,h_in_bz,n_z,n_theta);

  const char* option="colz";
  c->Divide(2,3);
  c->cd(1);
  h_out_bz->Draw(option);
  c->cd(2);
  h_out_br->Draw(option);
  c->cd(3);
  h_center_bz->Draw(option);
  c->cd(4);
  h_center_br->Draw(option);
  c->cd(5);
  h_in_bz->Draw(option);
  c->cd(6);
  h_in_br->Draw(option);
}


void ShimCoil::fill_cylinder(double r,TH2D* h_br,TH2D* h_bz,int n_z,int n_theta)
{
  SetCylinderHist(h_br);
  SetCylinderHist(h_bz);
  int n_bin=h_br->GetYaxis()->GetNbins();
  double z_w=(h_br->GetYaxis()->GetBinCenter(n_bin)-h_br->GetYaxis()->GetBinCenter(1))/2.;
  z_w/=1.e2;//[cm]->[m]
  for(int i_z=0;i_z<n_z;i_z++)
    {
      for(int i_theta=0;i_theta<n_theta+1;i_theta++)
        {
          double theta=TMath::Pi()*(-1.+2.*(double)i_theta/(n_theta-1.));
          double x=r*cos(theta);
          double y=r*sin(theta);
          double z=z_w*(-1.+2.*(double)i_z/(n_z-1.));
          TVector3 mfep(x,y,z);
          TVector3 b=ShimCoil::B(mfep);
          z*=1.e2;//[m]->[cm]
          double br=b.Dot(TVector3(cos(theta),sin(theta),0.));
          h_br->Fill(theta,z,br);
          h_bz->Fill(theta,z,b.z());
        }
    }
}

void ShimCoil::draw_xz(TCanvas* c)
{
  
}
  

