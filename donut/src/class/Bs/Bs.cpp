



#include"../../include.h"
using namespace std;
using namespace ROOT::Math;

Bs::Bs()
{
  
}

Bs::Bs(string shape)
{
  if(shape=="donut")
    {
      Bs::SetOnDonut();
    }
  if(shape=="bale")
    {
      Bs::SetOnBale();
    }
}

Bs::Bs(string ifn_dat,string unit_pos,string unit_B)
{
  ifstream InputFile(ifn_dat.c_str());
  string line;
  double x,y,z,bx,by,bz;
  int line_legend=7;
  for(int i_line=0;getline(InputFile,line);i_line++)
    {
      if(i_line<line_legend)
        {
          continue;
        }
      istringstream stream(line);
      stream>>x>>y>>z>>bx>>by>>bz;
      TVector3 pos(x,y,z);
      TVector3 b(bx,by,bz);
      if(unit_pos=="cm")
        {
          pos*=1.e-2;
        }
      if(unit_B=="gauss")
        {
          b*=1.e-4;
        }
      Bs::push_back(pos,b);
    }
}

Bs::operator vector<Bfield>()
{
  return this->_B;
}

int Bs::MFEPs()
{
  int _MFEPs=this->_B.size();
  return _MFEPs;
}

int Bs::m1()
{
  return this->mesh1;  
}

int Bs::m2()
{
  return this->mesh2;
}

double Bs::r_center()
{
  return this->_r_center;
}

double Bs::r_width()
{
  return this->_r_width;
}

double Bs::z_center()
{
  return this->_z_center;
}

double Bs::z_width()
{
  return this->_z_width;
}

double Bs::min(char component)
{
  if(component=='z')
    {
      return this->_min_Bz;
    }
  if(component=='r')
    {
      return this->_min_Br;
    }
  if(component=='x')
    {
      return this->_min_Bx;
    }
  return 0.;
}

double Bs::max(char component)
{
  if(component=='z')
    {
      return this->_max_Bz;
    }
  if(component=='r')
    {
      return this->_max_Br;
    }
  if(component=='x')
    {
      return this->_max_Bx;
    }
  return 0.;
}

Bfield Bs::at(int index)
{
  int Bs_size=this->_B.size();
  if(Bs_size<index)
    {
      cout<<"error :Check the size of Bs"<<endl;
      abort;
    }
  Bfield __B= this->_B[index];
  return __B;
}

void Bs::push_back(TVector3 pos,TVector3 magneticfield,double phi)
{
  Bfield B_new;
  B_new.setB(magneticfield);
  B_new.setMFEP(pos);
  B_new.settheta(phi);
  this->_B.push_back(B_new);
}

void Bs::setUniform(double Bz_target)
{
  TVector3 B_TG(0.,0.,Bz_target);
  for(int i_MFEP=0;i_MFEP<this->_B.size();i_MFEP++)
    {
      this->_B[i_MFEP].setB(B_TG);
    }
}

void Bs::setonDSS(double diameter_DSS,int n_theta_MFEP,int n_phi_MFEP,double Bz_target)
{
  double diameter=diameter_DSS;//(m)
  double radius=diameter/2.;
  TVector3 mfep;
  double theta,phi;
  Bfield B_tmp;
  for(int i_theta_MFEP=0;i_theta_MFEP<n_theta_MFEP;i_theta_MFEP++){
    theta=TMath::Pi()*i_theta_MFEP/(n_theta_MFEP-1.);
    for(int i_phi_MFEP=0;i_phi_MFEP<n_phi_MFEP;i_phi_MFEP++)
      {
        phi=2*TMath::Pi()*i_phi_MFEP/n_phi_MFEP;
        mfep.SetMagThetaPhi(radius,theta,phi);
        B_tmp.setMFEP(mfep);
        this->_B.push_back(B_tmp);
      }
  }
  setUniform(Bz_target);
}

double Bs::uniformity()
{
  vector<Bfield> B_TG=this->_B;
  vector<double> Bz;
  for(Bfield B:B_TG)
    {
      Bz.push_back(B.Bz());
    }
  double Bz_min=*min_element(Bz.begin(),Bz.end());
  double Bz_max=*max_element(Bz.begin(),Bz.end());
  this->_min_Bz=Bz_min;
  this->_max_Bz=Bz_max;
  double average=Average(Bz);
  double peak2peak=P2P(Bz);
  double uni=peak2peak/average;
  return uni;
}

double Bs::average()
{
  vector<Bfield> B_TG=this->_B;
  vector<double> Bz;
  for(Bfield B:B_TG)
    {
      Bz.push_back(B.Bz());
    }
  double average=Average(Bz);
  return average;
}


void Bs::subtract(cb coils)
{
  int n_MFEP=this->_B.size();
  if(n_MFEP==0)
    {
      puts("MFEP has been not set yet.");
      return;
    }
  TVector3 tempB_coil,B_TG_new,mfep;
  vector<double> bz_vec,br_vec;
  for(int i_MFEP=0;i_MFEP<n_MFEP;i_MFEP++)
    {
      mfep=this->_B[i_MFEP].MFEP();
      tempB_coil=coils.B(mfep);
      B_TG_new=this->_B[i_MFEP].B()-tempB_coil;
      this->_B[i_MFEP].setB(B_TG_new);
      bz_vec.push_back(B_TG_new.z());
      br_vec.push_back(this->_B[i_MFEP].Br());
    }
  peak2peakSubstitution();
}

void Bs::addBs(Bs Bs_added,Bs Bs_add,double coefficient_added,double coefficient_add)
{
  Bs::clear();
  int size_added=Bs_added.MFEPs();
  int size_add=Bs_add.MFEPs();
  Bs Bs_new;
  TVector3 B_new,B_add,B_added;
  if(size_add!=size_added)
    {
      cout<<"cannot operate addition to objects with defferent sizes"<<endl;
      return;
    }
  //It costs much time to check all MFEPs are exactly same.

  for(int i_mfep=0;i_mfep<size_add;i_mfep++)
    {
      B_added=((vector<Bfield>)Bs_added)[i_mfep].B();
      B_add=((vector<Bfield>)Bs_add)[i_mfep].B();
      B_new=B_add*coefficient_add+B_added*coefficient_added;
      this->_B[i_mfep].setB(B_new);
    }
  peak2peakSubstitution(); 
}

void Bs::setonXZ(double r_center,double r_width,double z_center,double z_width,
                 double phi,int mesh_r,int mesh_z)
{
  this->_r_center=r_center;
  this->_r_width=r_width;
  this->_z_center=z_center;
  this->_z_width=z_width;
  int mfep_r=mesh_r;
  int mfep_z=mesh_z;
  this->mesh1=mfep_r;
  this->mesh2=mfep_z;
  double x,y,z,r;
  TVector3 pos;
  Bfield _B;
  for(int i_mfep_z=0;i_mfep_z<mfep_z;i_mfep_z++)
    {
      z=z_center-z_width+z_width*2.*i_mfep_z/(mfep_z-1.);
      for(int i_mfep_r=0;i_mfep_r<mfep_r;i_mfep_r++)
        {
          r=r_center-r_width+r_width*2.*i_mfep_r/(mfep_r-1.);
          x=r*cos(phi);
          y=r*sin(phi);
          pos.SetXYZ(x,y,z);
          _B.setMFEP(pos);
          this->_B.push_back(_B);
        }
    }
}

void Bs::setonXY(double r_center,double r_width)
{
  int r_mfep=50;
  this->mesh1=r_mfep;
  this->mesh2=r_mfep;
  this->_r_center=r_center;
  this->_r_width=r_width;
  double x,y;
  double buffer=1.;
  TVector3 pos;
  Bfield _B;
  for(int i_mfep_x=0;i_mfep_x<r_mfep;i_mfep_x++)
    {
      x=-(r_center+r_width)+2.*(r_center+r_width)*i_mfep_x/(r_mfep-1.);
      x*=buffer;
      for(int i_mfep_y=0;i_mfep_y<r_mfep;i_mfep_y++)
        {
          y=-(r_center+r_width)+2.*(r_center+r_width)*i_mfep_y/(r_mfep-1.);
          y*=buffer;
          pos.SetXYZ(x,y,0.);
          _B.setMFEP(pos);
          this->_B.push_back(_B);
        }
    }
}

void Bs::setonCS(double r_center,double z_width,
                 int theta_mfep,int z_mfep,
                 double Bz)
{
  this->mesh1=theta_mfep+1;// +1 is like a problem of planting trees
  this->mesh2=z_mfep;
  this->_r_center=r_center;
  this->_z_center=0.;
  this->_z_width=z_width;
  double z,theta;
  double x,y;
  TVector3 pos;
  Bfield _B;
  TVector3 B(0.,0.,Bz);
  for(int i_mfep_z=0;i_mfep_z<z_mfep;i_mfep_z++)
    {
      z=-z_width+2.*z_width*i_mfep_z/(z_mfep-1.);
      for(int i_mfep_theta=0;i_mfep_theta<theta_mfep;i_mfep_theta++)
        {
          theta=TMath::Pi()*(-1.+2.*i_mfep_theta/theta_mfep);
          x=r_center*cos(theta);
          y=r_center*sin(theta);
          pos.SetXYZ(x,y,z);
          _B.setMFEP(pos);
          _B.settheta(theta);
          _B.setB(B);
          this->_B.push_back(_B);
        }
    }
}

void Bs::peak2peakSubstitution()
{
  vector<double> Br_vec,Bz_vec,Bx_vec;
  for(int i_MFEP=0;i_MFEP< this->_B.size();i_MFEP++)
    {
      double Br=this->_B[i_MFEP].Br();
      double Bz=this->_B[i_MFEP].Bz();
      double Bx=this->_B[i_MFEP].B().x();
      Br_vec.push_back(Br);
      Bz_vec.push_back(Bz);
      Bx_vec.push_back(Bx);
    }
  double Bz_min=*min_element(Bz_vec.begin(),Bz_vec.end());
  double Bz_max=*max_element(Bz_vec.begin(),Bz_vec.end());
  this->_min_Bz=Bz_min;
  this->_max_Bz=Bz_max;
  double Br_min=*min_element(Br_vec.begin(),Br_vec.end());
  double Br_max=*max_element(Br_vec.begin(),Br_vec.end());
  this->_min_Br=Br_min;
  this->_max_Br=Br_max;
  double Bx_min=*min_element(Bx_vec.begin(),Bx_vec.end());
  double Bx_max=*max_element(Bx_vec.begin(),Bx_vec.end());
  this->_min_Bx=Bx_min;
  this->_max_Bx=Bx_max;
}

void Bs::add_coil(cb coils,double factor)
{
  double flux_add,flux_old,rho;
  TVector3 B_add,B_old,B_new,pos;
  for(int i_MFEP=0;i_MFEP< this->_B.size();i_MFEP++)
    {
      pos=this->_B[i_MFEP].MFEP();
      flux_old=this->_B[i_MFEP].flux();
      flux_add=coils.Flux(pos);
      B_add=coils.B(pos);
      B_add*=factor;
      B_old=this->_B[i_MFEP].B();
      this->_B[i_MFEP].setflux(flux_add+flux_old);
      B_new=B_add+B_old;
      this->_B[i_MFEP].setB(B_new);
    }
  Bs::peak2peakSubstitution();
}

void Bs::add_coil(vector<cb> CBs,double factor)
{
  for(cb coil:CBs)
    {
      Bs::add_coil(coil,factor);
    }
}

void Bs::Add_MM(mms mms)
{
  TVector3 B_add,B_old,B_new,pos;
  int n_MFEP=this->_B.size();
  for(int i_MFEP=0;i_MFEP<n_MFEP;i_MFEP++)
    {
      pos=this->_B[i_MFEP].MFEP();
      B_add=mms.B(pos);
      B_old=this->_B[i_MFEP].B();
      B_new=B_add+B_old;
      this->_B[i_MFEP].setB(B_new);
    }
  peak2peakSubstitution();
}

void Bs::Add_MM_RM(Eigen::MatrixXd ResponseMatrix,mms mms)
{
  int n_MFEP=ResponseMatrix.rows();
  auto mms_eigen=mms.mms_eigen();
  Eigen::VectorXd B_eigen(n_MFEP);
  B_eigen=ResponseMatrix*mms_eigen;
  Bs::Eigen_Bs(B_eigen);
}

void Bs::clear()
{
  TVector3 NullVector;
  for(int i_MFEP=0;i_MFEP< this->_B.size();i_MFEP++)
    {
      this->_B[i_MFEP].setflux(0.);
      this->_B[i_MFEP].setB(NullVector);
    }
  this->_min_Bz=0.;
  this->_max_Bz=0.;
  this->_min_Br=0.;
  this->_max_Br=0.;
}

 Eigen::VectorXd Bs::Bs_eigen(string element)
{
  int n_MFEP=this->_B.size();
  Eigen::VectorXd B_eigen(n_MFEP);
  for(int i_MFEP=0;i_MFEP<n_MFEP;i_MFEP++)
    {
      TVector3 B=this->_B[i_MFEP].B();
      if(element=="Bx")
        {
          B_eigen(i_MFEP)=B.x();
        }
      if(element=="Bz")
        {
          B_eigen(i_MFEP)=B.z();
        }
      if(element=="B")
        {
          B_eigen(i_MFEP)=B.Mag()*(B.z()/abs(B.z()));
        }
      if(element=="Br")
        {
          TVector3 MFEP=this->_B[i_MFEP].MFEP();
          TVector3 RadialDirection(MFEP.x(),MFEP.y(),0.);
          double br=B.Dot(RadialDirection);
          B_eigen(i_MFEP)=br;
        }
    }
  return B_eigen;
}

void Bs::Eigen_Bs(Eigen::VectorXd B_eigen)
{
  double Bz;
  int n_Eigen=B_eigen.rows();
  int n_Bs=this->_B.size();
  if(n_Eigen!=n_Bs)
    {
      puts("size of Eigen::vector and Bs is different");
    }
  for(int i_MFEP=0;i_MFEP<n_Eigen;i_MFEP++)
    {
      Bz=B_eigen(i_MFEP);
      TVector3 B(0.,0.,Bz);
      this->_B[i_MFEP].setB(B);
    }
}

Bs::operator vector<double>()
{
  vector<double> _Bs_vec;
  double Bz;

  for(Bfield B:this->_B)
    {
      Bz=B.Bz();
      _Bs_vec.push_back(Bz);
    }
  return _Bs_vec;
}

void Bs::read(const char* path_to_MFEPs_data)
{
  ifstream ifn(path_to_MFEPs_data);
  if(ifn.fail())
    {
      puts("MFEP data was not found.");
      abort;
    }
  double x,y=0.,z;
  double Br,By=0.,Bz;
  string line;

  for(int row=0;getline(ifn,line);row++)
    {
      istringstream stream(line);
      for(int col=0;stream>>x >>z  >>Bz >>Br ;col++)
        {
          if(x/100<5.e-2)
            {
              continue;
            }
          TVector3 pos(x/100.,y/100.,z/100.);
          TVector3 B(Br/1.e4,By,Bz/1.e4);
          push_back(pos,B);
        }
    }
}

void Bs::SetOnDonut(double Bz)
{
  double r,z,theta,x,y;
  TVector3 mfep;
  TVector3 B(0.,0.,Bz);
  int r_mesh=number_of_MFEPs_radius;
  int z_mesh=number_of_MFEPs_z;
  int theta_mesh=number_of_MFEPs_theta;
  r=r_center_fiducial-r_width_fiducial;
  for(int out=0;out<2;out++)
    {
      for(int i_z_mesh=0;i_z_mesh<z_mesh;i_z_mesh++)
        {
          z=z_center_fiducial-z_width_fiducial+2.*z_width_fiducial*i_z_mesh/(z_mesh-1.);
          for(int i_theta_mesh=0;i_theta_mesh<theta_mesh;i_theta_mesh++)
            {
              theta=TMath::Pi()*(-1.+2.*i_theta_mesh/theta_mesh);
              x=r*cos(theta);
              y=r*sin(theta);
              TVector3 mfep(x,y,z);
              Bs::push_back(mfep,B);
            }
        }
      r+=2.*r_width_fiducial;
    }
  
  z=z_center_fiducial-z_width_fiducial;
  for(int top=0;top<2;top++)
    {
      for(int i_r_mesh=1;i_r_mesh<r_mesh-1;i_r_mesh++)
        {
          r=r_center_fiducial-r_width_fiducial+2.*r_width_fiducial*i_r_mesh/(r_mesh-1.);
          for(int i_theta_mesh=0;i_theta_mesh<theta_mesh;i_theta_mesh++)
            {
              theta=TMath::Pi()*(-1.+2.*i_theta_mesh/theta_mesh);
              x=r*cos(theta);
              y=r*sin(theta);
              TVector3 mfep(x,y,z);
              Bs::push_back(mfep,B);
            }
        }
      z+=2.*z_width_fiducial;
    }
}

void Bs::DirectionBlinding()
{
  for(Bfield &_b:this->_B)
    {
      _b.DirectionBlinding();
    }
}

void Bs::ChangeB(int i_mfep,TVector3 newB)
{
  int n_mfep=Bs::MFEPs();
  if(i_mfep>=n_mfep)
    {
      cout<<"1st argument "<<i_mfep<<"must be less than "<<n_mfep<<endl;
    }
  this->_B[i_mfep].setB(newB);
}

void Bs::out(const char* ofn_dat)
{
  ofstream OutputFile(ofn_dat);
  for(Bfield b:this->_B)
    {
      auto mfep=b.MFEP();
      auto B=b.B();
      OutputFile<<mfep.x()<<"\t"<<mfep.y()<<"\t"<<mfep.z()<<"\t";
      OutputFile<<B.x()<<"\t"<<B.y()<<"\t"<<B.z()<<endl;
    }
}

void Bs::OutOperaMap(string ofn_root)
{
  Bs donut("donut");
  int n_ShimmingPoint=donut.MFEPs();
  double r_center=33.3;//[cm]
  double r_width=2.;//[cm]
  double z_width=8.;//[cm]
  int n_r=30;
  int n_theta=50;
  int n_z=50;
  int n_wide=n_r*n_theta*n_z;
  vector<Bfield> B_ave(n_r*n_z);
  vector<double> variance(n_r*n_z,0.);
  int i_mfep=0;
  for(int i_r=0;i_r<n_r;i_r++)
    {
      for(int i_theta=0;i_theta<n_theta;i_theta++)
        {
          for(int i_z=0;i_z<n_z;i_z++)
            {
              i_mfep++;
              int i_mfep_ave=i_r*n_z+i_z;
              auto B=Bs::at(i_mfep+n_ShimmingPoint+1).B();
              auto pos=Bs::at(i_mfep+n_ShimmingPoint+1).MFEP();
              double dif=B.z()-3.;
              if(abs(dif)<1.e-3)
                {
                  TVector3 offset(0.,0.,3.);
                  B=B-offset;//[T]
                }
              variance[i_mfep_ave]+=B.z()*B.z();
              auto b_add=B*(1./(double)n_theta);
              double r=hypot(pos.x(),pos.y());
              pos.SetXYZ(r,0.,pos.z());
              B_ave[i_mfep_ave].setMFEP(pos);
              B_ave[i_mfep_ave].setB(B_ave[i_mfep_ave].B()+b_add);
            }
        }
    }
  auto h_wide=new TH2D("h_wide",";x [cm];z [cm]",
                       n_r,r_center-r_width*(1.+1./n_r),r_center+r_width*(1.+1./n_r),
                       n_z,-z_width*(1.+1./n_z),z_width*(1.+1./n_z));
  auto h_ripple=new TH2D("h_ripple",";x [cm];z [cm]",
                       n_r,r_center-r_width*(1.+1./n_r),r_center+r_width*(1.+1./n_r),
                       n_z,-z_width*(1.+1./n_z),z_width*(1.+1./n_z));
  SetStyle_TH2Ds(h_wide,h_ripple);
  for(int i_mfep_ave=0;i_mfep_ave<B_ave.size();i_mfep_ave++)
    {
      auto B=B_ave[i_mfep_ave];
      auto pos=B.MFEP();
      auto b=B.B();
      double x_cm=pos.x()*1.e2;
      double z_cm=pos.z()*1.e2;
      double bz=b.z();
      double dev=sqrt(variance[i_mfep_ave]/(double)n_theta -bz*bz);
      h_wide->Fill(x_cm,z_cm,bz);
      h_ripple->Fill(x_cm,z_cm,dev);
    }
  ArrangeRange(h_wide);
  ArrangeRange(h_ripple);
  auto c=new TCanvas("c","c");
  c->Divide(2,1);
  c->cd(1);
  h_wide->Draw("colz");
  c->cd(2);
  h_ripple->Draw("colz");
  auto f=new TFile(ofn_root.c_str(),"recreate");
  c->Write();
  f->Close();
  delete h_wide;
  delete h_ripple;
  delete c;
  delete f;  
}

void Bs::reverse()
{
  for(auto& b:this->_B)
    {
      b.setB(-b.B());
    }
}

void Bs::SetOnBale()
{
  double interval_r=r_width_fiducial;
  int n_theta=30;
  int n_z=10;
  double Z=z_width_fiducial;
  double interval_z=2.*Z/(n_z-1.);
  double r0=r_center_fiducial;
  vector<double> dr{-1.,0.,1.,-.5,.5};
  vector<double> dz{0.,0.,0.,1.,1.};
  int n_channel=5;
  for(int i_theta=0;i_theta<n_theta;i_theta++)
    {
      double theta=TMath::TwoPi()*(double)i_theta/n_theta;
      for(int i_z=0;i_z<n_z;i_z++)
        {
          double z0=Z*(-1.+2.*(i_z/(n_z-1.)));
          for(int i_channel=0;i_channel<n_channel;i_channel++)
            {
              double r=r0+dr[i_channel]*interval_r;
              double z=z0+dz[i_channel]*interval_z;
              TVector3 position(r*cos(theta),r*sin(theta),z);
              Bs::push_back(position,i_channel,theta);
            }
        }
    }
}

void Bs::push_back(TVector3 mfep,int channel,double theta,TVector3 B)
{
  Bfield B_new;
  B_new.setB(B);
  B_new.setMFEP(mfep);
  B_new.settheta(theta);
  B_new.SetChannel(channel);
  this->_B.push_back(B_new);  
}










