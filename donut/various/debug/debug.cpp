
#include"../../src/include.h"
using namespace std;

void debug_shim_coil()
{
  const char* ofn_root="debug_shim_coil.root";
  double r=radius_ShimCoil_Br;
  double z_top=.75;
  double z_bottom=.058;
  double I=1.e3;
  ShimCoil shim("Br");
  shim.ApplySameCurrent(I);
  vector<double> theta(12,0.);
  vector<double> phi(12,0.);
  vector<double> psi(12,0.);
  double Z=.1;
  for(int i=0;i<12;i++)
    {
      if(i%2==0)
        {
          theta[i]=TMath::Pi();
        }
    }
  shim.InputTilts(theta,phi,psi);
  vector<coil> C(4);
  C[0].set(I,r,z_top);
  C[1].set(-I,r,z_bottom);
  C[2].set(-I,r,-z_bottom);
  C[3].set(I,r,-z_top);
  int n=100;
  double range=1.;
  auto h_shim=new TH2D("h_shim","shim coil;x[cm];y[cm]",
                       n,-range*1.e2,range*1.e2,
                       n,-range*1.e2,range*1.e2);

  auto h_flc=new TH2D("h_flc","4 FLC ;x[cm];y[cm]",
                      n,-range*1.e2,range*1.e2,
                      n,-range*1.e2,range*1.e2);
  SetStyle_TH2D(h_shim);
  SetStyle_TH2D(h_flc);
  for(int i_x=0;i_x<n;i_x++)
    {
      for(int i_y=0;i_y<n;i_y++)
        {
          double x=range*(-1.+2.*i_x/(n-1.));
          double y=range*(-1.+2.*i_y/(n-1.));
          TVector3 mfep(x,y,Z);
          double bz=shim.B(mfep).z();
          h_shim->Fill(x*1.e2,y*1.e2,bz);
          bz=0.;
          for(coil c:C)
            {
              bz+=c.b(mfep).z();
            }
          h_flc->Fill(x*1.e2,y*1.e2,bz);
        }      
    }
  ArrangeRange(h_flc);
  ArrangeRange(h_shim);
  auto c=new TCanvas("c","c");
  c->Divide(2,2);
  c->cd(1);
  h_shim->Draw("colz");
  c->cd(2);
  h_flc->Draw("colz");
  auto f=new TFile(ofn_root,"recreate");
  c->Write();
  f->Close();
  delete c;
  delete f;
  delete h_flc;
  delete h_shim;
  
}

void debug_line()
{
  double r=1.;
  double center_angle=TMath::Pi();
  double CentralAngle=TMath::Pi();
  
  int n=100;
  double I=1.e3;
  ofstream of("line_segment_current.dat");
  double x_coil=r;
  double y_coil=0.;
  double range=0.5;
  auto h_line=new TH2D("h_line",
                       "horizontally B-field by line segment current[Gauss];x[cm];y[cm]",
                       n,(x_coil-range)*100.,(x_coil+range)*100.,
                       n,(y_coil-range)*100.,(y_coil+range)*100.);
  SetStyle_TH2D(h_line);
  double height=100.;
  shim_coil sh(1,height/2.,-height/2.,0.,TMath::Pi());
  sh.SetCurrent(I);
  ofstream of_arrow("arrow.dat");
  for(int i_x=0;i_x<n;i_x++)
    {
      for(int i_y=0;i_y<n;i_y++)
        {
          double x=x_coil-range*(-1.+2.*i_x/(n-1.));
          double y=y_coil-range*(-1.+2.*i_y/(n-1.));
          TVector3 mfep(x,y,0.);
          double by=sh.ResponseElement_LineSegment(mfep,0.).y();
          by*=I;
          double bx=sh.ResponseElement_LineSegment(mfep,0.).x();
          bx*=I;
          h_line->Fill(x*100.,y*100.,(by*1.e4));
          of_arrow<<x<<"\t"<<y<<"\t"<<bx*1.e4<<"\t"<<by*1.e4<<endl;
        }
    }
  ArrangeRange(h_line);
  auto cc=new TCanvas("c_line","c_line");
  cc->Divide(2,2);
  cc->cd(1);
  h_line->Draw("colz");
  auto f=new TFile("line.root","recreate");
  cc->Write();
  f->Close();
  delete f;
  delete cc;
  delete h_line;
}

void StretchingSegment()
{
  ofstream of("stretch.dat");
  int n_point=1e2;
  double min_l=.1;//[m]
  double max_l=1.e9;
  double range=max_l/min_l;
  double base=pow(range,1./(double)n_point);
  double I=-.5e7;
  double r=1.;
  shim_coil sc(r,0.,0.,0.,0.,I);
  TVector3 mfep;

  for(int i_point=0;i_point<n_point;i_point++)
    {
      double factor=pow(range,(double)i_point/(n_point-1.));
      double l=min_l*factor;
      sc.SetHeight(-l/2.,l/2.);
      auto b=sc.ResponseElement_LineSegment(mfep,0.)*I;
      of<<l<<"\t"<<b.y()-1.<<endl;
    }
}

void debug_ShimCoil()
{
  ShimCoil SC("Br");
  double r=.86;
  double z_mfep=0.;
  double I=1.e3;
  SC.ApplySameCurrent(-I);
  vector<coil> flcs(4);
  double high=.75;
  double low=.058;
  flcs[0].set(I,r,high);
  flcs[1].set(-I,r,-high);
  flcs[2].set(-I,r,low);
  flcs[3].set(I,r,-low);
  cb cb(flcs);
  //  Draw draw_cb(cb,"flc.root");
  Draw draw_shim(SC,"shim.root");
  return;
  
  int n_mfep=1e2;
  double range=r*(1.+(double)n_mfep)/(double)n_mfep;
  auto h_dif=new TH2D("h_dif",";;",n_mfep,-range,range,n_mfep,-range,range);
  auto h_shim=new TH2D("h_shim",";;",n_mfep,-range,range,n_mfep,-range,range);
  auto h_flc=new TH2D("h_flc",";;",n_mfep,-range,range,n_mfep,-range,range);
  SetStyle_TH2D(h_dif);
  SetStyle_TH2D(h_shim);
  SetStyle_TH2D(h_flc);
  
  for(int i_x=0;i_x<n_mfep;i_x++)
    {
      for(int i_y=0;i_y<n_mfep;i_y++)
        {
          double x=r*(-1.+2.*i_x/(n_mfep-1.));
          double y=r*(-1.+2.*i_y/(n_mfep-1.));
          TVector3 mfep(x,y,z_mfep);
          TVector3 B_flc;
          for(int i=0;i<4;i++)
            {
              B_flc+=flcs[i].b(mfep);
            }
          auto B_shim=SC.B(mfep);
          double b_flc=B_flc.Mag();
          double b_dif=(B_flc+B_shim).Mag();
          h_shim->Fill(x,y,B_shim.Mag());
          h_flc->Fill(x,y,b_flc);
          h_dif->Fill(x,y,b_dif);
        }
    }
  //  ArrangeRange(h_dif);
  auto c=new TCanvas("c","c");
  SetStyle_TCanvas(c);
  c->Divide(2,2);
  c->Draw();
  c->cd(1);
  h_flc->Draw("colz");
  c->cd(2);
  h_shim->Draw("colz");
  c->cd(3);
  h_dif->Draw("colz");
  auto f=new TFile("BrShimCoil.root","recreate");
  c->Write();
  f->Close();
  delete c;
  delete f;
  delete h_dif;
  delete h_shim;
  delete h_flc;
}

void check_LegendreQuadrature()
{
  int degree=8;
  Legendre l(degree);
  precise();
  for(int i=0;i<degree;i++)
    {
      cout<<i<<"th node: "<<l.node(i)<<"\t | \t";
      cout<<i<<"th weight: "<<l.weight(i)<<endl;
    }
}

void debug_BiotSavart_elliptical()
{
  double I=1.e3;
  double r=1.;
  double z=0.;
  double dev_r=0.;
  coil c(I,r,z);
  coil c_ana(I,r,z);
  c.deformation_undulation(2,dev_r,0.);

  int n=100;
  double range=.5;
  auto h=new TH2D("h","Bz by set of coil line segments[Gauss];x[cm];y[cm]",
                  100,-range*1.e2,range*1.e2,
                  100,-range*1.e2,range*1.e2);
  auto h_ana=new TH2D("h_ana","Bz analytically[Gauss];x[cm];y[cm]",
                      100,-range*1.e2,range*1.e2,
                      100,-range*1.e2,range*1.e2);
  auto h_dif=new TH2D("h_dif","difference[Gauss];x[cm];y[cm]",
                      100,-range*1.e2,range*1.e2,
                      100,-range*1.e2,range*1.e2);
  SetStyle_TH2D(h);
  SetStyle_TH2D(h_ana);
  SetStyle_TH2D(h_dif);
  for(int i_x=0;i_x<n;i_x++)
    {
      for(int i_y=0;i_y<n;i_y++)
        {
          double x=range*(-1.+2.*i_x/(n-1.));
          double y=range*(-1.+2.*i_y/(n-1.));
          TVector3 mfep(x,y,1.);
          double bz=c.b(mfep).z();
          double bz_ana=c_ana.b(mfep).z();
          h->Fill(x*1.e2,y*1.e2,bz*1.e4);
          h_ana->Fill(x*1.e2,y*1.e2,bz_ana*1.e4);
        }
    }
  h_dif->Add(h,h_ana,1,-1.);

  ArrangeRange(h);
  ArrangeRange(h_ana);
  ArrangeRange(h_dif);
  
  const char* option="colz";
  auto can=new TCanvas("can","can");
  can->Divide(2,2);
  SetStyle_TCanvas(can);
  can->cd(1);
  h->Draw(option);
  can->cd(2);
  h_ana->Draw(option);
  can->cd(3);
  h_dif->Draw(option);
  auto of_root=new TFile("debug_LineSegment_coil.root","recreate");
  can->Write();
  of_root->Close();
  delete can;
  delete of_root;
  delete h;
  delete h_ana;
  delete h_dif;
}

void Convergence_LineSegment()
{
  double I=1.e3;
  double r=1.;
  double z=0.;
  double dev_r=0.;
  coil coil_circle(I,r,z);
  TVector3 mfep_ref(1.,.4,.1);
  //  ofstream of_line_segment("convergence_LineSegment.dat");
  int n=14;
  double bz_circle=coil_circle.b(mfep_ref).z();
  cout<<bz_circle*1.e6<<endl;
  for(int i=0;i<n;i++)
    {
      int n_element=pow(3,i+3);
      coil coil_not_ellipse(I,r,z);      
      coil_not_ellipse.deformation_undulation(2,dev_r,0.,n_element);
      double bz_ellipse=coil_not_ellipse.b(mfep_ref).z();
      double bz_dif=bz_ellipse-bz_circle;
      //      of_line_segment<<n_element<<"\t"<<bz_dif<<endl;
    }
}

void debug_shim_coil_ME()
{
  double r=1.;
  double z=1.;
  double center_angle=0.;
  double CentralAngle=TMath::Pi();
  double I=1.e4;
  shim_coil s(r,z,-z,center_angle,CentralAngle,I);

  draw(s,"perfect");
  double sin_theta2=80./(100.*sqrt(2.));
  double theta2=asin(sin_theta2);
  double sin_theta1=(cos(theta2)-sin(theta2))/sqrt(2.);
  double theta1=asin(sin_theta1);
  double TiltAngle=theta1;
  //  double TiltAngle=TMath::Pi()*1./180.;
  s.InputTilt(TiltAngle*0.9,0.,0.);
  draw(s,"tilt");
}

static void draw(shim_coil s,string qualifier)
{
  string ofn_root="debug_shim_coil_"+qualifier+".root";
  double range_x=1.5;
  double range_y=0.9;
  double range_z=.8;
  auto h_xy=new TH2D("h_xy","Bx on xy plane[mT];x[cm];y[cm]",
                     100,-range_x*1.e2,range_x*1.e2,
                     100,-range_y*1.e2,range_y*1.e2);
  auto h_xz=new TH2D("h_xz","Bz on xz plane[mT];x[cm];z[cm]",
                     100,-range_x*1.e2,range_x*1.e2,
                     100,-range_z*1.e2,range_z*1.e2);
  SetStyle_TH2D(h_xy);
  SetStyle_TH2D(h_xz);
  
  //xy plane 
  for(int i_x=0;i_x<100;i_x++)
    {
      for(int i_y=0;i_y<100;i_y++)
        {
          double x=range_x*(-1.+2.*i_x/(99.));
          double y=range_y*(-1.+2.*i_y/(99.));
          TVector3 mfep(x,y,0.);
          double bx=s.B(mfep).x();
          h_xy->Fill(x*1.e2,y*1.e2,bx*1.e3);
        }
    }

  //xz plane
  for(int i_x=0;i_x<100;i_x++)
    {
      for(int i_z=0;i_z<100;i_z++)
        {
          double x=range_x*(-1.+2.*i_x/(99.));
          double z=range_z*(-1.+2.*i_z/(99.));
          TVector3 mfep(x,0.,z);
          double bz=s.B(mfep).z();
          h_xz->Fill(x*1.e2,z*1.e2,bz*1.e3);
        }
    }

  auto c=new TCanvas("c","c");
  SetStyle_TCanvas(c);
  c->Divide(2,2);
  c->cd(1);
  c->SetLogz();
  h_xy->Draw("colz");
  c->cd(2);
  h_xz->Draw("colz");

  auto f=new TFile(ofn_root.c_str(),"recreate");
  c->Write();
  f->Close();
  delete c;
  delete f;
  delete h_xy;
  delete h_xz;
}

void debug_shimcoil_0425()
{
  ShimCoil SC("Bz");
  auto sc=SC.Get1coil(0);
  double z_center=(sc.GetZtop()+sc.GetZbottom())/2.;
  int n_mfep=100;
  double X=.5;
  TVector3 mfep(X,0.,z_center);
  cout<<sc.B(mfep).x()<<"\t"<<sc.B(mfep).y()<<"\t"<<sc.B(mfep).z()<<endl;
  sc.InputTilt(0.,TMath::Pi(),0.);
  cout<<sc.B(mfep).x()<<"\t"<<sc.B(mfep).y()<<"\t"<<sc.B(mfep).z()<<endl;
  return;  
  ofstream of("debug.dat");
  for(int i_mfep=0;i_mfep<n_mfep;i_mfep++)
    {
      double x=X*(-1.+2.*i_mfep/(n_mfep-1.));
      TVector3 mfep(x,0.,z_center);
      double bx=sc.B(mfep).x();
      of<<x<<"\t"<<bx<<"\t";
      sc.InputTilt(0.,0.,TMath::Pi());
      bx=sc.B(mfep).x();
      of<<bx<<endl;
      sc.TakeDownFlag();
    }
}

void BxGeneration()
{
  ShimCoil rsc("Br");
  double I=1.e3;
  int n_mfep=1e3;
  rsc.MakeModes(2,"normal",I,"simple");
  ofstream of("bx.dat");
  double r=33.3e-2;
  double z=0.;
  for(int i_mfep=0;i_mfep<n_mfep;i_mfep++)
    {
      double theta=TMath::TwoPi()*(-.5+(double)i_mfep/n_mfep);
      double x=r*cos(theta);
      double y=r*sin(theta);
      TVector3 mfep(x,y,z);
      TVector3 RadialDirection(cos(theta),sin(theta),0.);
      TVector3 b=rsc.B(mfep);
      double br=b.Dot(RadialDirection);
      of<<theta<<"\t"<<br<<endl;
    }
  Draw probe(rsc,"what.root"); 
}

void ArcAndSegments()
{
  double r=1.;
  double z=0.;
  double I=1.e3;
  shim_coil arc(r,z,0.,TMath::Pi()/6.,TMath::Pi()/3.,I);
  BiotSavart segments;
  int n_segment=1e7;
  segments.arc(r,z,TMath::Pi()/6.,TMath::Pi()/3.,n_segment);
  int n_mfep=1e2;
  double z_mfep=-.5;
  double X=1.;
  ofstream of("DiagonalCut.dat");
  for(int i_mfep=0;i_mfep<n_mfep;i_mfep++)
    {
      double x=X*(-1.+2.*i_mfep/(n_mfep-1.));
      double y=x;
          TVector3 mfep(x,y,z_mfep);
          auto b_arc=arc.ResponseElement_arc(mfep,0.);
          b_arc*=I;
          auto b_segments=segments.ResponseElement(mfep);
          b_segments*=I;
          auto b_dif=b_arc-b_segments;
          of<<x<<"\t"<<abs(b_dif.x())<<"\t"<<abs(b_dif.y())<<"\t"<<abs(b_dif.z());
          of<<"\t"<<b_dif.Mag()<<endl;
    }
}


void plane_ShimCoil()
  {
    ShimCoil sc("Br");
    sc.ApplySameCurrent(1.e3);
    auto saddle=sc.Get1coil(0);
    ofstream of("saddle.dat");
    double z_draw=(saddle.GetZtop()+saddle.GetZbottom())/2.;
    int n_x=50;
    int n_y=50;
    double Range=1.;
    for(int i_x=0;i_x<n_x;i_x++)
      {
        for(int i_y=0;i_y<n_y;i_y++)
          {
            double x=Range*(-1.+2.*i_x/(n_x-1.));
            double y=Range*(-1.+2.*i_y/(n_y-1.));
            TVector3 mfep(x,y,z_draw);
            auto b=saddle.B(mfep);
            of<<x<<"\t"<<y<<"\t"<<b.x()<<"\t"<<b.y()<<endl;
          }
      }  
  }

void ThinkAboutBxGeneration()
{
  ShimCoil SC("Br");
  SC.ApplySameCurrent(1.e3);
  auto saddle_up=SC.Get1coil(0);
  auto saddle_low=SC.Get1coil(1);
  ofstream of("thinkBx.dat");
  int n_theta=6;
  double r=33.3e-2;
  for(int i_theta=0;i_theta<n_theta;i_theta++)
    {
      double theta=TMath::TwoPi()*i_theta/n_theta;
      double x=r*cos(theta);
      double y=r*sin(theta);
      TVector3 mfep(x,y,0.);
      TVector3 b=saddle_up.B(mfep);
      b+=saddle_low.B(mfep);
      of<<x<<"\t"<<y<<"\t"<<b.x()<<"\t"<<b.y()<<endl;
    }
  
}

void convert_segments()
{
  double r=1.;
  double z=0.;
  double I=1.e3;
  shim_coil arc(r,z,0.,TMath::Pi()/6.,TMath::Pi()/3.,I);
  int n_point=1e2;
  ofstream of("convert.dat");
  TVector3 mfep(0.5,0.5,-0.5);
  for(int i_point=0;i_point<n_point;i_point++)
    {
      cout<<i_point<<" now "<<endl;
      int n_div=round(1.e2*pow(2.,i_point));
      BiotSavart segments;
      segments.arc(r,z,TMath::Pi()/6.,TMath::Pi()/3.,n_div);
      auto b_seg=segments.ResponseElement(mfep)*I;
  auto b_arc=arc.ResponseElement_arc(mfep,z)*I;
      double b_dif=(b_seg+b_arc).Mag();
      of<<n_div<<"\t"<<b_dif<<endl;
      break;
    }  
}

void BxArrow()
{
  ShimCoil sc("Br");
  double I=1.e3;
  //  sc.MakeModes(6,"skew");
  sc.ApplySameCurrent(1.e3);
  ofstream of("0thmodeArrow.dat");
  int n=10;
  double range=.4;

  for(int i_x=0;i_x<n;i_x++)
    {
      for(int i_y=0;i_y<n;i_y++)
        {
          double x=range*(-1.+2.*(double)i_x/(n-1.));
          double y=range*(-1.+2.*(double)i_y/(n-1.));
          TVector3 mfep(x,y,0.);
          TVector3 b=sc.B(mfep);
          of<<x<<"\t"<<y<<"\t"<<b.x()<<"\t"<<b.y()<<endl;
        }      
    }
}



