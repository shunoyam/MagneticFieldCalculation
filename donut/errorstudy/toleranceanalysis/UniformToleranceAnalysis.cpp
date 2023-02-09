
#ifndef _IN_UNIFORMTOLERANCEANALYSIS
#define _IN_UNIFORMTOLERANCEANALYSIS
#include"../../src/include.h"
using namespace std;

void uniform_tolerance_generator(const char* filename_root)
{
  //1
  vector<cb> MainCoilsAndYoke;
  ReadCBsData(MainCoilsAndYoke);
  vector<double> theta_vec,phi_vec;
  vector<TVector3> dis_vec;
  TVector3 dis_yoke,dis_MC10,dis_MC11,dis_MC20,dis_MC21,dis_MC30; 
  TTree* uniform_tolerance=new TTree("uni","");
  uniform_tolerance->Branch("theta_vec",&theta_vec);
  uniform_tolerance->Branch("phi_vec",&phi_vec);
  uniform_tolerance->Branch("dis_MC10",&dis_MC10);
  uniform_tolerance->Branch("dis_MC11",&dis_MC11);
  uniform_tolerance->Branch("dis_MC20",&dis_MC20);
  uniform_tolerance->Branch("dis_MC21",&dis_MC21);
  uniform_tolerance->Branch("dis_MC30",&dis_MC30);
  uniform_tolerance->Branch("dis_yoke",&dis_yoke);
  TRandom2* random=new TRandom2();
  int n_trial=1000;
  ofstream hurry("hurry.dat");
  vector<double> data_Bz,data_Br;
  double p2p_Bz,p2p_Br;
  double uniformity;
  double min_Bz,max_Bz,min_Br,max_Br;
  double shift_axial;  
  uniform_tolerance->Branch("p2p_Bz",&p2p_Bz);
  /*  uniform_tolerance->Branch("p2p_Br",&p2p_Br);
  uniform_tolerance->Branch("min_Bz",&min_Bz);
  uniform_tolerance->Branch("max_Bz",&max_Bz);
  uniform_tolerance->Branch("min_Br",&min_Br);
  uniform_tolerance->Branch("max_Br",&max_Br);*/
  uniform_tolerance->Branch("uniformity",&uniformity);
  uniform_tolerance->Branch("shift_axial",&shift_axial);
 
  for(int i_trial=0;i_trial<n_trial;i_trial++)
    {
      cout<<i_trial<<"trials now"<<endl;
      //3
      puts("step 3");
      //      theta_vec=get_theta_vec(random);
      //      phi_vec=get_phi_vec(random);
      //      dis_vec=get_dis_vec(random);
      shift_axial=random->Uniform(-tolerance_axial_MC10,tolerance_axial_MC10);
      TVector3 dis_MC10(0.,0.,shift_axial);
      MainCoilsAndYoke[1].dis(dis_MC10);
      /*      dis_yoke=dis_vec[0];
      dis_MC10=dis_vec[1];
      dis_MC11=dis_vec[2];
      dis_MC20=dis_vec[3];
      dis_MC21=dis_vec[4];
      dis_MC30=dis_vec[5];
      InputTolerance(MainCoilsAndYoke,theta_vec,phi_vec,dis_vec);
      */
      //4
      puts("step 4");
      data_Bz=Fiducial(MainCoilsAndYoke,"z");
      uniformity=data_Bz[0];
      //changed!
      p2p_Bz=data_Bz[5];
      min_Bz=data_Bz[3];
      max_Bz=data_Bz[4];
      /*      data_Br=Fiducial(MainCoilsAndYoke,"r");
      p2p_Br=data_Br[2]-data_Br[1];
      min_Br=data_Br[3];
      max_Br=data_Br[4];
      */
      hurry<<shift_axial<<"\t"<<p2p_Bz*1.e6 <<endl;
      uniform_tolerance->Fill();
    }

  TFile* f=new TFile(filename_root,"recreate");
  uniform_tolerance->Write();
  f->Close();
  delete f;
  delete random;  
}


void simpleanalysis(const char* ifn_dat)
{
  ifstream ifn(ifn_dat);
  double AxialShift;//mm
  double p2p;//uT
  string line;
  TH1D* h1=new TH1D("h1","uniform check;axial shift of MC10 [mm];counts [a.u]",
                    100,-0.6,0.6);
  TH1D* h2=new TH1D("h2",
                    "uniform check;p-p of azimuthally averaged Bz [uT];counts [a.u]",
                    100,-10.,130.);
  TH2D* h3=new TH2D("h3",
                    "correlation;axial shift of MC10 [mm];p-p of azimuthally averaged Bz [uT]",
                    100,-0.6,0.6,
                    100,-10.,130.);
  
  for(int row=0;getline(ifn,line);row++)
    {
      istringstream stream(line);
      for(int col=0;stream>>AxialShift >>p2p ;col++)
        {
          AxialShift*=1.e3;
          h1->Fill(AxialShift);
          h2->Fill(p2p);
          h3->Fill(AxialShift,p2p);
        }
    }
  TCanvas* c=new TCanvas("c","c");
  c->Divide(2,2);
  c->cd(1);
  h1->Draw();
  c->cd(2);
  h2->Draw();
  c->cd(3);
  h3->Draw("scatter");

  TFile* file=new TFile("SimpleAnalysis.root","recreate");
  c->Write();
  file->Close();
  delete c;
  delete file;

  
}











#endif
