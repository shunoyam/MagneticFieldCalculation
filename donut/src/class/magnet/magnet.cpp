


#include"../../include.h"
using namespace std;

void magnet::ReadCBsData(const char* path_to_magnet_data)
{
  ifstream ifn(path_to_magnet_data);
  if(ifn.fail())
    {
      puts("Magnet data file cannot be opened.");
      abort;
    }
  
  double radius,height,current;
  coil coil_temp;
  string line;
  int number,number_CB=0;
  cb iron_yoke,MC10,MC11,MC20,MC21,MC30;

  for(int row=0;getline(ifn,line);++row)
    {
      istringstream stream(line);
      for(int col=0;stream>>number >>radius >>height >>current ;col++)
        {
          if(number==1)
            {
              number_CB++;
            }
          coil_temp.set(current,radius,height);
          if(number_CB==1)
            {
              iron_yoke.push_back(coil_temp);
            }
          if(number_CB==2)
            {
              MC10.push_back(coil_temp);
            }
          if(number_CB==3)
            {
              MC11.push_back(coil_temp);
            }
          if(number_CB==4)
            {
              MC20.push_back(coil_temp);
            }
          if(number_CB==5)
            {
              MC21.push_back(coil_temp);
            }
          if(number_CB==6)
            {
              MC30.push_back(coil_temp);
            }
        }
    }
  this->MainCoilsAndIronYoke.push_back(iron_yoke);
  this->MainCoilsAndIronYoke.push_back(MC10);
  this->MainCoilsAndIronYoke.push_back(MC11);
  this->MainCoilsAndIronYoke.push_back(MC20);
  this->MainCoilsAndIronYoke.push_back(MC21);
  this->MainCoilsAndIronYoke.push_back(MC30);
}

void magnet::ReadCBsData_sub()
{
  this->MainCoilsAndIronYoke[n_MC10].WriteDownSides(radius_inner_MC10,radius_outer_MC10,
                                                    lower_height_MC10,upper_height_MC10);
  this->MainCoilsAndIronYoke[n_MC11].WriteDownSides(radius_inner_MC11,radius_outer_MC11,
                                                    lower_height_MC11,upper_height_MC11);
  this->MainCoilsAndIronYoke[n_MC20].WriteDownSides(radius_inner_MC20,radius_outer_MC20,
                                                    lower_height_MC20,upper_height_MC20);
  this->MainCoilsAndIronYoke[n_MC21].WriteDownSides(radius_inner_MC21,radius_outer_MC21,
                                                    lower_height_MC21,upper_height_MC21);
  this->MainCoilsAndIronYoke[n_MC30].WriteDownSides(radius_inner_MC30,radius_outer_MC30,
                                                    lower_height_MC30,upper_height_MC30);
}

magnet::magnet()
{
  magnet::ReadCBsData();
  magnet::ReadCBsData_sub();
}

magnet::operator vector<cb>()
{
  return this->MainCoilsAndIronYoke;
}

cb magnet::CombinedCB()
{
  cb _CombinedCB;
  for(cb coils:this->MainCoilsAndIronYoke)
    {
      _CombinedCB.Push_Back(coils);
    }
  return _CombinedCB;
}

cb magnet::at(int i_cb)
{
  if(i_cb<0||n_YOKEandMainCoils<=i_cb)
    {
      cout<<"coil number"<<i_cb<<" is invalid"<<endl;
      abort;
    }
  return this->MainCoilsAndIronYoke[i_cb];
}
  
cb magnet::GetIronYoke()
{
  return this->MainCoilsAndIronYoke[n_YOKE];
}

cb magnet::GetMC10()
{
  return this->MainCoilsAndIronYoke[n_MC10];
}

cb magnet::GetMC11()
{
  return this->MainCoilsAndIronYoke[n_MC11];
}

cb magnet::GetMC20()
{
  return this->MainCoilsAndIronYoke[n_MC20];
}

cb magnet::GetMC21()
{
  return this->MainCoilsAndIronYoke[n_MC21];
}

cb magnet::GetMC30()
{
  return this->MainCoilsAndIronYoke[n_MC30];
}

void magnet::Perfect()
{
  for(cb& CB:this->MainCoilsAndIronYoke)
    {
      CB.perfect();
    }
}

void magnet::tilt(int i_coil,double theta_tilt,double phi_tilt)
{
  if(i_coil<0 || 5<i_coil)
    {
      cout<<"magnet::tilt(int i_coil,double theta_tilt,double phi_tilt)"<<endl;
      puts("1st argument,i_coil is invalid");
      abort;
    }
  this->MainCoilsAndIronYoke[i_coil].tilt(theta_tilt,phi_tilt);
}

void magnet::dis(int i_coil,TVector3 displacement)
{
  if(i_coil<0 || 5<i_coil)
    {
      cout<<"void magnet::dis(int i_coil,TVector3 displacement)"<<endl;
      puts("1st argument,i_coil is invalid");
      abort;
    }
  this->MainCoilsAndIronYoke[i_coil].dis(displacement);
}

void magnet::deformation(int i_coil,
                         double deformation_in,double deformation_out,
                         double deformation_thick)
{
  if(i_coil<1 || 5<i_coil)
    {
      cout<<"void magnet::deformation(int i_coil,double deformation_in,";
      cout<<"double deformation_out,double deformation_thick)"<<endl;
      puts("1st argument,i_coil is invalid");
      abort;
    }
  this->MainCoilsAndIronYoke[i_coil].deformation(deformation_in,deformation_out,
                                                 deformation_thick);
}

void magnet::InputTolerance(vector<double> theta_vec,
                            vector<double> phi_vec,
                            vector<TVector3> dis_vec,
                            vector<double> deformation_inner_vec,
                            vector<double> deformation_outer_vec,
                            vector<double> deformation_thickness_vec)
{
  double theta,phi;
  TVector3 displacement;
  double def_in,def_out,def_thi;
  for(int i_cb=0;i_cb<this->MainCoilsAndIronYoke.size();i_cb++)
    {
      theta=theta_vec[i_cb];
      phi=phi_vec[i_cb];
      displacement=dis_vec[i_cb];
      this->MainCoilsAndIronYoke[i_cb].dis(displacement);
      this->MainCoilsAndIronYoke[i_cb].tilt(theta,phi);
      if(i_cb)
        {
          def_in=deformation_inner_vec[i_cb-1];
          def_out=deformation_outer_vec[i_cb-1];
          def_thi=deformation_thickness_vec[i_cb-1];
          this->MainCoilsAndIronYoke[i_cb].deformation(def_in,def_out,def_thi);
        }
    }
}

void magnet::InputTolerances_manual(double ToleranceLevel)
{
  double factor=ToleranceLevel/.5e-3;
  //local uniformity worst case 897ppm
  magnet::dis(n_YOKE,TVector3(factor*0.0591503e-3,
                              factor*0.359568e-3,factor*0.233219e-3));
  magnet::tilt(n_YOKE,-factor*0.338002e-3,1.26722);

  magnet::dis(n_MC10,TVector3(factor*0.453815e-3,
                              factor*0.459717e-3,
                              -factor*0.389698e-3));
  magnet::deformation(n_MC10,factor*0.22477*1.e-3,
                      factor*0.216169e-3,
                      factor*0.320096*1.e-3);
  magnet::tilt(n_MC10,factor*0.441668e-3,1.1011);

  magnet::dis(n_MC11,TVector3(factor*0.0503205e-3,
                              factor*0.358001e-3,
                              factor*0.211484e-3));
  magnet::deformation(n_MC11,-factor*0.340354e-3,
                      -factor*0.119169e-3,factor*0.49887e-3);
  magnet::tilt(n_MC11,factor*0.394726e-3,4.22474);

  magnet::dis(n_MC20,TVector3(-factor*0.229526e-3,-factor*0.225345e-3,
                              -factor*0.372175e-3));
  magnet::deformation(n_MC20,factor*0.46824*1.e-3,
                      factor*0.391558e-3,
                      factor*0.233223e-3);
  magnet::tilt(n_MC20,-factor*0.385556e-3,4.60003);

  magnet::dis(n_MC21,TVector3(factor*0.352357e-3,
                              -factor*0.318229e-3,
                              factor*0.0242506e-3));
  magnet::deformation(n_MC21,factor*0.41433e-3,
                      -factor*0.346957e-3,
                      factor*0.345161e-3);
  magnet::tilt(n_MC21,factor*0.358926e-3,5.76073);

  magnet::dis(n_MC30,TVector3(-factor*0.321883e-3,
                              -factor*0.269148e-3,
                              -factor*0.288334e-3));
  magnet::deformation(n_MC30,factor*0.306333e-3,-factor*0.0566379e-3,
                      -factor*0.0348243e-3);
  magnet::tilt(n_MC30,-factor*0.15858e-3,0.409254);
}

magnet::~magnet()
{
  MainCoilsAndIronYoke.clear();
}

TVector3 magnet::B(TVector3 MFEP)
{
  TVector3 _B;
  for(cb CB:this->MainCoilsAndIronYoke)
    {
      _B+=CB.B(MFEP);
    }
  return _B;
}

double magnet::Bz(TVector3 MFEP)
{
  double _Bz=magnet::B(MFEP).z();
  return _Bz;
}

void magnet::deformation_elliptical(int i_coil,double deviation_radius,
                                    double phase_MajorAxis)
{
  magnet::OvalDeformation(i_coil,deviation_radius,phase_MajorAxis,2);
}

void magnet::deformation_benz(int i_coil,double deviation_radius,
                              double phase_MajorAxis)
{
  magnet::OvalDeformation(i_coil,deviation_radius,phase_MajorAxis,3);
}

void magnet::deformation_cross(int i_coil,double deviation_radius,
                               double phase_MajorAxis)
{
  magnet::OvalDeformation(i_coil,deviation_radius,phase_MajorAxis,4);
}

void magnet::OvalDeformation(int i_coil,double deviation_radius,double phase_MajorAxis,int mode)
{
  if(i_coil<0||5<i_coil)
    {
      cout<<"1st argument "<<i_coil<<"is invalid"<<endl;
      return;
    }
  this->MainCoilsAndIronYoke[i_coil].deformation_shape(mode,deviation_radius,
                                                       phase_MajorAxis);  
}

double magnet::TotalCurrent()
{
  double current=0.;
  for(int i_coil=1;i_coil<MainCoilsAndIronYoke.size();i_coil++)
    {
      double current_tmp=MainCoilsAndIronYoke[i_coil].TotalCurrent();
      current+=current_tmp;
    }
  return current;
}

double magnet::U_lc()
{
  return _U_lc;
}

double magnet::U_theta()
{
  return _U_theta;
}

void magnet::DrawBzOnCylinder(TCanvas* c,int i_trial)
{
  c->cd(i_trial+1);
  double r_c=33.3e-2;
  double z_w=5.e-2;
  int n_theta=30;
  int n_z=10;
  double z_max=z_w*(1.+1./(double)n_z)*1.e2;
  auto h=new TH2D(to_string(i_trial).c_str(),";;",
             n_theta,-TMath::Pi()*(double)n_theta/(n_theta-1.),
             TMath::Pi()*(double)n_theta/(n_theta-1.),
             n_z,-z_max,z_max);
  for(int i_theta=0;i_theta<n_theta;i_theta++)
    {
      for(int i_z=0;i_z<n_z;i_z++)
        {
          double theta=TMath::Pi()*(-1.+2.*(double)i_theta/(double)(n_theta-1.));
          double z=z_w*(-1.+2.*(double)i_z/(n_z-1.));
          TVector3 mfep(r_c*cos(theta),r_c*sin(theta),z);
          double bz=magnet::Bz(mfep);
          h->Fill(theta,z*1.e2,bz);
        }
    }
  h->Draw("colz");
  double bz_min=2.99983;
  double bz_max=3.00026;
  h->GetZaxis()->SetRangeUser(bz_min,bz_max);
  double LabelSize=.13;
  h->GetYaxis()->SetNdivisions(2);
  h->GetZaxis()->SetLabelSize(0);
  h->GetXaxis()->SetNdivisions(2);
  if(!i_trial)
    {
      auto ax=h->GetXaxis();
      ax->SetTitle("#theta [rad/#pi]");
      ax->SetLabelSize(LabelSize);
      ax->SetTitleSize(LabelSize);
      ax->CenterTitle();
      ax->ChangeLabel(1,-1,-1,-1,-1,-1,"-1");
      ax->ChangeLabel(-1,-1,-1,-1,-1,-1,"1");
      auto ay=h->GetYaxis();
      ay->SetLabelSize(LabelSize);
      ay->CenterTitle();
      ay->SetTitle("z [mm]");
      ay->SetTitleOffset(.3);
      ay->ChangeLabel(1,-1,-1,-1,-1,-1,"-50");
      ay->ChangeLabel(-1,-1,-1,-1,-1,-1,"50");
      ay->SetTitleSize(LabelSize);

      auto az=h->GetZaxis();
      az->SetNdivisions(1);
      az->SetLabelSize(.1);
      az->ChangeLabel(1,-1,-1,-1,-1,-1,"3 T - 170 #muT");
      az->ChangeLabel(-1,-1,-1,-1,-1,-1,"3 T + 260 #muT");
    }
  else
    {
      h->GetXaxis()->SetLabelSize(0);
      h->GetYaxis()->SetLabelSize(0);
      h->GetZaxis()->SetLabelSize(0);
      c->Update();
      TPaletteAxis* p=(TPaletteAxis*)h->GetListOfFunctions()->FindObject("palette");
      p->SetX1NDC(8);
      c->Update();
      p->SetX2NDC(9);
      p->SetY1NDC(9);
      p->SetY2NDC(0.8);
    }
  TPaveStats* st=(TPaveStats*)h->FindObject("stats");
  h->SetStats(0);
}


void magnet::InputRandomTolerances(TRandom2* random)
{
  vector<double> theta_vec,phi_vec;
  vector<TVector3> dis_vec;
  vector<XYZVector> dis_vec_XYZ(6);
  vector<double> def_in_vec,def_out_vec,def_thi_vec;
  theta_vec=get_theta_vec(random);
  phi_vec=get_phi_vec(random);
  dis_vec=get_dis_vec(random);
  def_in_vec=get_deformation_inner_vec(random);
  def_out_vec=get_deformation_outer_vec(random);
  def_thi_vec=get_deformation_thickness_vec(random);
  magnet::InputTolerance(theta_vec,phi_vec,dis_vec,def_in_vec,
                         def_out_vec,def_thi_vec);
}











