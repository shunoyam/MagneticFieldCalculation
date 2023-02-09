
#ifndef _IN_ANALYTIC
#define _IN_ANALYTIC
#include"../../src/include.h"

using namespace std;

void analytic()
{
  vector<double> sensitivity_u_radi{0.0848642,0.088858,0.0888587,0.207994,0.207994,0.110518};
  vector<double> sensitivity_u_axia{31.8409,74.3809,74.4211,52.1859,52.877,112.069};
  vector<double> sensitivity_u_tilt{0.0851903,0.145708,0.145708,0.147446,0.147446,0.0726915};
  TRandom2* random=new TRandom2();
  double r,z,theta;
  vector<double> r_vec,z_vec,theta_vec;
  double tolerance=0.5;
  int n_trial=100000;
  double uniformity;
  TTree* tr=new TTree("tr","");
  TH1D* h=new TH1D("h",
                   "uniformly random tolerance analysis;uniformity[ppm];entries",
                   300,0.,200);
  tr->Branch("uniformity",&uniformity);
  /*
  TPaveStats* st_h=(TPaveStats*)h->FindObject("stats");
  h->SetStats(0);
  */
  
  for(int i_trial=0;i_trial<n_trial;i_trial++)
    {
      r_vec.clear();
      z_vec.clear();
      theta_vec.clear();
      for(int i_cb=0;i_cb<6;i_cb++)
        {
      z=random->Uniform(0.,tolerance);
      r=random->Uniform(0.,tolerance);
      theta=random->Uniform(0.,tolerance);
      r_vec.push_back(r);
      z_vec.push_back(z);
      theta_vec.push_back(theta);
        }
      uniformity=dots(r_vec,sensitivity_u_radi,
                      z_vec,sensitivity_u_axia,
                      theta_vec,sensitivity_u_tilt);
      h->Fill(uniformity);
      tr->Fill();
    }

  TFile* f=new TFile("analytic.root","recreate");
  tr->Write();
  h->Write();
  f->Close();
  delete f;
  delete tr;  
}

double dot(vector<double> vec1,vector<double> vec2)
{
  double value=0.;
  for(int i=0;i<vec1.size();i++)
    {
      value+=vec1[i]*vec2[i];
    }
  return value;
}

double dots(vector<double> vec1,vector<double> vec2,
            vector<double> vec3,vector<double> vec4,
            vector<double> vec5,vector<double> vec6)
{
 double sum=dot(vec1,vec2);
 sum+=dot(vec3,vec4);
 sum+=dot(vec5,vec6);
 return sum;
}








#endif
