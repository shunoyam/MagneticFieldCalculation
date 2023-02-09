
#ifndef _IN_COILMESH
#define _IN_COILMESH
#include"../../src/include.h"
using namespace std;

void coilmesh()
{
  ofstream f("CoilMeshStudy.dat");

  Bs B;
  const char* path_to_MFEP_configfile="../config/alongR12.dat";
  B.read(path_to_MFEP_configfile);
  B.clear();
  double r_in=.46;
  double r_out=.4808;
  double height=.54;
  int mesh0_r=1,mesh0_z=30;
  int mesh_r,mesh_z;
  int n_point=30;
  double Bz_min,Bz_ave,Bz_max;
  double deviation,Bz_ave_before;
  double TotalCurrent_main=(24.04-23)*(27-(-27))*88.7986069450757;
  double TotalCurrent_weak=(25.14-24.1)*(4.0427878-(-4.0427878))*-68.397846473723;
  
  ofstream dev("deviation.dat");
  
  for(int i_point=0;i_point<n_point;i_point++)
    {
      cout<<"now "<<i_point<<endl;
      B.clear();
      cb coil;
      mesh_r=mesh0_r*i_point;
      mesh_z=mesh0_z*i_point;
      coil.SetOnMesh(r_in,r_out,height,mesh_r,mesh_z,TotalCurrent_main);
      B.add_coil(coil);
      Bz_min=B.min('z');
      Bz_max=B.max('z');
      Bz_ave=B.average();
      deviation=Bz_ave-Bz_ave_before;
      Bz_ave_before=Bz_ave;
      dev<<i_point<<"\t"<<deviation<<endl;
      f<<fixed<<setprecision(15)<<i_point<<"\t"<<Bz_min<<"\t"<<Bz_ave<<"\t"<<Bz_max<<endl;
    }  
}

int id_z(double z,int point_z,double height)
{
  double z_high=height/2.;
  double z_low=-z_high;
  int z_div=1.;
  double closeness;
  double z_on_mesh;
  int i_point_z;
  for(i_point_z=0;i_point_z<point_z;i_point_z++)
    {
      z_on_mesh=z_low+z_div*i_point_z;
      closeness=abs(z_on_mesh-z);
      if(closeness<1.e-5)
        {
          return i_point_z;
        }
    }
  puts("close z was not found");
    return 1000;
}

int id_r(double r,int point_r)
{
  double r_in=3.;
  double r_out=18.;
  int r_div=1.;
  double closeness;
  double r_on_mesh;
  int i_point_r;
  for(i_point_r=0;i_point_r<point_r;i_point_r++)
    {
      r_on_mesh=r_in+r_div*i_point_r;
      closeness=abs(r_on_mesh-r);
      if(closeness<1.e-5)
        {
          return i_point_r;
        }
    }
  puts("close r was not found.");
  return 1000;
}



void HowFluctuation()
{
  //  const char* path_to_table_with_hole="../various/iinuma_san_task/toSVD_876_684.table";
  const char* path_to_table_without_hole_with_SUS="../various/iinuma_san_task/toSVD_814_weak0_noHole_intg.table";
  ifstream ifn(path_to_table_without_hole_with_SUS);
  if(ifn.fail())
    {
      puts("OPERA data was not found.");
      abort;
    }
  double x,y,z;//[cm]
  double bx,by,bz;//[Gauss]
  string line_in_datafile;
  double height=68.;//[cm]
  int point_z=69;
  int point_r=16;
  int point_theta=36;
  int i_point_z,i_point_r,i_point_theta;
  vector<vector<vector<double>>> bz_3dvec(point_r, vector<vector<double>>(point_z, vector<double>(0)));
  double r,theta;
  
   for(int row=0;getline(ifn,line_in_datafile);++row)
     {
       if(row<8)
         {
           continue;
         }
       istringstream stream(line_in_datafile);
       for(int col=0;stream>>x >>z >>y >>bx >>bz >>by ;++col)
          {
            r=sqrt(x*x+y*y);
            theta=atan2(y,x);
            i_point_r=id_r(r,point_r);
            i_point_z=id_z(z,point_z,height);
            bz_3dvec.at(i_point_r).at(i_point_z).push_back(bz);
          }
     }
   ofstream t__34("_34_SUS.dat");
   ofstream t_0("0_SUS.dat");
   ofstream t_34("34_SUS.dat");
   TH2D* h_ripple=new TH2D("h_ripple",
                           "Bz ripple over rotational direction at r=10 [cm];#theta [rad];z [cm]",
                           point_theta,-TMath::Pi()-1.e-5,2.968,
                           point_z,-height/2.,height/2.);
   for(int i_theta=0;i_theta<point_theta;i_theta++)
     {
       theta=-TMath::Pi()+TMath::TwoPi()*i_theta/point_theta;
       for(int i_z=0;i_z<point_z;i_z++)
         {
           z=-height/2+height*i_z/(point_z-1.);
           bz=bz_3dvec.at(7).at(i_z).at(i_theta);
           h_ripple->Fill(theta,z,bz);
           if(i_z==0)
             {
               t__34<<theta<<"\t"<<bz<<endl;
             }
           if(i_z==34)
             {
               t_0<<theta<<"\t"<<bz<<endl;
             }
           if(i_z==68)
             {
               t_34<<theta<<"\t"<<bz<<endl;
             }
         }
     }
   TCanvas* c=new TCanvas("c","c");
   c->Divide(2,2);
   c->cd(1);
   double size_font=0.065;
   double title_offset=0.7;
   h_ripple->GetXaxis()->SetLabelSize(size_font);
   h_ripple->GetXaxis()->SetTitleSize(size_font);
   h_ripple->GetXaxis()->SetTitleOffset(title_offset);
   h_ripple->GetYaxis()->SetLabelSize(size_font);
   h_ripple->GetYaxis()->SetTitleSize(size_font);
   h_ripple->GetYaxis()->SetTitleOffset(title_offset);
   h_ripple->GetZaxis()->SetLabelSize(size_font/1.2);
   h_ripple->Draw("colz");
   TFile* f=new TFile("CoilMesh_SUS.root","recreate");
   c->Write();
   f->Close();
   delete f;
   delete c;
   delete h_ripple;

   

}









#endif
