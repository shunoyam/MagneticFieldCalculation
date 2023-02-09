
#include"../../src/include.h"
using namespace std;

 void enough_size_of_pocket()
 {
   mms ironshims;
   ironshims.read("shim_20210922.dat");
   TVector3 pos(0.333,0.,0.);
   double bz;
   ofstream f("sizevsbz.dat");
   double v;
   int n=100;
   for(int i=0;i<n;i++)
     {
       v=i*0.06*1.e-6;
       for(int i_shim=0;i_shim<((vector<mm>)ironshims).size();i_shim++)
         {
           ironshims.set_volume(i_shim,v);
         }
       //  cout<<ironshims.mm_vec()[3].volume()<<endl;
       bz=ironshims.B(pos).z();
       f<<v*1.e6<<"\t"<<bz<<endl;
     }
 }

void IterationAnalysis(const char* inputfile_root,const char* outputfile_root)
{
  gStyle->SetTitleXSize(0.07);
  gStyle->SetTitleYSize(0.07);
  gStyle->SetLabelSize(0.07,"x");
  gStyle->SetLabelSize(0.07,"y");
  gStyle->SetTitleSize(0.07,"");
  gStyle->SetMarkerStyle(8);
  gStyle->SetMarkerSize(0.5);    
  TCanvas* c=new TCanvas("c","c");
  c->Divide(2,3);
  c->cd(1);
  c->Draw();
  TFile* fin=new TFile(inputfile_root,"read");
  TTree* itr=(TTree*)fin->Get("itr");
  int N=itr->GetEntries();
  int i_iteration;
  double B_RE_min,B_RE_max,rate_OK,rate_lower,rate_upper,pocket_use_rate,TotalVolume;
  double peaktopeak;
  vector<double> *delta_ironshims_vec=0;
  vector<double> *ironshims_vec=0;
  vector<double> *ironshims_before_vec=0;
  vector<double> *B_TG_vec=0,*B_RE_vec=0;
  itr->SetBranchAddress("i_iteration",&i_iteration);
  itr->SetBranchAddress("B_RE_min",&B_RE_min);
  itr->SetBranchAddress("B_RE_max",&B_RE_max);
  itr->SetBranchAddress("rate_OK",&rate_OK);
  itr->SetBranchAddress("rate_lower",&rate_lower);
  itr->SetBranchAddress("rate_upper",&rate_upper);
  itr->SetBranchAddress("pocket_use_rate",&pocket_use_rate);
  itr->SetBranchAddress("TotalVolume",&TotalVolume);
  itr->SetBranchAddress("peaktopeak",&peaktopeak);
  itr->SetBranchAddress("B_TG_vec",&B_TG_vec);
  itr->SetBranchAddress("delta_ironshims_vec",&delta_ironshims_vec);
  itr->SetBranchAddress("ironshims_vec",&ironshims_vec);
  itr->SetBranchAddress("B_RE_vec",&B_RE_vec);
  itr->SetBranchAddress("ironshims_before_vec",&ironshims_before_vec);
  //  itr->SetBranchAddress("",&);
  //  itr->SetBranchAddress("",&);
  //  itr->SetBranchAddress("",&);
  //  itr->SetBranchAddress("",&);
  //  itr->SetBranchAddress("",&);
  //  itr->SetBranchAddress("",&);
  TGraph *g1_OK=new TGraph();
  g1_OK->Draw();
  TGraph *g1_low=new TGraph();
  TGraph *g1_up=new TGraph();
  TGraph* g2=new TGraph();
  TGraph *g3=new TGraph();
  TGraph* g4=new TGraph();
  TGraph* g5=new TGraph();
  TGraph* g6_after=new TGraph(); 
  TGraph* g6_before=new TGraph();
  TGraph* g7=new TGraph();
 
  for (int ientry=0;ientry<N;ientry++)
    {
      itr->GetEntry(ientry);
      g1_OK->SetPoint(ientry,i_iteration,rate_OK);
      g1_low->SetPoint(ientry,i_iteration,rate_lower);
      g1_up->SetPoint(ientry,i_iteration,rate_upper);
      g2->SetPoint(ientry,i_iteration,peaktopeak*1.e6);
      g7->SetPoint(ientry,i_iteration,TotalVolume*1.e6);
    
      if(ientry==0)
        {
          for(int i_shim=0;i_shim<delta_ironshims_vec->size();i_shim++)
            {
              g3->SetPoint(g3->GetN(),g3->GetN(),delta_ironshims_vec->at(i_shim));
              g6_before->SetPoint(g6_before->GetN(),g6_before->GetN(),
                                  ironshims_before_vec->at(i_shim));
              g6_after->SetPoint(g6_after->GetN(),
                                 g6_after->GetN(),ironshims_vec->at(i_shim));
            }
          for(int i_MFEP=0;i_MFEP<B_TG_vec->size();i_MFEP++)
            {
              g4->SetPoint(g4->GetN(),g4->GetN(),B_TG_vec->at(i_MFEP));
              g5->SetPoint(g5->GetN(),g5->GetN(),B_RE_vec->at(i_MFEP));
            }
        }
    }
  g1_OK->GetYaxis()->SetRangeUser(0.,1.);
  g1_OK->SetTitle("ratio of 3 group about volume of iron piece;iteration;ratio(red over,black OK, blue under)");
  const char* option_draw="l";
  g1_OK->Draw(option_draw);
  g1_low->SetLineColor(4);//blue
  g1_low->Draw("l same");
  g1_up->SetLineColor(2);//red
  g1_up->Draw("l same");
  
  c->cd(2);
  g2->Draw();
  g2->SetTitle("change of peak to peak;iteration;p-p(uT)");
  g2->Draw(option_draw);
  
  c->cd(3);
  g7->Draw();
  g7->Draw(option_draw);
  g7->SetTitle("total volume of iron shim;iteration;volume[cc]");
  

  
  c->cd(6);

  TFile* of=new TFile(outputfile_root,"recreate");
  c->Write();
  of->Close();
  delete c;    
}

void DistributionAnalysis(const char* inputfile_root,const char* outputfile_root,int ientry)
{
  gStyle->SetTitleXSize(0.07);
  gStyle->SetTitleYSize(0.07);
  gStyle->SetLabelSize(0.07,"x");
  gStyle->SetLabelSize(0.07,"y");
  gStyle->SetTitleSize(0.07,"");
  gStyle->SetMarkerStyle(8);
  gStyle->SetMarkerSize(0.5);    
  TCanvas* c=new TCanvas("c","c");
  c->Divide(2,3);
  c->cd(1);
  c->Draw();
  TFile* fin=new TFile(inputfile_root,"read");
  TTree* itr=(TTree*)fin->Get("itr");
  int N=itr->GetEntries();
    if(N<=ientry)
      {
        cout<<ientry<<"th data is invalid."<<endl;
        cout<<"Iteration was done "<<N<<"times"<<endl;
      }
  vector<double> *delta_ironshims_vec=0;
  vector<double> *ironshims_vec=0;
  vector<double> *ironshims_before_vec=0;
  vector<double> *B_TG_vec=0,*B_RE_vec=0,*B_MM_vec=0;
  itr->SetBranchAddress("B_MM_vec",&B_MM_vec);
  itr->SetBranchAddress("B_RE_vec",&B_RE_vec);
  itr->SetBranchAddress("B_TG_vec",&B_TG_vec);
  itr->SetBranchAddress("delta_ironshims_vec",&delta_ironshims_vec);
  itr->SetBranchAddress("ironshims_vec",&ironshims_vec);
  itr->SetBranchAddress("ironshims_before_vec",&ironshims_before_vec);
  //  itr->SetBranchAddress("",&);
  //  itr->SetBranchAddress("",&);
  //  itr->SetBranchAddress("",&);
  //  itr->SetBranchAddress("",&);
  //  itr->SetBranchAddress("",&);
  //  itr->SetBranchAddress("",&);
  TGraph* g1=new TGraph();
  TGraph* g2=new TGraph();
  TGraph* g3_before=new TGraph();
  TGraph* g3_after=new TGraph();
  TGraph* g4=new TGraph();
  TGraph* g5=new TGraph();
  TGraph* g6=new TGraph();

  itr->GetEntry(ientry);
  for(int i_shim=0;i_shim<delta_ironshims_vec->size();i_shim++)
    {
      g1->SetPoint(g1->GetN(),g1->GetN(),delta_ironshims_vec->at(i_shim)*1.e6);
      g3_before->SetPoint(g3_before->GetN(),g3_before->GetN(),
                          ironshims_before_vec->at(i_shim));
      g3_after->SetPoint(g3_after->GetN(),g3_after->GetN(),ironshims_vec->at(i_shim));
    }
  for(int i_MFEP=0;i_MFEP<B_TG_vec->size();i_MFEP++)
    {
      g2->SetPoint(g2->GetN(),g2->GetN(),B_MM_vec->at(i_MFEP));
      g4->SetPoint(g4->GetN(),g4->GetN(),B_TG_vec->at(i_MFEP));
      g5->SetPoint(g5->GetN(),g5->GetN(),B_RE_vec->at(i_MFEP));
    }
  c->cd(1);
  g1->Draw();
  g1->Draw("ap");
  g1->SetTitle("set of iron shims;# of iron shim [a.u];ideal amount of change of iron [cc]");

  
  c->cd(2);
  g2->Draw();
  g2->Draw("ap");
  g2->SetTitle("contribution of iron shims;# of MFEP;B_MM[T]");

  c->cd(3);
  g3_before->SetTitle("array of volume_shim_0;# of shim;volume[m3]");
  g3_before->SetMarkerColor(4);
  g3_before->Draw("ap");

  g3_after->Draw("same");
  g3_after->SetMarkerColor(2);

  c->cd(4);
  g4->Draw("ap");
  g4->SetTitle("set of target field;# of MFEP;residual Bz[T]");

  c->cd(5);
  g5->Draw("ap");
  g5->SetTitle("set of residual field;# of MFEP;residual Bz[T]");

  //  c->cd(5);

  TFile* of=new TFile(outputfile_root,"recreate");
  string title_pdf=to_string(ientry)+"iteration.pdf";
  c->SaveAs(title_pdf.c_str());
  c->Write();
  of->Close();
  delete c;
  delete of;
}


void study1()
{

  for(int i=0;i<10;i++)
    {
      DistributionAnalysis("outfile.root","tmp.root",i);
    }
}








