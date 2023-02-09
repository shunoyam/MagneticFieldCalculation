
#include"../../src/include.h"
using namespace std;

void ConsistencyCheck(const char* path_to_MFEP_configuration_file,
                      const char* ofn_max_current_density,
                      const char* ofn_decreasing_error,
                      const char* ofn_root)
{
  //1
  puts("step 1");
  Bs B_target;
  B_target.read(path_to_MFEP_configuration_file);

  //2
  puts("step 2");
  cb Coil;
  set_coil_miniSol(Coil);
  B_target.subtract(Coil);

  puts("target was set");
  cb IronYoke;
  const char* yoke_config_fn="iron_shape_5mm.dat";
  const char* path_to_iron_yoke_config_file="../config/iron_shape_5mm.dat";
  ofstream file_max_current_density(ofn_max_current_density);
  double pitch=1.e-3;
  create_yoke_geometry(pitch,yoke_config_fn);
  IronYoke.read_rz(path_to_iron_yoke_config_file);
  int n_coil_yoke=IronYoke.coil_vec().size();
  cout<<"FLC number: "<<n_coil_yoke<<endl;
  cout<<"MFEP number: "<<B_target.MFEPs()<<endl;
  //3
  puts("step 3");
  int TruncatedMode=63;
  ofstream f_p2p(ofn_decreasing_error);
  vector<double> CurrentDensity_vec;
  double max_CurrentDensity;
  double threshold_current_density=1.7e6;//[A/m]
  f_p2p<<"# truncated mode number \t p-p of error field [Gauss]"<<endl;

  for(int i_TruncatedMode=TruncatedMode-2;
      i_TruncatedMode<TruncatedMode;
      i_TruncatedMode++)
    {
      puts("----------------------");
      if(!(i_TruncatedMode%2))
        {
                    continue;
        }
      optimize_coil(IronYoke,
                    i_TruncatedMode,B_target,
                    0,"not_needed");
      ofstream f_coil("IronYoke_distribution.dat");
      f_coil<<"#filament loop coils on the surface of the iron yoke of mini solenoid"<<endl;
      f_coil<<"#r[mm]\t z[mm]\t I[A]"<<endl;
      int even=0;
      for(coil coil:IronYoke.coil_vec())
        {
          even++;
          if(even%2)
            {
              continue;
            }
          f_coil<<coil.r()*1.e3<<"\t"<<coil.z()*1.e3<<"\t"<<coil.I()<<endl;
          CurrentDensity_vec.push_back(coil.I()/pitch);//[A/m]
        }
       max_CurrentDensity=*max_element(CurrentDensity_vec.begin(),
                                                 CurrentDensity_vec.end());
      cout<<"max of reproduced current density: ";
      cout<<max_CurrentDensity/1.e6<<"[kA/mm]"<<endl;
      file_max_current_density<<i_TruncatedMode<<"\t"<<max_CurrentDensity/1.e6<<endl;
      //4
      puts("step 4");
      Bs B_error;
      B_error=B_target;
      B_error.subtract(IronYoke);
      double min_B_error=B_error.min('z');
      double max_B_error=B_error.max('z');
      double p2p=max_B_error-min_B_error;
      f_p2p<<i_TruncatedMode<<"\t"<<p2p*1.e4<<endl;
      cout<<"reproduced with the accuracy of "<<p2p*1.e4<<"[Gauss]"<<endl;
      cout<<"minimum of error Bz :"<<min_B_error*1.e4<<"[Gauss]"<<endl;
      cout<<"maximum of error Bz :"<<max_B_error*1.e4<<"[Gauss]"<<endl;
      if(max_CurrentDensity>threshold_current_density)
        {
          break;
        }
      }
  
  // draw_B_miniSol(IronYoke,Coil,ofn_root,
  //                path_to_MFEP_configuration_file);
}

void draw_B_miniSol(cb FLCs_IronYoke,cb Coil,const char* ofn_root,
                    const char* path_to_MFEP_configuration_file)
{
  puts("drawing");
   double r_in=0.05;
  double r_out=0.18;
  double r_center=(r_out+r_in)/2.;
  double r_width=(r_out-r_in)/2.;
  double height=0.68;
  Bs B_xz;
  int mesh_r=14;
  int mesh_z=69;
  B_xz.setonXZ(r_center,r_width,0.,height/2.,0.,mesh_r,mesh_z);
  B_xz.add_coil(FLCs_IronYoke);
  B_xz.add_coil(Coil);
  Bs B_OnlyCoil;
  B_OnlyCoil.setonXZ(r_center,r_width,0.,height/2.,0.,mesh_r,mesh_z);
  B_OnlyCoil.add_coil(Coil);

  Bs B_target;
  B_target.read(path_to_MFEP_configuration_file);
 auto h_error=new TH2D("h_error",
                       "error field B_{rep}-B_{tg} [Gauss];x[cm];z[cm]",
                   mesh_r,r_in*100.,r_out*100.,
                   mesh_z,-height*100./2.,height*100./2.);
 auto h_all=new TH2D("h_all",
              "target magnetic field [Gauss];x[cm];z[cm]",
              mesh_r,r_in*100.,r_out*100.,
              mesh_z,-height*100./2.,height*100./2.);
 auto h_OnlyCoil=new TH2D("h_OnlyCoil",
              "magnetic field by coil [Gauss];x(cm);z(cm)",
              mesh_r,r_in*100.,r_out*100.,
              mesh_z,-height*100./2.,height*100./2.);
 auto h_yoke=new TH2D("h_yoke",
              "magnetic field by yoke [Gauss];x(cm);z(cm)",
              mesh_r,r_in*100.,r_out*100.,
              mesh_z,-height*100./2.,height*100./2.);
  SetStyle_TH2D(h_error);
  SetStyle_TH2D(h_all);
  SetStyle_TH2D(h_OnlyCoil);
  SetStyle_TH2D(h_yoke);
  vector<Bfield> B_xz_vec=B_xz;
  vector<Bfield> B_OnlyCoil_vec=B_OnlyCoil;
  double Bz_near;
  for(int i_MFEP=0;i_MFEP<B_xz_vec.size();i_MFEP++)
    {
      double r_MFEP=B_xz_vec[i_MFEP].MFEP().x();
      double z_MFEP=B_xz_vec[i_MFEP].MFEP().z();
      double Bz_OnlyCoil=B_OnlyCoil_vec[i_MFEP].Bz();
      double Bz_coil_yoke=B_xz_vec[i_MFEP].Bz();
      double Bz_yoke=Bz_coil_yoke-Bz_OnlyCoil;
      double Bz_all=B_target.at(i_MFEP).Bz();
      double Bz_error=Bz_coil_yoke-Bz_all;
      double abs_Bz_error=abs(Bz_error);
      h_error->Fill(r_MFEP*100.,z_MFEP*100.,abs_Bz_error*1.e4);
      h_all->Fill(r_MFEP*100.,z_MFEP*100.,Bz_all*1.e4);
      h_OnlyCoil->Fill(r_MFEP*100.,z_MFEP*100.,Bz_OnlyCoil*1.e4);
      h_yoke->Fill(r_MFEP*100.,z_MFEP*100.,Bz_yoke*1.e4);
    }
  ArrangeRange(h_error);
  ArrangeRange(h_OnlyCoil);
  ArrangeRange(h_all);
  ArrangeRange(h_yoke);
  string option="colz";
  auto c=new TCanvas("c","c");
  c->Divide(2,2);
  SetStyle_TCanvas(c);
  c->cd(1);
  h_all->Draw(option.c_str());
  c->cd(2);
  //  gPad->SetLogz(1);
  h_error->Draw(option.c_str());
  c->cd(3);
  h_OnlyCoil->Draw(option.c_str());
  c->cd(4);
  h_yoke->Draw(option.c_str());
  auto *fout=new TFile(ofn_root,"recreate");
  c->Write();
  fout->Close();
  delete h_error;
  delete h_all;
  delete h_OnlyCoil;
  delete h_yoke;
  delete c;
  delete fout;
}

void set_coil_miniSol(cb& Coil)
{
  double diameter_in_main=.46;
  double diameter_out_main=.4808;
  double height_main=.54;
  double r_in_main=diameter_in_main/2.;
  double r_out_main=diameter_out_main/2.;
  double center_r_main=(r_in_main+r_out_main)/2.;
  double HalfWidth_r_main=r_out_main-center_r_main;
  int node=50;
  double CurrentDensity_main=88.7986069450757;//A/cm2
  cb MainCoil;
   MainCoil.SetOnGaussNodes(center_r_main,HalfWidth_r_main,node,
                             0.,height_main/2.,node,
                            CurrentDensity_main*1.e4);
  Coil.Push_Back(MainCoil);
  double height_weak=.08855756;
  double CurrentDensity_weak=-68.397846473723;//A/cm2
  double diameter_in_weak=.4818;
  double diameter_out_weak=.5026;
  double r_in_weak=diameter_in_weak/2.;
  double r_out_weak=diameter_out_weak/2.;
  double center_r_weak=(r_in_weak+r_out_weak)/2.;
  double HalfWidth_r_weak=r_out_weak-center_r_weak;
  cb WeakCoil;
  WeakCoil.SetOnGaussNodes(center_r_weak,HalfWidth_r_weak,node,
                            0.,height_weak/2.,node,
                           CurrentDensity_weak*1.e4);
  Coil.Push_Back(WeakCoil);
}

void create_yoke_geometry(double pitch,const char* ofn_dat)
{
  ofstream f(ofn_dat);
  double x_out=0.3;
  double x_in=0.01;
  double y_out=0.408;
  double y_in=0.288;
  double x=1.e-8,y=y_in;

  while(x<x_in)
    {
      f<<fixed<<setprecision(15)<<x<<"\t"<<y<<endl;
      f<<fixed<<setprecision(15)<<x<<"\t"<<-y<<endl;
      x+=pitch;
    }
  x=x_in;

  while(y<y_out)
    {
      f<<fixed<<setprecision(15)<<x<<"\t"<<y<<endl;
      f<<fixed<<setprecision(15)<<x<<"\t"<<-y<<endl;
      y+=pitch;
    }
  y=y_out;

  while(x<x_out)
    {
      f<<fixed<<setprecision(15)<<x<<"\t"<<y<<endl;
      f<<fixed<<setprecision(15)<<x<<"\t"<<-y<<endl;
      x+=pitch;
    }
  x=x_out;

  while(0.<y)
    {
      f<<fixed<<setprecision(15)<<x<<"\t"<<y<<endl;
      f<<fixed<<setprecision(15)<<x<<"\t"<<-y<<endl;
      y-=pitch;
    }
  f<<x<<"\t 0."<<endl;
  y=-y_out;

  const char* path=" ../config/";
  mv(ofn_dat,path);    
}

static void draw(Bs B_target,const char* ofn_root)
{
   double r_in=0.05;
  double r_out=0.18;
  double r_center=(r_out+r_in)/2.;
  double r_width=(r_out-r_in)/2.;
  double height=0.68;
  int mesh_r=14;
  int mesh_z=69;
  double size_font=0.05;
  double size_title=0.4;
  const char* path_to_MFEP_configlation_file="../config/MFEP_miniSol.dat";
  auto* h=new TH2D("h",
              "|B| which iron yoke makes;x(cm);z(cm)",
                   mesh_r,r_in*100.,r_out*100.,
                   mesh_z,-height*100./2.,height*100./2.);

  double r_MFEP,z_MFEP,Bz,Br,B_norm;
  TVector3 B_TVector3;
  vector<Bfield> B_vec=B_target;
  for(int i_MFEP=0;i_MFEP<B_vec.size();i_MFEP++)
    {
      r_MFEP=B_vec[i_MFEP].MFEP().x();
      z_MFEP=B_vec[i_MFEP].MFEP().z();
      B_TVector3=B_vec[i_MFEP].B();
      Br=B_TVector3.x();
      Bz=B_TVector3.z();
      B_norm=sqrt(Bz*Bz+Br*Br);
      h->Fill(r_MFEP*100.,z_MFEP*100.,B_norm*1.e4);
    }

  h->GetXaxis()->SetLabelSize(size_font);
  h->GetYaxis()->SetLabelSize(size_font);
  h->SetStats(0);
  
  auto *fout=new TFile(ofn_root,"recreate");
  h->Draw("colz");
  h->Write();
  fout->Close();
  delete fout;  
}



