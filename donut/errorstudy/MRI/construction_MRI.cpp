

#include"../../src/include.h"
using namespace std;

static double Bz_MRI(vector<cb> MRI,TVector3 mfep)
{
  double Bz=0.;
  for(int i_coil=0;i_coil<MRI.size();i_coil++)
    {
      Bz+=MRI[i_coil].B(mfep).z();
    }
  return Bz;
}

void seeU_MRI()
{


  vector<double> CurrentDensity_vec
    {
     1.2359846e8,
     1.2359846e8,
     1.2364308e8,
     1.2364308e8,
     1.2353705e8,
     1.2353705e8,
     1.2347602e8,
     1.2347602e8     
    };

  vector<double> center_r_vec
    {
     0.8880868131,
     0.8880868131,
     0.5440526848,
     0.5440526848,
     0.5264199526,
     0.5264199526,
     0.5269742914,
     0.5269742914
    };

  vector<double> width_r_vec
    {
     0.0504,
     0.0504,
     0.0828,
     0.0828,
     0.0504,
     0.0504,      
     0.0396,
     0.0396
    };//full width

  vector<double> center_z_vec
    {
     0.4859,
     -0.4859,
     0.6476123386,
     -0.6476123386,
     0.2986783101,
     -0.2986783101,
     0.09242538088,
     -0.09242538088
    };

  vector<double> width_z_vec
    {      
     0.2212,
     0.2212,
     0.224,
     0.224, 
     0.1428,
     0.1428,
     0.1344,
     0.1344
    };//full width

  for(int i=0;i<width_z_vec.size();i++)
    {
      //      width_z_vec[i]/=2.;
      width_r_vec[i]/=2.;
    }
    
  vector<int> nodes_r_vec
    {
      5,5,5,5,5,5,5,5
        };

  vector<int> nodes_z_vec
    {
     10,10,10,10,7,7,8,8
    };

  
  int n_coil=8;
  vector<cb> MRI_gauss;
  vector<cb> MRI_mesh;
  for(int i_coil=0;i_coil<n_coil;i_coil++)
    {
      cb coil_gauss;
      cb coil_mesh;
      cout<<i_coil<<"coil"<<center_z_vec[i_coil]<<endl;
      coil_gauss.SetOnGaussNodes(center_r_vec[i_coil],
                                 width_r_vec[i_coil]/2.,
                                 nodes_r_vec[i_coil],
                                 center_z_vec[i_coil],
                                 width_z_vec[i_coil]/2.,
                                 nodes_z_vec[i_coil],
                                 CurrentDensity_vec[i_coil]);
      double r_in=center_r_vec[i_coil]-width_r_vec[i_coil]/2.;
      double r_out=center_r_vec[i_coil]+width_r_vec[i_coil]/2.;
      double height=width_z_vec[i_coil];
      double current=width_r_vec[i_coil]*width_z_vec[i_coil]*CurrentDensity_vec[i_coil];
      coil_mesh.SetOnMesh(r_in,r_out,height,100,100,current,
                          center_z_vec[i_coil]);
      MRI_gauss.push_back(coil_gauss);
      MRI_mesh.push_back(coil_mesh);
    }
   
  double Bz_mesh=0.,Bz_gauss=0.;
  for(int i=0;i<n_coil;i++)
    {
      Bz_gauss+=MRI_gauss[i].B(TVector3(0.,0.,0.)).z();
      Bz_mesh+=MRI_mesh[i].B(TVector3(0.,0.,0.)).z();
    }
  cout<<"Bz at the center of MRI (gauss nodes):"<<Bz_gauss<<"[T]"<<endl;
  cout<<"Bz at the center of MRI (100*100mesh):"<<Bz_mesh<<"[T]"<<endl;

  
  ////////////debug/////////////////////////////////
  ofstream t("test.dat");
  ////////////debug/////////////////////////////////


  double r,z,theta;
  double radius=0.25;
  int n_theta=100;
  vector<double> Bz_vec;
  MRI_gauss[2].dis(TVector3(2.e-3,1.e-4,0.));
  
  for(int i_theta=0;i_theta<n_theta;i_theta++)
    {
      theta=TMath::Pi()*i_theta/n_theta;
      r=radius*sin(theta);
      z=radius*cos(theta);
      TVector3 mfep(r,0.,z);
      double Bz=Bz_MRI(MRI_gauss,mfep);
      Bz_vec.push_back(Bz);
      t<<Bz<<endl;
    }
  double ave_50DSS=Average(Bz_vec);
  double p2p_50DSS=P2P(Bz_vec);
  cout<<"average on 50[cm] DSS:"<<ave_50DSS<<"[T]"<<endl;
  cout<<"p-p on 50[cm] DSS:"<<p2p_50DSS<<"[T]"<<endl;
  
  
}


static void Fill_Bz(TH2D* hist,cb coil,double range,int n)
{
  double mfep_x,mfep_z;
  SetStyle_TH2D(hist);
  vector<double> IG;//information gathering
  for(int i_x=0;i_x<n;i_x++)
    {
      mfep_x=(-1.+2.*(double)(i_x/(n-1.)))*range;
      for(int i_z=0;i_z<n;i_z++)
        {
          mfep_z=(-1.+2.*(double)(i_z/(n-1.)))*range;
          TVector3 mfep(mfep_x,0.,mfep_z);
          double bz=coil.B(mfep).z();
          IG.push_back(bz*1.e4);
          hist->Fill(mfep_x*100,mfep_z*100,bz*1.e4);
        }
    }
  double min=*min_element(IG.begin(),IG.end());
  double max=*max_element(IG.begin(),IG.end());
  hist->GetZaxis()->SetRangeUser(min,max);
}

void check_GaussNode()
{
  cb coil_gauss,coil_mesh;
  double center_r=1.;
  double width_r=.1;
  int nodes_r=8;
  int nodes_z=8;
  double center_z=0.;
  double width_z=.1;
  double CurrentDensity=1.e4;
  int mesh=100;
  coil_gauss.SetOnGaussNodes(center_r,width_r,nodes_r,
                             center_z,width_z,nodes_z,CurrentDensity);
  coil_mesh.SetOnMesh(center_r-width_r,center_r+width_r,
                      2.*width_z,
                      mesh,mesh,
                      CurrentDensity*2.*width_z*2.*width_r,center_z);
  double range=.2;
  int n=100;
  auto h_gauss=new TH2D("h_gauss","Bz(8*8 gauss node)[Gauss];radial[cm];axial[cm]",
                        n,-range*100.,range*100.,
                        n,-range*100.,range*100.);
  auto h_mesh=new TH2D("h_mesh","Bz(100*100 mesh)[Gauss];radial[cm];axial[cm]",
                       n,-range*100.,range*100.,
                       n,-range*100.,range*100.);
  
  Fill_Bz(h_mesh,coil_mesh,range,n);
  Fill_Bz(h_gauss,coil_gauss,range,n);

  auto c=new TCanvas("c","c");
  c->Divide(2,2);
  c->cd(1);
  h_mesh->Draw("colz");
  c->cd(2);
  h_gauss->Draw("colz");
  auto f=new TFile("Mesh_Gauss.root","recreate");
  c->Write();
  f->Close();
  delete f;
  delete c;
  delete h_mesh;
  delete h_gauss;

}

void GaussStudy()
{
  double center_r=1.;
  double width_r=.1;
  double center_z=0.;
  double width_z=.1;
  double CurrentDensity=1.e4;
  const char* ofn_gauss="GaussStudy.dat";
  int mesh;
  ofstream gauss(ofn_gauss);
  TVector3 origin(0.,0.,0.);
  double bz,bz_5,bz_8,bz_10;
  auto* coil_gauss=new cb();
  int nodes=5;
  coil_gauss->SetOnGaussNodes(center_r,width_r,nodes,
                              center_z,width_z,nodes,CurrentDensity);
  bz_5=coil_gauss->B(origin).z();  
  delete coil_gauss;
  nodes=8;
  cb cb8;
  ofstream g8("gauss.dat");
  cb8.SetOnGaussNodes(center_r,width_r,nodes,
                      center_z,width_z,nodes,CurrentDensity);
  for(coil flc: cb8.coil_vec())
    {
      g8<<flc.r()<<"\t"<<flc.z()<<endl;
    }
  bz_8=cb8.B(origin).z();  
  nodes=10;
  cb cb10;
  cb10.SetOnGaussNodes(center_r,width_r,nodes,
                       center_z,width_z,nodes,CurrentDensity);
  bz_10=cb10.B(origin).z();  
  int n=30;
  for(int i=0;i<n;i++)
    {
      cout<<i<<"now"<<endl;
      mesh=pow(1.5,i);
      cout<<i<<"now"<<endl;
      cout<<"number of meshes:"<<mesh<<endl;
      cb coil_mesh;
      coil_mesh.SetOnMesh(center_r-width_r,center_r+width_r,
                          2.*width_z,
                          mesh,mesh,
                          CurrentDensity*2.*width_z*2.*width_r,center_z);
      bz=coil_mesh.B(origin).z();
      gauss<<fixed<<setprecision(20);
      gauss<<mesh<<"\t"<<bz_5*1.e4<<"\t"<<bz_8*1.e4<<"\t"<<bz_10*1.e4<<"\t"<<bz*1.e4<<endl;
    }
}



  




