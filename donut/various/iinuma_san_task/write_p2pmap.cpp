


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

void write_p2pmap(const char* ifn_dat="p2p_extraction.dat")
{

  ifstream ifn(ifn_dat);
  double r,z,p2p;
  string line;
  vector<double> p2p_vec;
  TH2D* h=new TH2D("h",
                   "p-p of Bz over rotational direction[Gauss];r[cm];z[cm]",
                   16,2.999,18.001,
                   69,-34.001,34.001);
  h->Draw();
  h->SetStats(0);
  for(int row=0;getline(ifn,line);row++)
    {
      istringstream stream(line);
      for(int col=0;stream>>r >>z >>p2p;col++)
        {
          p2p_vec.push_back(p2p);
          h->Fill(r,z,p2p);
        }
    }

  TCanvas* c=new TCanvas("c","c");
    h->SetContour(100);
  h->Draw("cont1z");
  TFile* f=new TFile("p2pmap.root","recreate");
  c->Write();
  f->Close();
  delete c,f;
  double min_p2p=*min_element(p2p_vec.begin(),p2p_vec.end());
  double max_p2p=*max_element(p2p_vec.begin(),p2p_vec.end());
  cout<<"from "<<min_p2p<<" to "<<max_p2p<<endl;
  
}
