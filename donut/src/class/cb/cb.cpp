


#include"../../include.h"

using namespace std;

cb::cb()
{
  
}

cb::cb(vector<coil> coils)
{
  cb::set(coils);
}

void cb::RampUp_cb(double _RampUpRate)
{
  this->RampUpRate=_RampUpRate;
  for(coil& flc:this->_coils)
    {
      flc.RampUp_coil(_RampUpRate);
    }
}

void cb::set(vector<coil> coils)
{
  this->_coils=coils;
}

void cb::WriteDownSides(double r_in,double r_out,
                        double z_lower,double z_upper)
{
  this->_center_r=(r_out+r_in)/2.;
  this->_center_z=(z_upper+z_lower)/2.;  
  this->_r_in=r_in;
  this->_r_out=r_out;
  this->_z_upper=z_upper;
  this->_z_lower=z_lower;
}

void cb::SetOnMesh(double r_in,double r_out,double height,int mesh_r,int mesh_z,
                   double TotalCurrent,double center_z)
{
  if(mesh_r<1 || mesh_z<1 )
    {
      cout<<"Number of meshes is invalid."<<endl;
      cout<<"number of radial meshes:"<<mesh_r<<endl;
      cout<<"number of axial meshes:"<<mesh_z<<endl;
      return;
    }
  if(height<0.)
    {
      cout<<"error!"<<endl;
      cout<<"height of coil needs to be positive"<<endl;
      return;
    }
  double IndividualCurrent=TotalCurrent/(mesh_r*mesh_z);
  coil coil_tmp;
  double r,z;
  double div_r=(r_out-r_in)/mesh_r;
  double div_z=height/mesh_z;
  double z_low=center_z-height/2.;
  for(int mesh_r_i=0;mesh_r_i<mesh_r;mesh_r_i++)
    {
      r=r_in+div_r*(mesh_r_i+0.5);
      for(int mesh_z_i=0;mesh_z_i<mesh_z;mesh_z_i++)
        {
          z=z_low+div_z*(mesh_z_i+0.5);
          coil_tmp.set(IndividualCurrent,r,z);
          cb::push_back(coil_tmp);
        }      
    }
}

void cb::push_back(coil coil)
{
  this->_coils.push_back(coil);
}

void cb::Push_Back(cb coils)
{
  vector<coil> coils_vec=coils.coil_vec();
  int n_coil=coils_vec.size();
  for(int i_coil=0;i_coil<n_coil;i_coil++)
    {
      this->push_back(coils_vec[i_coil]);
    }
}

coil cb::at(int index)
{
  coil coil_index=this->_coils[index];
  return coil_index;
}

vector<coil> cb::coil_vec()
{
  return this->_coils;
}

TVector3 cb::B(TVector3 MFEP)
{
  TVector3 _B;
  for(coil flc:this->_coils)
    {
      _B+=flc.b(MFEP);
    }
  return _B;
}

double cb::Bz(TVector3 MFEP)
{
  double _Bz=cb::B(MFEP).z();
  return _Bz;
}

double cb::Flux(TVector3 MFEP)
{
  vector<coil> coils=this->_coils;
  double _flux_coil=0.;
  for(int i_coil=0;i_coil<coils.size();i_coil++)
    {
      _flux_coil+=coils[i_coil].flux(MFEP);
    }
  return _flux_coil;
}

double cb::MMF()
{
  double magnetomotiveforce=0.;
  double current;
  vector<coil> coils=this->_coils;
  for(int i_flc=0;i_flc<coils.size();i_flc++)
    {
      current=coils[i_flc].I();
      magnetomotiveforce+=abs(current);
    }
  return magnetomotiveforce;
}

void cb::tilt(double theta,double phi)
{
  for(coil& flc:this->_coils)
    {
      flc.settilt(theta,phi);
    }
}

void cb::dis(TVector3 displacement)
{
  for(int i_flc=0;i_flc< this->_coils.size();i_flc++)
    {
      this->_coils[i_flc].setdis(displacement);
    }
}

void cb::read(const char* path_to_magnet_data)
{
  vector<coil> CoilBlock;
  ifstream ifn(path_to_magnet_data);
  int number;
  double radius,height,current;
  coil coil_temp;
  string line;
  for(int row=0;getline(ifn,line);++row)
    {
      istringstream stream(line);
      for(int col=0;stream>>number >>radius >>height >>current ;++col)
        {
          coil_temp.set(current,radius,height);
          CoilBlock.push_back(coil_temp);
        }
    }
  this->_coils=CoilBlock;
}

void cb::read_rz(const char* path_to_magnet_data)
{
  vector<coil> CoilBlock;
  ifstream ifn(path_to_magnet_data);
  double radius,height,current=0.;
  coil coil_temp;
  string line;
  for(int row=0;getline(ifn,line);++row)
    {
      istringstream stream(line);
      for(int col=0;stream>>radius >>height ;++col)
        {
          coil_temp.set(current,radius,height);
          CoilBlock.push_back(coil_temp);
        }
    }
  this->_coils=CoilBlock;
}

void cb::draw(const char* outputfilename_png)
{
  vector<coil> _coil=this->_coils;
  int n_coil=_coil.size();
  ofstream f("temp_I.dat");
  for(int i_coil=0;i_coil<n_coil;i_coil++)
    {
      f<<fixed<<setprecision(15)<<_coil[i_coil].z()<<"\t"<<_coil[i_coil].I()<<endl;
    }
  FILE *gp;
  gp=popen("gnuplot","w");
  fprintf(gp,"set title 'I distribution'\n");
  fprintf(gp,"set xl 'z(m)'\n");
  fprintf(gp,"set yl 'I(kA)'\n");
  fprintf(gp,"set title offset -3,0\n");
  fprintf(gp,"set title font 'Arial,20'\n");
  fprintf(gp,"set xl font 'Arial,20'\n");
  fprintf(gp,"set yl font 'Arial,20'\n");
  fprintf(gp,"set tics font 'Arial,20'\n");
  fprintf(gp,"set key font 'Arial,10'\n");
  fprintf(gp,"p 'temp_I.dat' u 1:($2/1000) pt 7 lc rgb 'red' title '' \n");
  fprintf(gp," set terminal pngcairo \n");
  fprintf(gp," set output '%s'\n",outputfilename_png);
  fprintf(gp,"rep\n");
  pclose(gp);
}

void cb::deformation(double deformation_inner,double deformation_outer,
                     double deformation_thickness)
{
  double ratio_expansion_inner,ratio_expansion_outer,ratio_expansion_thickness;
  double delta_r_in,delta_r_out;
  double delta_r,delta_z;
  double r_in=this->_r_in;
  double r_out=this->_r_out;
  double z_upper=this->_z_upper;
  double z_center=this->_center_z;
  double r_flc,z_flc;
  for(coil& flc: this->_coils)
    {
      r_flc=flc.r();
      z_flc=flc.z();
      delta_r_in=(r_out-r_flc)*deformation_inner/(r_out-r_in);
      delta_r_out=(r_flc-r_in)*deformation_outer/(r_out-r_in);
      delta_r=delta_r_in+delta_r_out;
      delta_z=(z_flc-z_center)*deformation_thickness/(z_upper-z_center);
      flc.SetDeformation(delta_r,delta_z);
    }
}

void cb::off_deformation()
{
  for(coil& flc: this->_coils)
    {
      flc.switchoff_deformation();
    }
}

void cb::off_displacement()
{
  for(coil& flc: this->_coils)
    {
      flc.switchoff_displacement();
    }
}

void cb::off_tilt()
{
  for(coil& flc: this->_coils)
    {
      flc.switchoff_tilt();
    }
}

void cb::off_deformation_shape()
{
  for(coil& flc: this->_coils)
    {
      flc.switchoff_undulation();
    }
}

void cb::off_RampUp()
{
  this->RampUpRate=1.;
  for(coil& flc: this->_coils)
    {
      flc.switchoff_RampUp();
    }
}

void cb::off()
{
  cb::off_deformation();
  cb::off_displacement();
  cb::off_tilt();
  cb::off_deformation_shape();
  cb::off_RampUp();
}

void cb::perfect()
{
  cb::off();
}

void cb::SetOnGaussNodes(double center_r,double width_r,int nodes_r,
                         double center_z,double width_z,int nodes_z,double CurrentDensity)
{
  int nodes=nodes_r*nodes_z;
  double TotalCurrent=CurrentDensity*2.*width_r*2.*width_z;
  double current,r,z,weight_r,weight_z,node_r,node_z;
  Legendre leg_r(nodes_r);
  Legendre leg_z(nodes_z);
  vector<double> weights_r_vec=leg_r.GetWeights();
  vector<double> weights_z_vec=leg_z.GetWeights();
  vector<double> nodes_r_vec=leg_r.GetNodes();
  vector<double> nodes_z_vec=leg_z.GetNodes();
  for(int i_node_r=0;i_node_r<nodes_r;i_node_r++)
    {
      for(int i_node_z=0;i_node_z<nodes_z;i_node_z++)
        {
          weight_r=weights_r_vec[i_node_r];
          weight_z=weights_z_vec[i_node_z];
          node_r=nodes_r_vec[i_node_r];
          node_z=nodes_z_vec[i_node_z];
          current=TotalCurrent*weight_r*weight_z/4.;
          r=center_r+node_r*width_r;
          z=center_z+node_z*width_z;
          coil flc(current,r,z);
          cb::push_back(flc);
        }      
    }  
}

void cb::deformation_shape(int n_bump,double deviation_radius,double phase_major)
{
  if(deviation_radius<0.)
    {
      cout<<"1st argument is invalid"<<endl;
      return;
    }
  for(coil &flc:this->_coils)
    {
      flc.deformation_undulation(n_bump,deviation_radius,phase_major);
    }
}

double cb::z_lower()
{
  return this->_z_lower;
}

double cb::TotalCurrent()
{
  double current=0.;
  for(coil coil: _coils)
    {
      current+=coil.I();
    }
  return current;
}
























