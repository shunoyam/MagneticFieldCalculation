
#include"../../../src/include.h"
using namespace std;


void HigherCorrection(string ofn_root,int i_mode,string NormalOrSkew)
{
  int n_trial=1e4;
  string type="Br";
  ShimCoil sc(type);
  double TargetAmplitude=1.;//[T]
  sc.MakeModes(i_mode,NormalOrSkew,TargetAmplitude,"svd");
  auto random=new TRandom2();
  //input values
  vector<double> theta_tilt_vec;
  vector<double> phi_tilt_vec;
  vector<double> psi_tilt_vec;
  vector<double> r_shift_vec;
  vector<double> z_shift_vec;
  vector<double> theta_shift_vec;
  
  //output values
  double min_Bx,ave_Bx,max_Bx,p2p_Bx;
  double min_By,ave_By,max_By,p2p_By;
  double min_Bz,ave_Bz,max_Bz,p2p_Bz;
  double min_Br,ave_Br,max_Br,p2p_Br;

  //
  auto tr_ref=new TTree("tr_ref","");
  tr_ref->Branch("min_Bx",&min_Bx);
  tr_ref->Branch("ave_Bx",&ave_Bx);
  tr_ref->Branch("max_Bx",&max_Bx);
  tr_ref->Branch("p2p_Bx",&p2p_Bx);

  tr_ref->Branch("min_By",&min_By);
  tr_ref->Branch("ave_By",&ave_By);
  tr_ref->Branch("max_By",&max_By);
  tr_ref->Branch("p2p_By",&p2p_By);

  tr_ref->Branch("min_Bz",&min_Bz);
  tr_ref->Branch("ave_Bz",&ave_Bz);
  tr_ref->Branch("max_Bz",&max_Bz);
  tr_ref->Branch("p2p_Bz",&p2p_Bz);

  tr_ref->Branch("min_Br",&min_Br);
  tr_ref->Branch("ave_Br",&ave_Br);
  tr_ref->Branch("max_Br",&max_Br);
  tr_ref->Branch("p2p_Br",&p2p_Br);
  eval probe_ref(sc);
  min_Bz=probe_ref.min_Bz();
  ave_Bz=probe_ref.ave_Bz();
  max_Bz=probe_ref.max_Bz();
  p2p_Bz=probe_ref.p2p_Bz();

  min_Bx=probe_ref.min_Bx();
  ave_Bx=probe_ref.ave_Bx();
  max_Bx=probe_ref.max_Bx();
  p2p_Bx=probe_ref.p2p_Bx();
      
  min_By=probe_ref.min_By();
  ave_By=probe_ref.ave_By();
  max_By=probe_ref.max_By();
  p2p_By=probe_ref.p2p_By();

  min_Br=probe_ref.min_Br();
  ave_Br=probe_ref.ave_Br();
  max_Br=probe_ref.max_Br();
  p2p_Br=probe_ref.p2p_Br();

  tr_ref->Fill();
  //
  
  auto tr=new TTree("tr","");
  tr->Branch("theta_tilt_vec",&theta_tilt_vec);
  tr->Branch("phi_tilt_vec",&phi_tilt_vec);
  tr->Branch("psi_tilt_vec",&psi_tilt_vec);
  tr->Branch("r_shift_vec",&r_shift_vec);
  tr->Branch("z_shift_vec",&z_shift_vec);
  tr->Branch("theta_shift_vec",&theta_shift_vec);

  tr->Branch("min_Bx",&min_Bx);
  tr->Branch("ave_Bx",&ave_Bx);
  tr->Branch("max_Bx",&max_Bx);
  tr->Branch("p2p_Bx",&p2p_Bx);
  
  tr->Branch("min_By",&min_By);
  tr->Branch("ave_By",&ave_By);
  tr->Branch("max_By",&max_By);
  tr->Branch("p2p_By",&p2p_By);

  tr->Branch("min_Bz",&min_Bz);
  tr->Branch("ave_Bz",&ave_Bz);
  tr->Branch("max_Bz",&max_Bz);
  tr->Branch("p2p_Bz",&p2p_Bz);
  
  tr->Branch("min_Br",&min_Br);
  tr->Branch("ave_Br",&ave_Br);
  tr->Branch("max_Br",&max_Br);
  tr->Branch("p2p_Br",&p2p_Br);

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
      min_Br=probe.min_Br();
      ave_Br=probe.ave_Br();
      max_Br=probe.max_Br();
      p2p_Br=probe.p2p_Br();
      
      tr->Fill();
      sc.Perfect();
    }
  auto OutputFile=new TFile(ofn_root.c_str(),"recreate");
  tr_ref->Write();
  tr->Write();
  OutputFile->Close();
  delete tr;
  delete tr_ref;
  delete OutputFile;
  delete random;
}

void analysis_HigherCorrection(string ifn_root)
{
  auto fin=new TFile(ifn_root.c_str());
  auto tr=(TTree*)fin->Get("tr");
  auto tr_ref=(TTree*)fin->Get("tr_ref");
  string ofn_root="ana_"+ifn_root;
  auto f=new TFile(ofn_root.c_str(),"recreate");
  ExtractShimCoilInformation(tr,"x",tr_ref);
  ExtractShimCoilInformation(tr,"y",tr_ref);
  ExtractShimCoilInformation(tr,"z",tr_ref);
  ExtractShimCoilInformation(tr,"r",tr_ref);
  f->Close();
  delete f;
  delete fin;
}

void ExtractShimCoilInformation(TTree* tr,string element,TTree* tr_ref)
{
  double min,ave,max,p2p;
  double min_ref,ave_ref,max_ref,p2p_ref;

  string min_address="min_B"+element;
  tr->SetBranchAddress(min_address.c_str(),&min);
  tr_ref->SetBranchAddress(min_address.c_str(),&min_ref);

  string ave_address="ave_B"+element;
  tr->SetBranchAddress(ave_address.c_str(),&ave);
  tr_ref->SetBranchAddress(ave_address.c_str(),&ave_ref);

  string max_address="max_B"+element;
  tr->SetBranchAddress(max_address.c_str(),&max);
  tr_ref->SetBranchAddress(max_address.c_str(),&max_ref);

  string p2p_address="p2p_B"+element;
  tr->SetBranchAddress(p2p_address.c_str(),&p2p);
  tr_ref->SetBranchAddress(p2p_address.c_str(),&p2p_ref);

  vector<double> min_vec,ave_vec,max_vec,p2p_vec;
  int nentry=tr->GetEntries();
  //loop to determine max and min
  for (int ientry=0;ientry<nentry;ientry++)
    {
      tr->GetEntry(ientry);
      min_vec.push_back(min);
      ave_vec.push_back(ave);
      max_vec.push_back(max);
      p2p_vec.push_back(p2p);
    }
  double min_min_hist=*min_element(min_vec.begin(),min_vec.end());
  double max_min_hist=*max_element(min_vec.begin(),min_vec.end());
  double min_ave_hist=*min_element(ave_vec.begin(),ave_vec.end());
  double max_ave_hist=*max_element(ave_vec.begin(),ave_vec.end());
  double min_max_hist=*min_element(max_vec.begin(),max_vec.end());
  double max_max_hist=*max_element(max_vec.begin(),max_vec.end());
  double min_p2p_hist=*min_element(p2p_vec.begin(),p2p_vec.end());
  double max_p2p_hist=*max_element(p2p_vec.begin(),p2p_vec.end());

  double buffer=.3;

  string title_min="h_min_"+element;
  string label_min=";B_{"+element+"}^{min} [T];entries [a.u.]";
  auto h_min=new TH1D(title_min.c_str(),label_min.c_str(),100,
                      (1.+buffer)*min_min_hist-buffer*max_min_hist,
                      (1+buffer)*max_min_hist-buffer*min_min_hist);
  string title_ave="h_ave_"+element;
  string label_ave=";B_{"+element+"}^{ave} [T];entries [a.u.]";
  auto h_ave=new TH1D(title_ave.c_str(),label_ave.c_str(),100,
                      (1.+buffer)*min_ave_hist-buffer*max_ave_hist,
                      (1+buffer)*max_ave_hist-buffer*min_ave_hist);

  string title_max="h_max_"+element;
  string label_max=";B_{"+element+"}^{max} [T];entries [a.u.]";
  auto h_max=new TH1D(title_max.c_str(),label_max.c_str(),100,
                      (1.+buffer)*min_max_hist-buffer*max_max_hist,
                      (1+buffer)*max_max_hist-buffer*min_max_hist);
                      
  string title_p2p="h_p2p_"+element;
  string label_p2p=";B_{"+element+"}^{max} [T];entries [a.u.]";
  auto h_p2p=new TH1D(title_p2p.c_str(),label_p2p.c_str(),100,
                      (1.+buffer)*min_p2p_hist-buffer*max_p2p_hist,
                      (1+buffer)*max_p2p_hist-buffer*min_p2p_hist);

  for (int ientry=0;ientry<nentry;ientry++)
    {
      h_min->Fill(min_vec[ientry]);
      h_ave->Fill(ave_vec[ientry]);
      h_max->Fill(max_vec[ientry]);
      h_p2p->Fill(p2p_vec[ientry]);
    }

  tr_ref->GetEntry(0);


  string title_c="c_"+element;
  auto c=new TCanvas(title_c.c_str(),title_c.c_str());
  c->Divide(2,2);
  c->cd(1);
  h_min->Draw();
  gPad->Update();
  auto arrow_min=new TArrow(min_ref,0,min_ref,gPad->GetUymax());
  arrow_min->SetLineWidth(3);
  arrow_min->SetLineColor(2);
  arrow_min->Draw("same");
  c->cd(2);
  h_ave->Draw();
  gPad->Update();
  auto arrow_ave=new TArrow(ave_ref,0,ave_ref,gPad->GetUymax());
  arrow_ave->SetLineWidth(3);
  arrow_ave->SetLineColor(2);
  arrow_ave->Draw("same");
  c->cd(3);
  h_max->Draw();
  gPad->Update();
  auto arrow_max=new TArrow(max_ref,0,max_ref,gPad->GetUymax());
  arrow_max->SetLineWidth(3);
  arrow_max->SetLineColor(2);
  arrow_max->Draw("same");
  c->cd(4);
  h_p2p->Draw();
  gPad->Update();
  auto arrow_p2p=new TArrow(p2p_ref,0,p2p_ref,gPad->GetUymax());
  arrow_p2p->SetLineWidth(3);
  arrow_p2p->SetLineColor(2);
  arrow_p2p->Draw("same");
  c->Write();
  delete h_min;
  delete h_ave;
  delete h_max;
  delete h_p2p;
  delete c;
  delete arrow_min;
  delete arrow_ave;
  delete arrow_max;
  delete arrow_p2p;
  
}

void IntegratedShimCoilStudy()
{
  for(int i_type=0;i_type<2;i_type++)
    {
      string BeafOrChicken="skew";
      if(i_type)
        {
          BeafOrChicken="normal";
        }
      for(int i_mode=0;i_mode<4;i_mode++)
        {
          string ofn_root=to_string(i_mode)+BeafOrChicken+".root";
          HigherCorrection(ofn_root,i_mode,BeafOrChicken);
          analysis_HigherCorrection(ofn_root);
        }
    }

}







