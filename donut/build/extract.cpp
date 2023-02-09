

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
#include<string.h>

using namespace std;

void extract()
{
  TFile f("after.root","read");
  auto c=(TCanvas*)f.Get("cy");
  c->Draw();
  c->cd(3);
  auto h=(TH2D*)c->GetPrimitive("h_center_bz");
  h->Draw("colz");
  // auto of=new TFile("after-ASC.root","recreate");
  // auto c2=new TCanvas("c");
  // f.Close();
  // c2->Draw();
  // h->Draw();
  // c2->Write();
  // of->Close();









  
}
