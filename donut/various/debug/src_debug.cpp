

#include"../../src/include.h"

using namespace std;

void arc_cat(int n_arc,double r,double z_top,double z_bottom,double I,TVector3 mfep,
             TH2D* h,double range,int mesh)
{
  SetStyle_TH2D(h);
  shim_coil* shims[n_arc];
  TH2D* hs[n_arc];
  double CentralAngle=TMath::TwoPi()/n_arc;
  double bz;
  for(int i_arc=0;i_arc<n_arc;i_arc++)
    {
      string name="Bz by "+to_string(i_arc)+"th arc[Gauss]";
      hs[i_arc]=new TH2D(name.c_str(),"Bz by 6 arc[Gauss];x[m];y[m]",
                         mesh,-range,range,
                         mesh,-range,range);
      SetStyle_TH2D(hs[i_arc]);
      double center_angle=TMath::TwoPi()*i_arc/n_arc;
      shims[i_arc]=new shim_coil(r,z_top,z_bottom,center_angle,CentralAngle,I);
      TVector3 ResponseElement=shims[i_arc]->ResponseElement_arc(mfep,0.);
      bz+=ResponseElement.z();
    }
  bz*=I;
  cout<<n_arc<<" arc: "<<bz*1.e4<<"Gauss"<<endl;
  for(int i=0;i<mesh;i++)
    {
      double x=range*(-1+2.*i/(mesh-1.));
      for(int j=0;j<mesh;j++)
        {
          double y=range*(-1+2.*j/(mesh-1.));
          TVector3 mfep(x,y,1.);
          double bz_sum=0.;
          for(int i_arc=0;i_arc<n_arc;i_arc++)
            {
              double bz_part=shims[i_arc]->ResponseElement_arc(mfep,0.).z();
              bz_sum+=bz_part;
              hs[i_arc]->Fill(x,y,bz_part*1.e4);
            }
          h->Fill(x,y,bz_sum*1.e4);
        }
    }
  auto c=new TCanvas("c","c");
  c->Divide(3,2);
  SetStyle_TCanvas(c);
  for(int i_arc=0;i_arc<n_arc;i_arc++)
    {
      c->cd(i_arc+1);
      ArrangeRange(hs[i_arc]);
      hs[i_arc]->Draw("colz");
      delete[] shims[i_arc];
    }
  auto f=new TFile("6arc.root","recreate");
  c->Write();
  f->Close();
  delete c;
  delete f;
  ArrangeRange(h);
}

void difference_arc_flc(TH2D* h_dif,TH2D* h_flc,TH2D* h_arc)
{
  SetStyle_TH2D(h_dif);
  h_dif->Add(h_flc,h_arc,1,-1);
  ArrangeRange(h_dif);
}





