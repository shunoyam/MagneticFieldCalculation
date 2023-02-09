
#include"../../../src/include.h"
using namespace std;

void ShimCoilError(string type,string ofn_root)
{
  int n_trial=1e4;
  ShimCoil sc(type);
  double current_ref=1.e3;//[A]
  sc.ApplySameCurrent(current_ref);
  //  sc.MakeBx(current_ref);
  auto random=new TRandom2();
  //input values
  vector<double> theta_tilt_vec;
  vector<double> phi_tilt_vec;
  vector<double> psi_tilt_vec;
  vector<double> r_shift_vec;
  vector<double> z_shift_vec;
  vector<double> theta_shift_vec;
  
  //output values
  double min_Bz;
  double ave_Bz;
  double max_Bz;
  double p2p_Bz;
  double min_Br;
  double ave_Br;
  double max_Br;
  double p2p_Br;

  double min_Bx;
  double ave_Bx;
  double max_Bx;
  double p2p_Bx;
  
  auto tr=new TTree("tr","");
  tr->Branch("theta_tilt_vec",&theta_tilt_vec);
  tr->Branch("phi_tilt_vec",&phi_tilt_vec);
  tr->Branch("psi_tilt_vec",&psi_tilt_vec);
  tr->Branch("r_shift_vec",&r_shift_vec);
  tr->Branch("z_shift_vec",&z_shift_vec);
  tr->Branch("theta_shift_vec",&theta_shift_vec);

  tr->Branch("min_Bz",&min_Bz);
  tr->Branch("ave_Bz",&ave_Bz);
  tr->Branch("max_Bz",&max_Bz);
  tr->Branch("p2p_Bz",&p2p_Bz);

  tr->Branch("min_Br",&min_Br);
  tr->Branch("ave_Br",&ave_Br);
  tr->Branch("max_Br",&max_Br);
  tr->Branch("p2p_Br",&p2p_Br);
  
  tr->Branch("min_Bx",&min_Bx);
  tr->Branch("ave_Bx",&ave_Bx);
  tr->Branch("max_Bx",&max_Bx);
  tr->Branch("p2p_Bx",&p2p_Bx);
  
  for(int i_trial=0;i_trial<n_trial;i_trial++)
    {
      if(i_trial%10==0)
        {
          cout<<i_trial<<"/"<<n_trial<<" trials now"<<endl;
        }
      T_ShimCoil t(random);
      phi_tilt_vec=t.phi_tilts();
      psi_tilt_vec=t.psi_tilts();
      theta_tilt_vec=t.theta_tilts();
      z_shift_vec=t.z_shifts();
      theta_shift_vec=t.theta_shifts();
      sc.InputTolerances(t);
      eval probe(sc);
      min_Bz=probe.min_Bz();
      ave_Bz=probe.ave_Bz();
      max_Bz=probe.max_Bz();
      p2p_Bz=probe.p2p_Bz();

      min_Br=probe.min_Br();
      ave_Br=probe.ave_Br();
      max_Br=probe.max_Br();
      p2p_Br=probe.p2p_Br();

      min_Bx=probe.min_Br();
      ave_Bx=probe.ave_Br();
      max_Bx=probe.max_Br();
      p2p_Bx=probe.p2p_Br();
      tr->Fill();
      sc.Perfect();
    }
  auto OutputFile=new TFile(ofn_root.c_str(),"recreate");
  tr->Write();
  OutputFile->Close();
  delete tr;
  delete OutputFile;
}

void BxError(string ofn_root)
{
  int n_trial=1e4;
  string type="Br";
  ShimCoil sc(type);
  double current_ref=1.e3;//[A]
  sc.MakeModes(2,"normal",current_ref,"simple");
  auto random=new TRandom2();
  //input values
  vector<double> theta_tilt_vec;
  vector<double> phi_tilt_vec;
  vector<double> psi_tilt_vec;
  vector<double> r_shift_vec;
  vector<double> z_shift_vec;
  vector<double> theta_shift_vec;
  
  //output values
  double min_Bz;
  double ave_Bz;
  double max_Bz;
  double p2p_Bz;

  double min_Bx;
  double ave_Bx;
  double max_Bx;
  double p2p_Bx;
  
  double min_By;
  double ave_By;
  double max_By;
  double p2p_By;
  
  auto tr=new TTree("tr","");
  tr->Branch("theta_tilt_vec",&theta_tilt_vec);
  tr->Branch("phi_tilt_vec",&phi_tilt_vec);
  tr->Branch("psi_tilt_vec",&psi_tilt_vec);
  tr->Branch("r_shift_vec",&r_shift_vec);
  tr->Branch("z_shift_vec",&z_shift_vec);
  tr->Branch("theta_shift_vec",&theta_shift_vec);

  tr->Branch("min_Bz",&min_Bz);
  tr->Branch("ave_Bz",&ave_Bz);
  tr->Branch("max_Bz",&max_Bz);
  tr->Branch("p2p_Bz",&p2p_Bz);
  
  tr->Branch("min_Bx",&min_Bx);
  tr->Branch("ave_Bx",&ave_Bx);
  tr->Branch("max_Bx",&max_Bx);
  tr->Branch("p2p_Bx",&p2p_Bx);
  
  tr->Branch("min_By",&min_By);
  tr->Branch("ave_By",&ave_By);
  tr->Branch("max_By",&max_By);
  tr->Branch("p2p_By",&p2p_By);
  
  for(int i_trial=0;i_trial<n_trial;i_trial++)
    {
      if(i_trial%10==0)
        {
          cout<<i_trial<<"/"<<n_trial<<" trials now"<<endl;
        }
      T_ShimCoil t(random);
      phi_tilt_vec=t.phi_tilts();
      psi_tilt_vec=t.psi_tilts();
      theta_tilt_vec=t.theta_tilts();
      z_shift_vec=t.z_shifts();
      theta_shift_vec=t.theta_shifts();
      sc.InputTolerances(t);
      eval probe(sc);
      min_Bz=probe.min_Bz();
      ave_Bz=probe.ave_Bz();
      max_Bz=probe.max_Bz();
      p2p_Bz=probe.p2p_Bz();

      min_Bx=probe.min_Bx();
      ave_Bx=probe.ave_Bx();
      max_Bx=probe.max_Bx();
      p2p_Bx=probe.p2p_Bx();
      
      min_By=probe.min_By();
      ave_By=probe.ave_By();
      max_By=probe.max_By();
      p2p_By=probe.p2p_By();
      
      tr->Fill();
      sc.Perfect();
    }
  auto OutputFile=new TFile(ofn_root.c_str(),"recreate");
  tr->Write();
  OutputFile->Close();
  delete tr;
  delete OutputFile;
}

void Analysis_ShimCoilError(string ifn_root)
{
  auto fin=new TFile(ifn_root.c_str(),"read");
  auto tr=(TTree*)fin->Get("tr");
  double min_Bz,ave_Bz,max_Bz,p2p_Bz;
  double min_Br,ave_Br,max_Br,p2p_Br;
  double min_Bx,ave_Bx,max_Bx,p2p_Bx;
  
  tr->SetBranchAddress("min_Bz",&min_Bz);
  tr->SetBranchAddress("ave_Bz",&ave_Bz);
  tr->SetBranchAddress("max_Bz",&max_Bz);
  tr->SetBranchAddress("p2p_Bz",&p2p_Bz);
  tr->SetBranchAddress("min_Br",&min_Br);
  tr->SetBranchAddress("ave_Br",&ave_Br);
  tr->SetBranchAddress("max_Br",&max_Br);
  tr->SetBranchAddress("p2p_Br",&p2p_Br);
  tr->SetBranchAddress("min_Bx",&min_Bx);
  tr->SetBranchAddress("ave_Bx",&ave_Bx);
  tr->SetBranchAddress("max_Bx",&max_Bx);
  tr->SetBranchAddress("p2p_Bx",&p2p_Bx);

  vector<double> min_Bz_vec,ave_Bz_vec,max_Bz_vec,p2p_Bz_vec;
  vector<double> min_Br_vec,ave_Br_vec,max_Br_vec,p2p_Br_vec;
  vector<double> min_Bx_vec,ave_Bx_vec,max_Bx_vec,p2p_Bx_vec;

  int nentry=tr->GetEntries();

  //loop to determine max and min
  for (int ientry=0;ientry<nentry;ientry++)
    {
      tr->GetEntry(ientry);
      min_Bz_vec.push_back(min_Bz);
      ave_Bz_vec.push_back(ave_Bz);
      max_Bz_vec.push_back(max_Bz);
      p2p_Bz_vec.push_back(p2p_Bz);
      min_Br_vec.push_back(min_Br);
      ave_Br_vec.push_back(ave_Br);
      max_Br_vec.push_back(max_Br);
      p2p_Br_vec.push_back(p2p_Br);
      min_Bx_vec.push_back(min_Bx);
      ave_Bx_vec.push_back(ave_Bx);
      max_Bx_vec.push_back(max_Bx);
      p2p_Bx_vec.push_back(p2p_Bx);
    }
  double min_Bz_hist=*min_element(min_Bz_vec.begin(),min_Bz_vec.end());
  double max_Bz_hist=*max_element(max_Bz_vec.begin(),max_Bz_vec.end());
  double min_Bz_p2p_hist=*min_element(p2p_Bz_vec.begin(),p2p_Bz_vec.end());
  double max_Bz_p2p_hist=*max_element(p2p_Bz_vec.begin(),p2p_Bz_vec.end());

  double min_Br_hist=*min_element(min_Br_vec.begin(),min_Br_vec.end());
  double max_Br_hist=*max_element(max_Br_vec.begin(),max_Br_vec.end());
  double min_Br_p2p_hist=*min_element(p2p_Br_vec.begin(),p2p_Br_vec.end());
  double max_Br_p2p_hist=*max_element(p2p_Br_vec.begin(),p2p_Br_vec.end());

  double min_Bx_hist=*min_element(min_Bx_vec.begin(),min_Bx_vec.end());
  double max_Bx_hist=*max_element(max_Bx_vec.begin(),max_Bx_vec.end());
  double min_Bx_p2p_hist=*min_element(p2p_Bx_vec.begin(),p2p_Bx_vec.end());
  double max_Bx_p2p_hist=*max_element(p2p_Bx_vec.begin(),p2p_Bx_vec.end());

  double offset_Bz;
  auto h_max_Bz=new TH1D("h_max_Bz",
                         "Bz_{max}[T];;entries[a.u.]",
                         100,min_Bz_hist-1.e-6,max_Bz_hist+1.e-6);
  h_max_Bz->SetLineColor(39);
  auto h_min_Bz=new TH1D("h_min_Bz",
                         "Bz_{min}[T];;entries[a.u.]",
                         100,min_Bz_hist-1.e-6,max_Bz_hist+1.e-6);
  h_min_Bz->SetLineColor(46);
  auto h_p2p_Bz=new TH1D("h_p2p_Bz","Bz_{p2p}[T];;entries[a.u.]",
                         100,min_Bz_p2p_hist-1.e-6,max_Bz_p2p_hist+1.e-6);
  auto h_ave_Bz=new TH1D("h_ave_Bz",
                         "B_{z}^{min,ave,max}[T];;entries[a.u.]",
                         100,min_Bz_hist-1.e-6,max_Bz_hist+1.e-6);
  h_ave_Bz->SetLineColor(8);

  auto h_max_Br=new TH1D("h_max_Br",
                         "Br_{max}[T];;entries[a.u.]",
                         100,min_Br_hist-1.e-6,max_Br_hist+1.e-6);
  h_max_Br->SetLineColor(39);
  auto h_min_Br=new TH1D("h_min_Br",
                         "Br_{min}[T];;entries[a.u.]",
                         100,min_Br_hist-1.e-6,max_Br_hist+1.e-6);
  h_min_Br->SetLineColor(46);
  auto h_p2p_Br=new TH1D("h_p2p_Br","Br_{p2p}[T];;entries[a.u.]",
                         100,min_Br_p2p_hist-1.e-6,max_Br_p2p_hist+1.e-6);
  auto h_ave_Br=new TH1D("h_ave_Br",
                         "B_{r}^{min,ave,max}[T];;entries[a.u.]",
                         100,min_Br_hist-1.e-6,max_Br_hist+1.e-6);
  h_ave_Br->SetLineColor(8);
  
  auto h_min_Bx=new TH1D("h_min_Bx",
                         "B_{r}^{min,ave,max}[T];;entries[a.u.]"
                         ,100,min_Bx_hist-1.e-5,max_Bx_hist+1.e-4);
  h_min_Bx->SetLineColor(46);
  auto h_ave_Bx=new TH1D("h_ave_Bx",
                         "B_{r}^{min,ave,max}[T];;entries[a.u.]"
                         ,100,min_Bx_hist-1.e-6,max_Bx_hist+1.e-6);
  h_ave_Bx->SetLineColor(8);
  auto h_max_Bx=new TH1D("h_max_Bx",
                         "B_{r}^{min,ave,max}[T];;entries[a.u.]"
                         ,100,min_Bx_hist-1.e-6,max_Bx_hist+1.e-6);
  h_max_Bx->SetLineColor(39);
  auto h_p2p_Bx=new TH1D("h_p2p_Bx","Bx_{p2p}[T];;entries[a.u.]",
                         100,min_Bx_p2p_hist-1.e-6,max_Bx_p2p_hist+1.e-6);
  
  for (int ientry=0;ientry<nentry;ientry++)
    {
      tr->GetEntry(ientry);
      h_min_Bz->Fill(min_Bz);
      h_ave_Bz->Fill(ave_Bz);
      h_max_Bz->Fill(max_Bz);
      h_p2p_Bz->Fill(p2p_Bz);

      h_min_Br->Fill(min_Br);
      h_ave_Br->Fill(ave_Br);
      h_max_Br->Fill(max_Br);
      h_p2p_Br->Fill(p2p_Br);

      h_min_Bx->Fill(min_Bx);
      h_ave_Bx->Fill(ave_Bx);
      h_max_Bx->Fill(max_Bx);
      h_p2p_Bx->Fill(p2p_Bx);
    }
  SetStyle_TH1Ds(h_min_Bz,h_ave_Bz,h_max_Bz,h_p2p_Bz);
  SetStyle_TH1Ds(h_min_Br,h_ave_Br,h_max_Br,h_p2p_Br);
  SetStyle_TH1Ds(h_min_Bx,h_ave_Bx,h_max_Bx,h_p2p_Bx);

  auto c=new TCanvas("c","c");
  c->Divide(2,2);
  SetStyle_TCanvas(c);
  c->cd(1);
  auto st_min=(TPaveStats*)h_min_Bz->FindObject("stats");
  auto st_ave=(TPaveStats*)h_ave_Bz->FindObject("stats");
  auto st_max=(TPaveStats*)h_max_Bz->FindObject("stats");
  st_min->SetTextColor(46);
  st_ave->SetTextColor(8);
  st_max->SetTextColor(39);
  c->Modified();
  h_ave_Bz->Draw();
  h_max_Bz->Draw("sames");
  h_min_Bz->Draw("sames");
  c->cd(2);
  st_min=(TPaveStats*)h_min_Br->FindObject("stats");
  st_ave=(TPaveStats*)h_ave_Br->FindObject("stats");
  st_max=(TPaveStats*)h_max_Br->FindObject("stats");
  st_min->SetTextColor(46);
  st_ave->SetTextColor(8);
  st_max->SetTextColor(39);
  c->Modified();
  h_ave_Br->Draw();
  h_max_Br->Draw("sames");
  h_min_Br->Draw("sames");
  c->cd(3);
  h_p2p_Bz->Draw();
  c->cd(4);
  h_p2p_Br->Draw();

  auto c2=new TCanvas("c2","c2");
  SetStyle_TCanvas(c2);
  c2->Divide(2,2);
  c2->cd(1);
  st_min=(TPaveStats*)h_min_Bx->FindObject("stats");
  st_ave=(TPaveStats*)h_ave_Bx->FindObject("stats");
  st_max=(TPaveStats*)h_max_Bx->FindObject("stats");
  st_min->SetTextColor(46);
  st_ave->SetTextColor(8);
  st_max->SetTextColor(39);
  c2->Modified();
  h_ave_Bx->Draw();
  h_max_Bx->Draw("sames");
  h_min_Bx->Draw("sames");
  c2->cd(2);
  h_p2p_Bx->Draw();
  
  string ofn_root="ana_"+ifn_root;
  auto RootFile=new TFile(ofn_root.c_str(),"recreate");
  c->Write();
  c2->Write();
  RootFile->Close();

  delete RootFile;
  delete c;
  delete h_min_Bz;
  delete h_ave_Bz;
  delete h_max_Bz;
  delete h_min_Br;
  delete h_ave_Br;
  delete h_max_Br;
  delete h_min_Bx;
  delete h_ave_Bx;
  delete h_max_Bx;
  delete fin;
  delete c2;
}

void Analysis_BxError(string ifn_root)
{
  auto fin=new TFile(ifn_root.c_str());
  auto tr=(TTree*)fin->Get("tr");
  double min_Bz,ave_Bz,max_Bz,p2p_Bz;
  double min_Bx,ave_Bx,max_Bx,p2p_Bx;
  double min_By,ave_By,max_By,p2p_By;
  
  tr->SetBranchAddress("min_Bz",&min_Bz);
  tr->SetBranchAddress("ave_Bz",&ave_Bz);
  tr->SetBranchAddress("max_Bz",&max_Bz);
  tr->SetBranchAddress("p2p_Bz",&p2p_Bz);

  tr->SetBranchAddress("min_Bx",&min_Bx);
  tr->SetBranchAddress("ave_Bx",&ave_Bx);
  tr->SetBranchAddress("max_Bx",&max_Bx);
  tr->SetBranchAddress("p2p_Bx",&p2p_Bx);
  
  tr->SetBranchAddress("min_By",&min_By);
  tr->SetBranchAddress("ave_By",&ave_By);
  tr->SetBranchAddress("max_By",&max_By);
  tr->SetBranchAddress("p2p_By",&p2p_By);

  vector<double> min_Bz_vec,ave_Bz_vec,max_Bz_vec,p2p_Bz_vec;
  vector<double> min_Bx_vec,ave_Bx_vec,max_Bx_vec,p2p_Bx_vec;
  vector<double> min_By_vec,ave_By_vec,max_By_vec,p2p_By_vec;

  int nentry=tr->GetEntries();
  double factor=1.e6 /3.;

  //loop to determine max and min
  for (int ientry=0;ientry<nentry;ientry++)
    {
      tr->GetEntry(ientry);
      min_Bz_vec.push_back(min_Bz*factor);
      ave_Bz_vec.push_back(ave_Bz*factor);
      max_Bz_vec.push_back(max_Bz*factor);
      p2p_Bz_vec.push_back(p2p_Bz*factor);

      min_Bx_vec.push_back(min_Bx);
      ave_Bx_vec.push_back(ave_Bx);
      max_Bx_vec.push_back(max_Bx);
      p2p_Bx_vec.push_back(p2p_Bx);

      min_By_vec.push_back(min_By);
      ave_By_vec.push_back(ave_By);
      max_By_vec.push_back(max_By);
      p2p_By_vec.push_back(p2p_By);
    }
  double min_Bz_hist=*min_element(min_Bz_vec.begin(),min_Bz_vec.end());
  double max_Bz_hist=*max_element(max_Bz_vec.begin(),max_Bz_vec.end());
  double min_Bz_p2p_hist=*min_element(p2p_Bz_vec.begin(),p2p_Bz_vec.end());
  double max_Bz_p2p_hist=*max_element(p2p_Bz_vec.begin(),p2p_Bz_vec.end());

  double min_Bx_hist=*min_element(min_Bx_vec.begin(),min_Bx_vec.end());
  double max_Bx_hist=*max_element(max_Bx_vec.begin(),max_Bx_vec.end());
  double min_Bx_p2p_hist=*min_element(p2p_Bx_vec.begin(),p2p_Bx_vec.end());
  double max_Bx_p2p_hist=*max_element(p2p_Bx_vec.begin(),p2p_Bx_vec.end());

  double min_By_hist=*min_element(min_By_vec.begin(),min_By_vec.end());
  double max_By_hist=*max_element(max_By_vec.begin(),max_By_vec.end());
  double min_By_p2p_hist=*min_element(p2p_By_vec.begin(),p2p_By_vec.end());
  double max_By_p2p_hist=*max_element(p2p_By_vec.begin(),p2p_By_vec.end());

  double offset_Bz;
  auto h_max_Bz=new TH1D("h_max_Bz",
                         "B_{z}^{max}[T];;entries[a.u.]",
                         100,min_Bz_hist-.5,max_Bz_hist+.5);
  h_max_Bz->SetLineColor(39);
  auto h_min_Bz=new TH1D("h_min_Bz",
                         "B_{z}^{min}[T];;entries[a.u.]",
                         100,min_Bz_hist-.5,max_Bz_hist+.5);
  h_min_Bz->SetLineColor(46);
  auto h_p2p_Bz=new TH1D("h_p2p_Bz","Bz_{p2p}[T];;entries[a.u.]",
                         100,min_Bz_p2p_hist-1.e-6,max_Bz_p2p_hist+1.e-6);
  auto h_ave_Bz=new TH1D("h_ave_Bz",
                         "B_{z}^{min,ave,max}[T];;entries[a.u.]",
                         100,min_Bz_hist-.5,max_Bz_hist+.5);
  h_ave_Bz->SetLineColor(8);

  auto h_min_By=new TH1D("h_min_By",
                         "B_{y}^{min,ave,max}[T];;entries[a.u.]"
                         //                         ,100,.708e-3,.712e-3);
                         ,100,.709e-3,.711e-3);
  h_min_By->SetLineColor(46);
  auto h_ave_By=new TH1D("h_ave_By",
                         "B_{y}^{min,ave,max}[T];;entries[a.u.]"
                         ,100,.708e-3,.76e-3);
  h_ave_By->SetLineColor(8);
  auto h_max_By=new TH1D("h_max_By",
                         "B_{y}^{min,ave,max}[T];;entries[a.u.]"
                         ,100,.757e-3,.759e-3);
  h_max_By->SetLineColor(39);
  auto h_p2p_By=new TH1D("h_p2p_By","By_{p2p}[T];;entries[a.u.]",
                         100,min_By_p2p_hist-1.e-6,max_By_p2p_hist+1.e-6);
  double range=2.e-6;
  range=0.;
  auto h_cor_By=new TH2D("h_cor_By",";;",
                         100,.709e-3,.711e-3,100,.757e-3,.759e-3);
    
  for (int ientry=0;ientry<nentry;ientry++)
    {
      tr->GetEntry(ientry);
      h_min_Bz->Fill(min_Bz*factor);
      h_ave_Bz->Fill(ave_Bz*factor);
      h_max_Bz->Fill(max_Bz*factor);
      h_p2p_Bz->Fill(p2p_Bz);

      h_min_By->Fill(min_By);
      h_ave_By->Fill(ave_By);
      h_max_By->Fill(max_By);
      h_p2p_By->Fill(p2p_By);
      h_cor_By->Fill(min_By,max_By);
    }
  SetStyle_TH1Ds(h_min_Bz,h_ave_Bz,h_max_Bz,h_p2p_Bz);
  SetStyle_TH1Ds(h_min_By,h_ave_By,h_max_By,h_p2p_By);

  auto c_div=new TCanvas("c_div","c_div");
  c_div->Divide(2,2);
  SetStyle_TCanvas(c_div);
  c_div->cd(1);
  h_min_By->Draw();
  c_div->cd(2);
  h_ave_By->Draw();
  c_div->cd(3);
  h_max_By->Draw();
  c_div->cd(4);
  h_cor_By->Draw();
  double cf=h_cor_By->GetCorrelationFactor();
  cout<<cf<<endl;
  auto c=new TCanvas("c","c");  
  c->Divide(2,2);
  SetStyle_TCanvas(c);
  c->cd(1);
  auto st_min=(TPaveStats*)h_min_Bz->FindObject("stats");
  auto st_ave=(TPaveStats*)h_ave_Bz->FindObject("stats");
  auto st_max=(TPaveStats*)h_max_Bz->FindObject("stats");
  st_min->SetTextColor(46);
  st_ave->SetTextColor(8);
  st_max->SetTextColor(39);
  c->Modified();
  h_ave_Bz->Draw();
  h_max_Bz->Draw("sames");
  h_min_Bz->Draw("sames");
  c->cd(2);
  st_min=(TPaveStats*)h_min_By->FindObject("stats");
  st_ave=(TPaveStats*)h_ave_By->FindObject("stats");
  st_max=(TPaveStats*)h_max_By->FindObject("stats");
  st_min->SetTextColor(46);
  st_ave->SetTextColor(8);
  st_max->SetTextColor(39);
  c->Modified();
  h_min_By->Draw();
  h_ave_By->Draw("sames");
  h_max_By->Draw("sames");
  c->cd(3);
  h_p2p_Bz->Draw();
  c->cd(4);
  h_p2p_By->Draw();
  
  string ofn_root="ana_"+ifn_root;
  auto RootFile=new TFile(ofn_root.c_str(),"recreate");
  c->Write();
  c_div->Write();
  RootFile->Close();
  delete RootFile;
  delete c;
  delete c_div;
  delete h_cor_By;
  delete h_min_Bz;
  delete h_ave_Bz;
  delete h_max_Bz;
  delete h_min_By;
  delete h_ave_By;
  delete h_max_By;
  delete fin;
}




