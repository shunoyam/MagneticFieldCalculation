
#include"../../include.h"
using namespace std;

Draw::Draw(cb cb,string _ofn_root)
{
  this->ofn_root=_ofn_root;
  _cb=cb;
  sources="cb";
  Draw::drawing();
}

Draw::Draw(ShimCoil ShimCoil,string _ofn_root)
{
  this->ofn_root=_ofn_root;
  _ShimCoil=ShimCoil;
  sources="ShimCoil";
  Draw::drawing();
}

Draw::Draw(magnet magnet,mms mms,ShimCoil ShimCoil,string _ofn_root)
{
  this->ofn_root=_ofn_root;
  this->_magnet=magnet;
  this->_ShimCoil=ShimCoil;
  this->_mms=mms;
  sources="all";
  Draw::drawing();
}

Draw::Draw(magnet magnet,string _ofn_root)
{
  this->ofn_root=_ofn_root;
  _magnet=magnet;
  sources="magnet";
  Draw::drawing();
}

Draw::Draw(magnet magnet,mms mms,string _ofn_root)
{
  this->ofn_root=_ofn_root;
  _mms=mms;
  _magnet=magnet;
  sources="magnetmms";
  Draw::drawing();
}

Draw::Draw(mms mms,string _ofn_root)
{
  this->ofn_root=_ofn_root;
  _mms=mms;
  sources="mms";
  Draw::drawing();
}

void Draw::drawing()
{
  TH2D* h_xz_Br,*h_xz_Bz,*h_azi_Br,*h_azi_Bz;
  TH2D* h_in_Br,*h_center_Br,*h_out_Br,*h_in_Bz,*h_center_Bz,*h_out_Bz;
  this->c_xz=new TCanvas("xz","xz");
  this->c_cylinder=new TCanvas("cy","cy");
  Draw::cylinder(h_in_Bz,h_center_Bz,h_out_Bz,h_in_Br,h_center_Br,h_out_Br);
  //  Draw::xz(h_xz_Br,h_xz_Bz,h_azi_Br,h_azi_Bz);
  auto outfile=new TFile(ofn_root.c_str(),"recreate");
  c_cylinder->Write();
  c_xz->Write();
  // h_xz_Br->Write();
  // h_azi_Br->Write();
  // h_xz_Bz->Write();
  // h_azi_Bz->Write();
  // h_in_Br->Write();
  // h_center_Br->Write();
  // h_out_Br->Write();
  // h_in_Bz->Write();
  // h_center_Bz->Write();
  //  h_out_Bz->Draw("colz");
  //  h_out_Bz->Write();
  outfile->Close();
  //  delete c_cylinder;
  //  delete c_xz;
  delete outfile;
}

void Draw::cylinder(TH2D* h_in_bz,TH2D* h_center_bz,TH2D* h_out_bz,
                    TH2D* h_in_br,TH2D* h_center_br,TH2D* h_out_br)
{
  double range_theta=(double)n_theta/(n_theta-1.);
  double buffer_z=1.;//(mm)
  double range_z=buffer_z+1.e3*z_w*n_z/(n_z-1.);//(mm)
  h_out_bz=new TH2D("h_out_bz",
                    "Bz[T] on the outer surface;#theta(rad/#pi);z(cm);",
                    n_theta,-range_theta,range_theta,
                    n_z,-range_z,range_z);
  h_in_bz=new TH2D("h_in_bz",
                   "Bz[T] on the inner surface;#theta(rad/#pi);z(cm);",
                   n_theta,-range_theta,range_theta,
                   n_z,-range_z,range_z);
  h_center_bz=new TH2D("h_center_bz",
                       ";#theta (rad/#pi);z (mm);",
                       n_theta,-range_theta,range_theta,
                       n_z,-range_z,range_z);

  h_out_br=new TH2D("h_out_br",
                    "Br[T] on the outer surface;#theta(rad/#pi);z(cm);",
                    n_theta,-range_theta,range_theta,
                    n_z,-range_z,range_z);
  h_in_br=new TH2D("h_in_br",
                   "Br[T] on the inner surface;#theta(rad/#pi);z(cm);",
                   n_theta,-range_theta,range_theta,
                   n_z,-range_z,range_z);
  h_center_br=new TH2D("h_center_br",
                       ";#theta (rad/#pi);z (cm);",
                       n_theta,-range_theta,range_theta,
                       n_z,-range_z,range_z);
  
  //  Draw::fill_cylinder(r_c+r_w,h_out_br,h_out_bz);
  Draw::fill_cylinder(r_c,h_center_br,h_center_bz);
  //  Draw::fill_cylinder(r_c-r_w,h_in_br,h_in_bz);
  
  auto c=new TCanvas("c","");
  c->SetBottomMargin(.15);
  c->SetRightMargin(.17);
  h_center_bz->GetZaxis()->SetTitle("(B_{z} - B_{z}^{ave}) / B_{z}^{ave} (ppm)");
  h_center_bz->GetZaxis()->SetTitleSize(.07);
  h_center_bz->GetZaxis()->SetTitleOffset(.8);
  h_center_bz->Draw("colz");
  auto f=new TFile("bz_cylinder.root","recreate");
  c->Write();
  f->Close();
  delete c;
  delete f;
  const char* option="colz";
  c_cylinder->Divide(2,3);
  c_cylinder->cd(1);
  h_out_bz->Draw(option);
  c_cylinder->cd(2);
  h_out_br->Draw(option);
  c_cylinder->cd(3);
  h_center_bz->Draw(option);
  c_cylinder->cd(4);
  h_center_br->Draw(option);
  c_cylinder->cd(5);
  h_in_bz->Draw(option);
  c_cylinder->cd(6);
  h_in_br->Draw(option);
}

void Draw::fill_cylinder(double r,TH2D* h_br,TH2D* h_bz)
{
  SetCylinderHist(h_br);
  SetCylinderHist(h_bz);
  int n_bin=h_br->GetYaxis()->GetNbins();
  double _z_w=(h_br->GetYaxis()->GetBinCenter(n_bin)-h_br->GetYaxis()->GetBinCenter(1))/2.;
  _z_w/=1.e3;//(mm)->(m)
  for(int i_z=0;i_z<n_z;i_z++)
    {
      for(int i_theta=0;i_theta<n_theta+1;i_theta++)
        {
          double theta=TMath::Pi()*(-1.+2.*(double)i_theta/(n_theta-1.));
          double x=r*cos(theta);
          double y=r*sin(theta);
          double z=_z_w*(-1.+2.*(double)i_z/(n_z-1.));
          TVector3 mfep(x,y,z);
          TVector3 b=Draw::B(mfep);
          z*=1.e3;//[m]->(mm)
          double br=b.Dot(TVector3(cos(theta),sin(theta),0.));
          theta/=TMath::Pi();//to draw with a unit of [rad/pi]
          h_br->Fill(theta,z,br);
          h_bz->Fill(theta,z,b.z());
        }
    }
  Draw::T_ppm(h_bz);
}

void Draw::xz(TH2D* h_xz_Br,TH2D* h_xz_Bz,TH2D* h_azi_Br,TH2D* h_azi_Bz)
{
  double z_draw=z_w*(.5+n_z)/(double)n_z*1.e2;
  double r_draw_min=(r_c-r_w*(.5+n_r)/(double)n_r)*1.e2;
  double r_draw_max=(r_c+r_w*(.5+n_r)/(double)n_r)*1.e2;
  h_xz_Br=new TH2D("h_xz_Br","Bx on xz plane[T];x(cm);z(cm)",
                   n_r,r_draw_min,r_draw_max,
                   n_z,-z_draw,z_draw);
  h_xz_Bz=new TH2D("h_xz_Bz","Bz on xz plane[T];x(cm);z(cm)",
                   n_r,r_draw_min,r_draw_max,
                   n_z,-z_draw,z_draw);
  h_azi_Br=new TH2D("h_azi_Br","azimuthally averaged Bx[T];r (cm);z (cm)",
                    n_r,r_draw_min,r_draw_max,
                    n_z,-z_draw,z_draw);
  h_azi_Bz=new TH2D("h_azi_Bz",
                    "(<B_{z}>_{#theta} - <B_{z}>_{#theta}^{ave}) / <B_{z}>_{#theta}^{ave} [ppm];r [cm];z [cm]",
                    n_r,r_draw_min,r_draw_max,
                    n_z,-z_draw,z_draw);
  //  Draw::fill_xz(h_xz_Br,h_xz_Bz);
  Draw::fill_azi(h_azi_Br,h_azi_Bz);
  const char* option="colz";
  c_xz->Divide(4,1);
  c_xz->cd(1);
  gPad->SetRightMargin(.24);
  gPad->SetBottomMargin(.14);
  h_xz_Br->Draw(option);
  c_xz->cd(2);
  gPad->SetRightMargin(.24);
  gPad->SetBottomMargin(.14);
  h_xz_Bz->Draw(option);
  c_xz->cd(3);
  gPad->SetRightMargin(.24);
  gPad->SetBottomMargin(.14);
  h_azi_Br->Draw(option);
  c_xz->cd(4);
  gPad->SetRightMargin(.24);
  gPad->SetBottomMargin(.14);
  h_azi_Bz->Draw(option);
  // delete h_xz_Br;
  // delete h_xz_Bz;
  // delete h_azi_Br;
  // delete h_azi_Bz;
}

void Draw::fill_xz(TH2D* h_br,TH2D* h_bz)
{
  SetStyle_TH2D(h_br);
  SetStyle_TH2D(h_bz);
  SetStyle_xz(h_br);
  SetStyle_xz(h_bz);
  for(int i_z=0;i_z<n_z;i_z++)
    {
      double z=z_w*(-1.+2.*i_z/(n_z-1.));
      for(int i_r=0;i_r<n_r;i_r++)
        {
          double x=r_c+r_w*(-1.+2.*i_r/(n_r-1.));
          TVector3 mfep(x,0.,z);
          auto b=Draw::B(mfep);
          h_br->Fill(x*1.e2,z*1.e2,b.x());
          h_bz->Fill(x*1.e2,z*1.e2,b.z());
        }
    }
}

void Draw::fill_azi(TH2D* h_br,TH2D* h_bz)
{
  SetStyle_TH2D(h_br);
  SetStyle_TH2D(h_bz);
  SetStyle_xz(h_br);
  SetStyle_xz(h_bz);
  for(int i_z=0;i_z<n_z;i_z++)
    {
      cout<<i_z<<"/"<<n_z<<endl;
      for(int i_r=0;i_r<n_r;i_r++)
        {
          double r=r_c+r_w*(-1.+2.*i_r/(n_r-1.));
          double z=z_w*(-1.+2.*i_z/(n_z-1.));          
          TVector3 b(0,0,0);
          for(int i_theta=0;i_theta<n_theta;i_theta++)
            {
              double theta=TMath::TwoPi()*(double)i_theta/n_theta;
              TVector3 mfep(r*cos(theta),r*sin(theta),z);
              b+=Draw::B(mfep);
            }
          b*=1./(double)n_theta;
          h_br->Fill(r*1.e2,z*1.e2,b.x());
          h_bz->Fill(r*1.e2,z*1.e2,b.z());
        }
    }
  Draw::T_ppm(h_bz);
  auto c=new TCanvas("c","c");
  h_bz->Draw("colz");
  auto f=new TFile("FinalAzimuthalBzDistribution.root","recreate");
  c->Write();
  f->Close();
  delete c;
  delete f;
}

TVector3 Draw::B(TVector3 mfep)
{
  if(sources=="cb")
    {
      return _cb.B(mfep);
    }
  if(sources=="ShimCoil")
    {
      return _ShimCoil.B(mfep)*1.e6;
    }
  if(sources=="magnet")
    {
      return _magnet.B(mfep);
    }
  if(sources=="magnetmms")
    {
      auto _B=_magnet.B(mfep);
      _B+=_mms.B(mfep);
      return _B;
    }
  if(sources=="all")
    {
      auto _B=_magnet.B(mfep)+_mms.B(mfep)+_ShimCoil.B(mfep);
      return _B;
    }
  if(sources=="mms")
    {
      auto _B=_mms.B(mfep);
      return _B;
    }
  puts("you cannot see this");
  return TVector3(nan(""),nan(""),nan(""));
}

void Draw::T_ppm(TH2D* h)
{
  int n_x=h->GetXaxis()->GetNbins();
  int n_y=h->GetYaxis()->GetNbins();
  vector<double> bz_list;
  ofstream of("AzimuthalBzDistribution.dat");
  for(int i_x=1;i_x<n_x+1;i_x++)  
    {
      for(int i_y=1;i_y<n_y+1;i_y++)
        {
          double bz=h->GetBinContent(i_x,i_y)-3.;
          bz_list.push_back(bz);
        }
    }
  double bz_ave=Average(bz_list)+3.;
  for(int i_x=1;i_x<n_x+1;i_x++)  
    {
      for(int i_y=1;i_y<n_y+1;i_y++)
        {
          double r=r_c+r_w*(-1.+2.*(i_x-1.)/(n_x-1.));
          double z=z_w*(-1.+2.*(i_y-1.)/(n_y-1.));
          double dif_T=h->GetBinContent(i_x,i_y)-bz_ave;
          double dif_ppm=1.e6*dif_T/bz_ave;
          h->SetBinContent(i_x,i_y,dif_ppm);
          of<<r<<"\t"<<z<<"\t"<<dif_ppm<<endl;
        }
    }
}










