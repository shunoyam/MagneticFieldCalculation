
#include"../../src/include.h"
using namespace std;

void study20221007()
{
  double min=0;
  double max=.5;
  int bin=100;
  int n_entry=1e4;
  
  string ifn_br="ShimCoilError_20221003.root";
  auto h_br=new TH1D("Br-type shim coil",
                     ";Field error due to manufacturing tolerance #muT_{p-p};Events",
                     bin,min,max);
  auto fin_br=new TFile(ifn_br.c_str());
  auto tr_br=(TTree*)fin_br->Get("tr");
  double p2p_Bx;
  tr_br->SetBranchAddress("p2p_Bx",&p2p_Bx);
  double maxr=0.;
  for(int i_entry=0;i_entry<n_entry;i_entry++)
    {
      tr_br->GetEntry(i_entry);
      double p2p_uT=p2p_Bx*1.e6;
      h_br->Fill(p2p_uT);
    }

  string ifn_bz="BzShimCoilError_20221006.root";
  auto h_bz=new TH1D("Bz-type shim coil",";Field error (#muT_{p-p});Events",bin,min,max);
  auto fin_bz=new TFile(ifn_bz.c_str());
  auto tr_bz=(TTree*)fin_bz->Get("tr");
  double p2p_Bz;
  tr_bz->SetBranchAddress("p2p_Bz",&p2p_Bz);
  for(int i_entry=0;i_entry<n_entry;i_entry++)
    {
      tr_bz->GetEntry(i_entry);
      double p2p_uT=p2p_Bz*1.e6;
      h_bz->Fill(p2p_uT);
      if(maxr<p2p_uT)
        {
          maxr=p2p_uT;
        }
    }
  cout<<"max_Br: "<<maxr<<endl;
  double s=.06;
  auto c=new TCanvas("c","");
  c->SetLogy();
  c->Draw();
  h_bz->Draw();
  h_bz->SetLineWidth(2);
  gPad->Update();
  auto st_bz=(TPaveStats*)h_bz->FindObject("stats");
  h_bz->SetStats(2);
  int blue=4;
  st_bz->SetLineColor(0);
  st_bz->SetTextColor(blue);

  h_br->SetLineColor(2);//red

  h_br->Draw("sames");
  h_br->SetLineWidth(2);
  gPad->Update();
  auto st_br=(TPaveStats*)h_br->FindObject("stats");
  h_br->SetStats(1);
  st_br->SetLineColor(0);
  st_br->SetTextColor(2);
  h_bz->SetLineColor(blue);
  h_bz->GetXaxis()->SetTitleSize(s);
  h_bz->GetYaxis()->SetTitleSize(s);
  h_bz->GetXaxis()->SetLabelSize(s);
  h_bz->GetYaxis()->SetLabelSize(s);
  h_bz->GetYaxis()->SetTitleOffset(.8);
  h_bz->GetXaxis()->SetNdivisions(505);
  h_bz->GetYaxis()->SetNdivisions(207);
  string ofn_root="analysis_20221007.root";
  auto f=new TFile(ofn_root.c_str(),"recreate");
  c->Write();
  f->Close();
  delete f;
  delete h_bz;
  delete h_br;
  delete fin_bz;
  delete fin_br;
}

void study20221007_wide()
{
  magnet m;
  m.InputTolerances_manual(.9e-3);
  int mode_min=50;
  int mode_max=150;

  int n_target=42;
  double target_min=2.99;
  double target_max=3.002;
  int n_shim=400;
  ofstream of("WideScan1204.dat");
  of<<fixed<<setprecision(15);
  for(int i_target=0;i_target<n_target;i_target++)
    {
      double target=target_min+(target_max-target_min)*(double)i_target/(n_target-1.);
      m.SetTargetField(target);
      for(int mode=mode_min;mode<mode_max;mode++)
        {
          cout<<i_target<<" th target, "<<mode<<" mode now"<<endl;
          auto shim=new mms();
          m.CostCalculation(shim,mode);
          eval probe(m,*shim);
          double U_lc=probe.U_lc();
          double TotalVolume=shim->TotalVolume();
          double rate_min=shim->count("low")/(double)n_shim;
          double rate_ok=shim->count("ok")/(double)n_shim;
          double rate_max=shim->count("high")/(double)n_shim;
          of<<target<<"\t"<<mode<<"\t"<<U_lc*1.e6<<"\t"<<TotalVolume*1.e6;
          of<<"\t"<<rate_min<<"\t"<<rate_ok<<"\t"<<rate_max<<endl;
        }
    }
}

void study20221009()
{
  double target0=3.;
  double target_div=10.e-6;//(T)
  int n_point=22;
  magnet m;
  m.InputTolerances_manual(.1e-3);
  int mode=65;
  ofstream of("20221009_done.dat");
  for(int i_point=0;i_point<n_point;i_point++)
    {
      dl();
      dl();
      dl();
      cout<<i_point<<"now"<<endl;
      double target=target0-i_point*target_div;
      m.SetTargetField(target);
      auto shim=new mms();
      m.CostCalculation(shim,mode);
      eval probe(m,*shim);
      double U=probe.U_lc();
      double V=shim->TotalVolume();
      of<<target-target0<<"\t"<<U*1.e6<<"\t"<<V*1.e6<<endl;
      delete shim;
    }
}

void EvaluationPoints()
{
  ofstream of("points.dat");
  int n_r=5;
  int n_theta=100;
  int n_z=10;
  for(int i_r=0;i_r<n_r;i_r++)
    {
      double r=33.3+1.5*(-1.+2.*(i_r/(n_r-1.)));
      for(int i_z=0;i_z<n_z;i_z++)
        {
          double z=5.*(-1.+2.*(i_z/(n_z-1.)));
          for(int i_theta=0;i_theta<n_theta;i_theta++)
            {
              double theta=TMath::Pi()*(-1.+2.*i_theta/n_theta);
              double x=r*cos(theta);
              double y=r*sin(theta);
              of<<x<<"\t"<<y<<"\t"<<z<<endl;
            }      
        } 
    }  
}

void DrawPockets()
{
  double dz=2.5;
  double dtheta=.05;
  double r=65;
  int n_z=20;
  int n_theta=20;
  int i_pocket=0;
  for(int i_z=0;i_z<n_z;i_z++)
    {
      for(int i_theta=0;i_theta<n_theta;i_theta++)
        {
          double z=140.*(-.5+i_z/(n_z-1.));
          double theta=TMath::TwoPi()*(-.5+(double)i_theta/n_theta);
          string ofn="pocket"+to_string(i_pocket)+".dat";
          ofstream of(ofn.c_str());
          of<<r*cos(theta+dtheta)<<"\t"<<r*sin(theta+dtheta)<<"\t"<<z+dz<<endl;
          of<<r*cos(theta+dtheta)<<"\t"<<r*sin(theta+dtheta)<<"\t"<<z-dz<<endl;
          of<<r*cos(theta-dtheta)<<"\t"<<r*sin(theta-dtheta)<<"\t"<<z-dz<<endl;
          of<<r*cos(theta-dtheta)<<"\t"<<r*sin(theta-dtheta)<<"\t"<<z+dz<<endl;
          of<<r*cos(theta+dtheta)<<"\t"<<r*sin(theta+dtheta)<<"\t"<<z+dz<<endl;
          i_pocket++;
        }
    }
}

void BarelyAchieved()
{
  vector<double> sigma_z{10.,2.,2.};//cm
  double width_z=10.;//cm
  double U=.4;//ppm/20 mm (p-p)
  for(int i=0;i<3;i++)
    {
      double alpha=width_z/sigma_z[i];
      double k=sqrt(2./TMath::Pi())*alpha*exp(-alpha*alpha/2.)/erff(alpha/sqrt(2));
      double e=U/alpha*sqrt(1.-k);
      cout<<e<<endl;
    }  
}

void DeformationStudy20230105()
{
  double dev=.1e-3;
  vector<int> i_cb_vec{1,3,5};
  for(int i_cb:i_cb_vec)
    {
      magnet m;
      m.OvalDeformation(i_cb,dev,0.,4);
      eval p(m);
      double U_lc=p.U_lc();
      cout<<i_cb<<" th coil: U_lc: "<<U_lc*1.e6<<" ppm"<<endl;
    }
}

void ManyFields()
{
  int n_col=2;
  int n_row=6;
  int n_trial=n_col*n_row;
  auto random=new TRandom2(3);
  auto c=new TCanvas("c","c",800,800);
  c->Divide(n_col,n_row);
  c->Draw();
  double level=.1e-3;
  for(int i_trial=0;i_trial<n_trial;i_trial++)
    {
      magnet m;
      m.InputRandomTolerances(random);
      m.DrawBzOnCylinder(c,i_trial);
    }
  auto f=new TFile("ManyFields.root","recreate");
  c->Write();
  f->Close();
  delete c;
  delete f;
  delete random;
}

void TargetFieldScanRangeDetermination()
{
  int bin=100;
  double min=3.-1.e-3;
  double max=3.+1.e-3;
  auto h=new TH1D("h",";;",bin,min,max);
  Bs Fields("donut");
  magnet m;
  m.InputTolerances_manual(1.5e-3);
  Fields.add_coil((vector<cb>)m);
  for(int i_point=0;i_point<Fields.MFEPs();i_point++)
    {
      double bz=Fields.at(i_point).Bz();
      h->Fill(bz);
    }
  auto c=new TCanvas("c");
  c->Draw();
  h->Draw();
  auto f=new TFile("TargetFieldScanRangeDetermination.root","recreate");
  c->Write();
  h->Write();
  f->Close();  
}






