
#include"../../src/include.h"
using namespace std;

void report_0620()
{
  magnet m;
  double z_range=.1e-3;
  int n_point=31;
  ofstream of("AxialShift20221127.dat");
  for(int i_point=0;i_point<n_point+1;i_point++)
    {
      cout<<i_point<<"now"<<endl;
      double dz=z_range*(-1.+2.*(i_point/(n_point-1.)));
      of<<dz*1.e3;
      for(int i_coil=0;i_coil<6;i_coil++)
        {
          cout<<i_coil<<"th coil now"<<endl;
          m.dis(i_coil,TVector3(0,0.,dz));
          eval probe(m);
          double U_lc=probe.U_lc();
          of<<"\t"<<U_lc*1.e6;
          m.Perfect();
        }
      of<<endl;
    }
}

void CrossSectionDeformation()
{
  magnet m;
  ofstream of("HorizontalDependence.dat");
  ofstream of2("HorizontalGradient.dat");
  int n_point=1e2;
  double r=33.3e-2;
  double X=10.e-2;
  double dx=X*2./(n_point-1.);
  vector<double> bz_before(6);
  for(int i_point=0;i_point<n_point;i_point++)
    {
      double x=r+X*(-1.+2.*i_point/(n_point-1.));
      x*=1.e2;
      of<<x<<"\t";
      of2<<x<<"\t";
      x/=1.e2;
      TVector3 mfep(x,0.,0.);
      for(int i_coil=0;i_coil<6;i_coil++)
        {
          double bz=m.at(i_coil).B(mfep).z();
          double bz_grad=(bz-bz_before[i_coil])/dx;
          bz_before[i_coil]=bz;
          of<<bz<<"\t";
          if(i_point==0)
            {
              continue;
            }
          of2<<bz_grad<<"\t";
        }
      of<<endl;
      of2<<endl;
    }  
}

void Overview_magnet()
{
  magnet m;
  auto coils=(vector<cb>)m;
  ofstream of_magnet("magnet.dat");
  for(cb cb:coils)
    {
      auto FLCs=cb.coil_vec();
      for(coil flc:FLCs)
        {
          of_magnet<<flc.r()<<"\t"<<flc.z()<<endl;
        }
    }
  ofstream of_field("field.dat");
  int n_r=5;
  int n_z=5;
  double range_r=.3;//half width
  double range_z=.5;//half width
  for(int i_r=0;i_r<n_r;i_r++)
    {
      double r=.333+range_r*(-1.+2.*i_r/(n_r-1.));
      for(int i_z=0;i_z<n_z;i_z++)
        {
          double z=range_z*(-1.+2.*i_z/(n_z-1.));
          TVector3 mfep(r,z,0.);
          TVector3 B=m.B(mfep);
          of_field<<r<<"\t"<<z<<"\t"<<B.x()<<"\t"<<B.z()<<endl;
        }
    }
  
}

void study_RadialDeformation()
{
  magnet m;
  const char* ofn_dat="RadialDeformations.dat";
  ofstream OutputFile(ofn_dat);
  int n_point=21;
  double range=.5e-3;
  for(int i_point=0;i_point<n_point;i_point++)
    {
      double x=range*(-1.+2.*i_point/(n_point/1.));
      for(int i_cb=1;i_cb<6;i_cb++)
        {
          m.deformation(i_cb,x,x,0.);
          eval probe(m);
          double U_lc=probe.U_lc();
          OutputFile<<x*1.e3<<"\t"<<U_lc*1.e6<<endl;
          m.Perfect();
        }
    }  
}

void EdgeIron()
{
  mms iron;
  iron.read("../config/shim_20210923.dat");
  Eigen::VectorXd volumes(iron.n_shim());
  double th=.5;//.68
  for(int i_shim=0;i_shim<iron.n_shim();i_shim++)
    {
      double z=iron.position(i_shim).z();
      if(abs(z)>th)
        {
          volumes(i_shim)=volume_maximum_iron_shim;
        }
      else
        {
          volumes(i_shim)=0.;          
        }
    }
  iron.set(volumes);
  iron.draw("placement.root");
  Draw probe(iron,"EdgeIron.root");

  
}


void RadialDeformations()
{
  magnet m;
  double z_div=.05e-3;//[m]
  double z_range=.5e-3;//[m] scan half width

  int n_point=round(2.*z_range/z_div);
  ofstream of("tilt_additional.dat");
  for(int i_point=0;i_point<n_point+1;i_point++)
    {
      cout<<i_point<<"now"<<endl;
      double dw=-z_range+i_point*z_div;
      of<<dw*1.e3<<"\t";//[mm]
      for(int i_coil=0;i_coil<1;i_coil++)
        {
          cout<<i_coil<<"th coil now"<<endl;
          m.tilt(i_coil,dw,0.);
          eval probe(m);
          double U_theta=probe.U_lc();
          of<<U_theta*1.e6<<"\t";
          cout<<U_theta<<endl;//[ppm]
          m.Perfect();
        }
      of<<endl;
    }
}

void SingularValuesAndModeStrengths()
{
  magnet m;
  m.InputTolerances_manual();
  auto irons=new mms();
  int TruncationMode=120;
  m.shimming_iron(irons,TruncationMode);
}

void RampUpMountain()
{
  magnet m;
  m.InputTolerances_manual();
  Bs before;
  before.SetOnDonut();
  before.add_coil((vector<cb>)m);
  double target=3.+1.e-4;//[T]
  m.SetTargetField(target);
  Bs after;
  after.SetOnDonut();
  after.add_coil((vector<cb>)m);
  auto h_after=new TH1D("after ramp up",";B_{z} at 2600 points [T];entries [a.u.]",
                  100,2.998,3.004);
  h_after->GetYaxis()->SetTitleOffset(.7);
  auto h_before=new TH1D("before ramp up",";B_{z} at 2600 points [T];entries [a.u.]",
                  100,2.998,3.004);
  h_before->GetYaxis()->SetTitleOffset(.7);
  for(int i_mfep=0;i_mfep<before.MFEPs();i_mfep++)
    {
      double bz_before=before.at(i_mfep).Bz();
      double bz_after=after.at(i_mfep).Bz();
      h_before->Fill(bz_before);
      h_after->Fill(bz_after);
    }
  h_before->SetLineColor(2);
  h_after->SetLineColor(52);
  auto c=new TCanvas("c","c");
  c->SetGridx();
  gStyle->SetGridStyle(0);
  gStyle->SetGridWidth(2);
  h_after->SetLineWidth(2);
  h_before->SetLineWidth(2);
  c->Draw();
  h_after->Draw();
  h_before->Draw("sames");
  gPad->Update();
  auto st_before=(TPaveStats*)h_before->FindObject("stats");
  st_before->SetLineColor(2);
  st_before->SetLineWidth(2);
  auto st_after=(TPaveStats*)h_after->FindObject("stats");
  st_after->SetLineColor(52);
  st_after->SetLineWidth(2);
  auto f=new TFile("RampUpMountain.root","recreate");
  c->Write();
  f->Close();
  delete h_before;
  delete h_after;
  delete c;
}

void WorstCaseEstimation()
{
  magnet m;
  int n_trial=1e4;
  ofstream OutputFile("MC1mm.dat");
  auto random=new TRandom2();
  vector<XYZVector> dis_vec_XYZ(6);
  auto start=clock();
  for(int i_trial=0;i_trial<n_trial;i_trial++)
    {
      auto theta_vec=get_theta_vec(random);
      auto phi_vec=get_phi_vec(random);
      auto dis_vec=get_dis_vec(random);
      for(int i=0;i<6;i++)
        {
          dis_vec_XYZ[i]=TVector3_XYZVector(dis_vec[i]);
        }
      vector<double> def_in_vec=get_deformation_inner_vec(random);
      vector<double> def_out_vec=get_deformation_outer_vec(random);
      vector<double> def_thi_vec=get_deformation_thickness_vec(random);
      m.InputTolerance(theta_vec,phi_vec,dis_vec,
                       def_in_vec,
                       def_out_vec,
                       def_thi_vec);
      eval probe(m);
      double U_lc=probe.U_lc();
      double U_theta=probe.U_theta();
      OutputFile<<U_lc<<"\t"<<U_theta<<endl;
      if(i_trial%10 ==0)
        {
          cout<<i_trial<<" now"<<endl;
          clock_t T=clock();
          double delta=(double)(T-start)/CLOCKS_PER_SEC;
          double AssumedLeftMin=delta*(double)(n_trial-i_trial)/(double)i_trial/60.;
          cout<<"it will take "<<AssumedLeftMin<<" mins"<<endl;
        }
    }
}

void WorstCaseEstimation2()
{
  ifstream InputFile("MC1mm.dat");
  double U_max=2000.;//ppm
  int n_bin=100;
  double BinWidth=U_max/n_bin;
  string xtitle="Local uniformity [ppm]";
  string ytitle="Entries [a.u.] / "+to_string((int)BinWidth)+" ppm";
  string Title=";"+xtitle+";"+ytitle;
  auto h_lc=new TH1D("Local uniformity",Title.c_str(),n_bin,0.,U_max);
  double size_font=.06;
  double title_offset_x=.9;
  double title_offset_y=.8;

  h_lc->GetXaxis()->SetLabelSize(size_font);
  h_lc->GetXaxis()->SetLabelOffset(.01);
  h_lc->GetXaxis()->SetTitleSize(size_font);
  h_lc->GetXaxis()->SetTitleOffset(title_offset_x);
  h_lc->GetYaxis()->SetLabelSize(size_font);
  h_lc->GetYaxis()->SetTitleSize(size_font);
  h_lc->GetYaxis()->SetTitleOffset(title_offset_y);
  auto h_theta=new TH1D("Azimuthal averaged uniformity",Title.c_str(),n_bin,0.,U_max);
  string line;
  double U_lc_max=0.;
  double U_lc,U_theta;
  while(getline(InputFile,line))
    {
      istringstream stream(line);
      stream>>U_lc>>U_theta;
      h_lc->Fill(U_lc*1.e6);
      h_theta->Fill(U_theta*1.e6);
      if(U_lc_max<U_lc)
        {
          U_lc_max=U_lc;
        }
    }
  auto c=new TCanvas("c","");
  h_theta->SetLineColor(6);//12
  //  h_theta->Draw();
  h_lc->SetLineColor(1);//6 is pink
  auto st=(TPaveStats*)h_lc->FindObject("stats");
  h_lc->SetStats(1110);
  h_lc->Draw();//"sames");
  gPad->Update();
  double entry_max=gPad->GetUymax();
  cout<<"worst uniformity: "<<U_lc_max*1.e6<<"ppm"<<endl;
  auto arrow_max=new TArrow(U_lc_max*1.e6,0,U_lc_max*1.e6,entry_max);
  arrow_max->SetLineWidth(2);
  gPad->SetGridx();
  arrow_max->SetLineColor(2);
  arrow_max->Draw("same");
  auto f=new TFile("MC1mm.root","recreate");
  c->Write();
  f->Close();  
}

void ReachingUniformity()
{
  int n_point=80;
  int InitialMode=50;
  ofstream of("ReachingUniformity.dat");
  magnet m;
  m.InputTolerances_manual();
  for(int i_point=0;i_point<n_point;i_point++)
    {
      int TruncationMode=InitialMode+i_point;
      cout<<i_point<<" point"<<endl;
      auto IronShims=new mms();
      Bs B_MS,B_0,B_ER,B_TG,B_MM;
      B_MS.SetOnDonut();
      B_MS.add_coil((vector<cb>)m);
      B_MS.DirectionBlinding();
      B_MS.DirectionBlinding();
      B_0.SetOnDonut(Target_Magnetic_Field);
      B_ER.SetOnDonut();
      B_ER.addBs(B_0,B_MS,1.,-1.);
      const char* filename_shimdata="../config/shim_20210923.dat";
      mms IronShims_ref,delta_IronShims;
      IronShims->read(filename_shimdata);
      IronShims_ref.read(filename_shimdata);
      int n_shim=IronShims->n_shim();
      delta_IronShims.read(filename_shimdata);
      auto ResponseMatrix=construct_response_matrix_mms(B_MS,*IronShims);
      SVD DecomposedMatrix(ResponseMatrix);
      B_MM.SetOnDonut();
      B_TG.SetOnDonut();
      IronShims->clear();
      B_MM.Add_MM_RM(ResponseMatrix,*IronShims);
      B_TG.addBs(B_ER,B_MM,1.,-1.);
      setVfromTSVDdata(delta_IronShims,DecomposedMatrix,B_TG,
                       1.e-15,TruncationMode);
      IronShims->add_mms(*IronShims,delta_IronShims);
      eval probe(m,*IronShims);
      double U_theta=probe.U_theta();
      double U_lc=probe.U_lc();
      of<<TruncationMode<<"\t"<<U_theta<<"\t"<<U_lc<<endl;
      cout<<TruncationMode<<"\t"<<U_theta<<"\t"<<U_lc<<endl;
      delete IronShims;
    }  
}

void study_20220920()
{
  magnet m;
  m.InputTolerances_manual(.1e-3);
  auto shim=new mms();
  double target=3. -1.2e-4;//[T]
  m.SetTargetField(target);
  m.CostCalculation(shim,65);
  int n_theta=8;
  int n_r=3;
  ofstream of("20220920.dat");
  double r0=33.3e-2;//[m]
  for(int i_r=0;i_r<n_r;i_r++)
    {
      for(int i_theta=0;i_theta<n_theta;i_theta++)
        {
          double r=r0*i_r/(n_r-1.);
          double theta=TMath::TwoPi()*(double)i_theta/n_theta;
          double x=r*cos(theta);
          double y=r*sin(theta);
          TVector3 mfep(x,y,0.);
          auto b=m.B(mfep);
          b+=shim->B(mfep);
          of<<x<<"\t"<<y<<"\t"<<b.x()<<"\t"<<b.y()<<endl;
          if(i_r==0)
            {
              cout<<"br at the center: "<<hypot(b.x(),b.y())<<"[T]"<<endl;
              break;
            }
        }
    }  
}

void DiscontinuedCoil(string ofn_dat,double z,double FineAngle)
{
  int n_theta=6;
  int n_point=1e3;
  ofstream of(ofn_dat.c_str(),ios_base::app);
  of<<fixed<<setprecision(14);
  for(int i_point=0;i_point<n_point;i_point++)
    {
      double theta=TMath::TwoPi()*i_point/n_point;
      int flag=0;
      for(int i_theta=0;i_theta<n_theta;i_theta++)
        {
          double theta_go=TMath::TwoPi()*i_theta/n_theta -FineAngle;
          double theta_back=TMath::TwoPi()*i_theta/n_theta +FineAngle;
          if(theta_go<theta and theta<theta_back)
            {
              flag++;
              break;
            }
        }
      if(flag)
        {
          continue;
        }
      double x=radius_ShimCoil_Bz*cos(theta);
      double y=radius_ShimCoil_Bz*sin(theta);
      of<<x<<"\t"<<y<<"\t"<<z<<endl;
    }
}

void JointPart(string ofn_dat,double bottom,double top,double FineAngle)
{
  int n_theta=6;
  ofstream of(ofn_dat.c_str(),ios_base::app);
  of<<fixed<<setprecision(14);
  int n_point=1e2;
  for(int i_theta=0;i_theta<n_theta;i_theta++)
    {
      double theta=TMath::TwoPi()*i_theta/n_theta-FineAngle;
      for(int go=0;go<2;go++)
        {
          for(int i_point=0;i_point<n_point;i_point++)
            {
              double x=radius_ShimCoil_Bz*cos(theta);
              double y=radius_ShimCoil_Bz*sin(theta);
              double z=bottom+(top-bottom)*(i_point/(n_point-1.));
              of<<x<<"\t"<<y<<"\t"<<z<<endl;
            }
          theta+=2.*FineAngle;
        }
    }
}


void ShimCoilDrawing()
{
  string ofn_dat="BzShimCoilSkelton.dat";
  string command="rm "+ofn_dat;
  double FineAngle=.01;//[rad]
  int result=system(command.c_str());
  DiscontinuedCoil(ofn_dat,top_ShimCoil_Bz,FineAngle);
  DiscontinuedCoil(ofn_dat,-top_ShimCoil_Bz,FineAngle);
  DiscontinuedCoil(ofn_dat,bottom_ShimCoil_Bz,FineAngle);
  DiscontinuedCoil(ofn_dat,-bottom_ShimCoil_Bz,FineAngle);
  JointPart(ofn_dat,bottom_ShimCoil_Bz,top_ShimCoil_Bz,FineAngle);
  JointPart(ofn_dat,-top_ShimCoil_Bz,-bottom_ShimCoil_Bz,FineAngle);
}

void study_20220923()
{
  string ofn_dat="TargetFieldCheck.dat";
  ofstream of(ofn_dat.c_str());
  int n_point=20;
  magnet m;
  double adjust0=1.1e-4;//[T] good at tolerance level=0.1mm
  double tolerance0=.1e-3;//[m]
  double tolerance_max=1.5e-3;
  int mode0=60;
  int mode_limit=100;
  int n_pocket=400;
  double requirement=.2e-6;
  auto r=new TRandom2();
  double NoiseLevel=2.e-5;
  for(int i_point=0;i_point<n_point;i_point++)
    {
      double tolerance=tolerance0+(tolerance_max-tolerance0)*i_point/(n_point-1.);
      double factor=tolerance/tolerance0;
      m.InputTolerances_manual(tolerance);
      double noise=r->Gaus(0.,NoiseLevel);
      double target=3.-adjust0*factor+noise;
      m.SetTargetField(target);
      of<<setprecision(7)<<target<<endl;
      continue;
      int mode_min;
      double U_best=1.;
      for(int mode=mode0;mode<mode_limit;mode++)
        {
          cout<<i_point<<"point, tolerance level:";
          cout<<tolerance*1.e3<<"mm, "<<mode<<"mode"<<endl;
          auto shim=new mms();
          m.CostCalculation(shim,mode);
          double rate_low=(double)shim->count("low")/(double)n_pocket;
          double rate_ok=(double)shim->count("ok")/(double)n_pocket;
          double rate_high=(double)shim->count("high")/(double)n_pocket;
          cout<<rate_low<<"\t"<<rate_ok<<"\t"<<rate_high<<endl;
          eval probe(m,*shim);
          double U_lc=probe.U_lc();
          if(U_lc<U_best)
            {
              U_best=U_lc;
              mode_min=mode;
            }
          delete shim;
          if(U_lc<requirement)
            {
              double U_theta=probe.U_theta();
              of<<tolerance<<"\t"<<U_lc*1.e6<<"\t"<<U_theta*1.e6<<"\t"<<rate_low;
              of<<"\t"<<rate_ok<<"\t"<<rate_high<<"\t"<<mode<<"\t"<<target<<endl;
              break;
            }
          if(mode==mode_limit-1)
            {
              mode=mode_min;
              cout<<i_point<<"point, tolerance level:";
              cout<<tolerance*1.e3<<"mm, "<<mode<<"mode"<<endl;
              auto shim=new mms();
              m.CostCalculation(shim,mode);
              double rate_low=(double)shim->count("low")/(double)n_pocket;
              double rate_ok=(double)shim->count("ok")/(double)n_pocket;
              double rate_high=(double)shim->count("high")/(double)n_pocket;
              cout<<rate_low<<"\t"<<rate_ok<<"\t"<<rate_high<<endl;
              eval probe(m,*shim);
              double U_lc=probe.U_lc();
              delete shim;
              double U_theta=probe.U_theta();
              of<<tolerance<<"\t"<<U_lc*1.e6<<"\t"<<U_theta*1.e6<<"\t"<<rate_low;
              of<<"\t"<<rate_ok<<"\t"<<rate_high<<"\t"<<mode<<endl;
              break;
            }
        }
    }  
}

void oval_20221001()
{
  magnet m;
  double dev=.1e-3;
  m.deformation_elliptical(5,dev,0.);
  ofstream of("oval_MC30.dat");
  int n_theta=100;
  int n_z=40;
  double r=33.3e-2;
  double Z=5.e-2;
  of<<fixed<<setprecision(15);
  for(int i_theta=0;i_theta<n_theta;i_theta++)
    {
      for(int i_z=0;i_z<n_z;i_z++)
        {
          double theta=TMath::Pi()*(-1.+2.*(double)i_theta/n_theta);
          double z=Z*(-1.+2.*i_z/(n_z-1.));
          TVector3 mfep(r*cos(theta),r*sin(theta),z);
          double bz=m.B(mfep).z();
          of<<theta<<"\t"<<z<<"\t"<<bz<<endl;
        }
    }
}

void study_20221002()
{
  string ofn_root="BzShimCoilError_20221006.root";
  int i_mode=2;
  int n_trial=1e4;
  string type="Bz";
  ShimCoil sc(type);
  double TargetAmplitude=7.8e-4;//[T]
  sc.ApplySameCurrent(1.e3);
  auto random=new TRandom2();
  double min_Bz,ave_Bz,max_Bz,p2p_Bz;
  Bs B_ref("donut");
  B_ref.add_ShimCoil(sc);
  auto tr=new TTree("tr","");
  tr->Branch("min_Bz",&min_Bz);
  tr->Branch("ave_Bz",&ave_Bz);
  tr->Branch("max_Bz",&max_Bz);
  tr->Branch("p2p_Bz",&p2p_Bz);

  for(int i_trial=0;i_trial<n_trial;i_trial++)
    {
      if(i_trial%10==0)
        {
          cout<<i_trial<<"/"<<n_trial<<" trials now"<<endl;
        }
      T_ShimCoil t(random);
      sc.InputTolerances(t);
      Bs B("donut");
      B.add_ShimCoil(sc);
      B.addBs(B,B_ref,1.,-1.);
      min_Bz=B.min('z');
      max_Bz=B.max('z');
      p2p_Bz=max_Bz-min_Bz;
      cout<<p2p_Bz<<endl;
      cout<<min_Bz<<endl;
      tr->Fill();
      sc.Perfect();
    }
  auto OutputFile=new TFile(ofn_root.c_str(),"recreate");
  tr->Write();
  OutputFile->Close();

  delete tr;
  delete OutputFile;
  delete random;
}

void analysis_study_20221002()
{
  string ifn_root="BzShimCoilError_20221006.root";
  auto fin=new TFile(ifn_root.c_str());
  auto tr=(TTree*)fin->Get("tr");
  double min_Bz,ave_Bz,max_Bz,p2p_Bz;
  
  tr->SetBranchAddress("min_Bz",&min_Bz);
  tr->SetBranchAddress("ave_Bz",&ave_Bz);
  tr->SetBranchAddress("max_Bz",&max_Bz);
  tr->SetBranchAddress("p2p_Bz",&p2p_Bz);
  auto h=new TH1D("h",";#DeltaB_{z} (#muT_{p-p});Entries",100,0.,.45);
  int n_entry=tr->GetEntries();
  for(int i_entry=0;i_entry<n_entry;i_entry++)
    {
      tr->GetEntry(i_entry);
      double p2p_uT=p2p_Bz*1.e6;
      h->Fill(p2p_uT);
    }
  auto c=new TCanvas("c","");
  h->Draw();
  auto st=(TPaveStats*)h->FindObject("stats");
  h->SetLineWidth(2);
  h->SetStats(10);
  h->GetXaxis()->SetNdivisions(505);
  h->GetYaxis()->SetNdivisions(505);
  gPad->Update();
  h->Draw();
  string ofn_root="analysis_"+ifn_root;
  auto f=new TFile(ofn_root.c_str(),"recreate");
  c->Write();
  f->Close();
  delete f;
  delete fin;
}

void ActiveShimming20221006()
{
  magnet m;
  m.InputTolerances_manual(.1e-3);
  // Draw p(m,"a.root");
  // return;
  m.SetTargetField(3.- 1.2e-4);
  auto shim=new mms();
  m.CostCalculation(shim,65);
  shim->draw("IronPlacement-100um.root");
  //  return;
  Draw p(m,*shim,"AfterPassiveShimming20221230.root");
  return;
  auto sc=new ShimCoil("Br");
  //  m.ActiveShimming(*shim,sc);
  Draw probe(m,*shim,*sc,"AfterActiveShimming20221221.root");
  return;
  auto cs=sc->GetCurrents();
  for(int i=0;i<12;i++)
    {
      cout<<cs[i]<<endl;
    }
  int n_theta=100;
  int n_z=100;
  double r=33.3e-2;
  auto h=new TH2D("h",";#theta [rad/#pi];z [cm]",
                  n_theta+1,-1.-1./(double)n_theta,1.,
                  n_z,-5.*(1.+1./(double)n_z),5.*(1.+1./(double)n_z));
  h->Draw("colz");
  h->GetZaxis()->SetTitle("B_{r} [#muT]");
  gPad->Update();
  for(int i_theta=0;i_theta<n_theta;i_theta++)
    {
      double theta=TMath::Pi()*(-1.+2.*(double)i_theta/(double)n_theta)+1.e-5;
      for(int i_z=0;i_z<n_z;i_z++)
        {
          double z=5.e-2 *(-1.+2.*i_z/(n_z-1.));
          TVector3 mfep(r*cos(theta),r*sin(theta),z);
          TVector3 B=m.B(mfep)+shim->B(mfep)+sc->B(mfep);
          double bz_draw=1.e6*(B.z()-3.)/3.;
          bz_draw-=0.;
          //          double Br=B.Dot(TVector3(cos(theta),sin(theta),0.));
          h->Fill(theta/TMath::Pi(),z*1.e2,bz_draw);
        }
    }
  auto c=new TCanvas("c","");
  c->Draw();
  ArrangeRange(h);
  gPad->SetLeftMargin(.05);
  gPad->SetRightMargin(.15);
  gPad->SetBottomMargin(.15);
  SetCylinderHist(h);
  h->GetZaxis()->SetTitle("B_{r} [#muT]");
  h->GetZaxis()->SetTitleSize(.07);
  h->GetZaxis()->CenterTitle(true);  
  h->GetZaxis()->SetTitleOffset(.6);  
  h->Draw("colz");
  h->GetZaxis()->SetTitle("(B_{z} - B_{z}^{ave}) / B_{z}^{ave} [ppm]");
  auto f=new TFile("AfterActiveShimming20221221.root","recreate");
  c->Write();
  h->Write();
  f->Close();  
}







