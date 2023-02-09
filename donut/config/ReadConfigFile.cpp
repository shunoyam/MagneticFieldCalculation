
#include"../src/include.h"
using namespace std;


//muon storage region
double r_center_fiducial;
double r_width_fiducial;
double z_center_fiducial;
double z_width_fiducial;

//passive shimming by iron pieces
int number_shim_pocket;
int number_shim_tray;
double radius_shim_tray;
double length_shim_tray;
double Target_Magnetic_Field;
double diameter_of_DSS;
int number_of_MFEPs_on_DSS_theta;
int number_of_MFEPs_on_DSS_phi;
int number_of_MFEPs_radius;
int number_of_MFEPs_theta;
int number_of_MFEPs_z;
int number_of_iteration_shimming;
double volume_maximum_iron_shim;
double volume_minimum_iron_shim;
double unit_iron_shim_plate;
int TruncatedEigenmode;

//tolerance analysis
double tolerance_radial_MC10;
double tolerance_axial_MC10;
double tolerance_tilt_MC10;
double tolerance_radial_MC11;
double tolerance_tilt_MC11;
double tolerance_axial_MC11;
double tolerance_radial_MC20;
double tolerance_axial_MC20;
double tolerance_tilt_MC20;
double tolerance_radial_MC21;
double tolerance_axial_MC21;
double tolerance_tilt_MC21;
double tolerance_radial_MC30;
double tolerance_axial_MC30;
double tolerance_tilt_MC30;
double tolerance_radial_yoke;
double tolerance_axial_yoke;
double tolerance_tilt_yoke;

double tolerance_expansion_inner_radius;
double tolerance_expansion_outer_radius;
double tolerance_expansion_thickness;
int separation_random_tolerance;

double radius_inner_MC10;
double radius_outer_MC10;
double radius_inner_MC11;
double radius_outer_MC11;
double radius_inner_MC20;
double radius_outer_MC20;
double radius_inner_MC21;
double radius_outer_MC21;
double radius_inner_MC30;
double radius_outer_MC30;

double lower_height_MC10;
double upper_height_MC10;
double lower_height_MC11;
double upper_height_MC11;
double lower_height_MC20;
double upper_height_MC20;
double lower_height_MC21;
double upper_height_MC21;
double lower_height_MC30;
double upper_height_MC30;

int n_YOKE;
int n_MC10;
int n_MC11;
int n_MC20;
int n_MC21;
int n_MC30;
int n_YOKEandMainCoils;

double RequiredAzimuthallyAveragedUniformity;
double RequiredLocalUniformity;

int RotationalDivisionNumber;
double radius_ShimCoil_Bz;
double top_ShimCoil_Bz;
double bottom_ShimCoil_Bz;
double radius_ShimCoil_Br;
double top_ShimCoil_Br;
double bottom_ShimCoil_Br;
double threshold_PassRate_IronShim;
double kappa_CostFunction;
double upsilon_CostFunction;
double div_scan_target;


void ReadConfigFile(const char* path_InputConfigFile)
{
  ifstream ifn(path_InputConfigFile);
  if(ifn.fail())
    {
      puts("There is no such file");
      return;
    }
  string line;
  string VariableName;
  string InputParameter;
  vector<string> InputParameters;
  for(int row=0;getline(ifn,line);row++)
    {
      istringstream stream(line);
      for(int col=0;stream>>VariableName >>InputParameter;col++)
        {
          InputParameters.push_back(InputParameter);
        }
    }
  vector<string> IPs=InputParameters;
  r_center_fiducial=stod(IPs[0]);
  IPs.erase(IPs.begin());
  r_width_fiducial=stod(IPs[0]);
  IPs.erase(IPs.begin());
  z_center_fiducial=stod(IPs[0]);
  IPs.erase(IPs.begin());
  z_width_fiducial=stod(IPs[0]);
  IPs.erase(IPs.begin());

  //  passive shimming by iron pieces
  number_shim_pocket=stoi(IPs[0]);
  IPs.erase(IPs.begin());
  number_shim_tray=stoi(IPs[0]);
  IPs.erase(IPs.begin());

  radius_shim_tray=stod(IPs[0]);
  IPs.erase(IPs.begin());
  length_shim_tray=stod(IPs[0]);
  IPs.erase(IPs.begin());

  Target_Magnetic_Field=stod(IPs[0]);
  IPs.erase(IPs.begin());
  diameter_of_DSS=stod(IPs[0]);
  IPs.erase(IPs.begin());
  number_of_MFEPs_on_DSS_theta=stoi(IPs[0]);
  IPs.erase(IPs.begin());
  number_of_MFEPs_on_DSS_phi=stoi(IPs[0]);
  IPs.erase(IPs.begin());
  number_of_MFEPs_radius=stoi(IPs[0]);
  IPs.erase(IPs.begin());
  number_of_MFEPs_theta=stoi(IPs[0]);
  IPs.erase(IPs.begin());
  number_of_MFEPs_z=stoi(IPs[0]);
  IPs.erase(IPs.begin());
  number_of_iteration_shimming=stoi(IPs[0]);
  IPs.erase(IPs.begin());
  volume_maximum_iron_shim=stod(IPs[0]);
  IPs.erase(IPs.begin());
  volume_minimum_iron_shim=stod(IPs[0]);
  IPs.erase(IPs.begin());
  unit_iron_shim_plate=stod(IPs[0]);
  IPs.erase(IPs.begin());
  TruncatedEigenmode=stoi(IPs[0]);
  IPs.erase(IPs.begin());

  //  tolerance analysis
  tolerance_radial_MC10=stod(IPs[0]);
  IPs.erase(IPs.begin());
  tolerance_axial_MC10=stod(IPs[0]);
  IPs.erase(IPs.begin());
  tolerance_tilt_MC10=stod(IPs[0]);
  IPs.erase(IPs.begin());
  tolerance_radial_MC11=stod(IPs[0]);
  IPs.erase(IPs.begin());
  tolerance_tilt_MC11=stod(IPs[0]);
  IPs.erase(IPs.begin());
  tolerance_axial_MC11=stod(IPs[0]);
  IPs.erase(IPs.begin());
  tolerance_radial_MC20=stod(IPs[0]);
  IPs.erase(IPs.begin());
  tolerance_axial_MC20=stod(IPs[0]);
  IPs.erase(IPs.begin());
  tolerance_tilt_MC20=stod(IPs[0]);
  IPs.erase(IPs.begin());
  tolerance_radial_MC21=stod(IPs[0]);
  IPs.erase(IPs.begin());
  tolerance_axial_MC21=stod(IPs[0]);
  IPs.erase(IPs.begin());
  tolerance_tilt_MC21=stod(IPs[0]);
  IPs.erase(IPs.begin());
  tolerance_radial_MC30=stod(IPs[0]);
  IPs.erase(IPs.begin());
  tolerance_axial_MC30=stod(IPs[0]);
  IPs.erase(IPs.begin());
  tolerance_tilt_MC30=stod(IPs[0]);
  IPs.erase(IPs.begin());
  tolerance_radial_yoke=stod(IPs[0]);
  IPs.erase(IPs.begin());
  tolerance_axial_yoke=stod(IPs[0]);
  IPs.erase(IPs.begin());
  tolerance_tilt_yoke=stod(IPs[0]);
  IPs.erase(IPs.begin());

  tolerance_expansion_inner_radius=stod(IPs[0]);
  IPs.erase(IPs.begin());
  tolerance_expansion_outer_radius=stod(IPs[0]);
  IPs.erase(IPs.begin());
  tolerance_expansion_thickness=stod(IPs[0]);
  IPs.erase(IPs.begin());
  separation_random_tolerance=stoi(IPs[0]);
  IPs.erase(IPs.begin());

  radius_inner_MC10=stod(IPs[0]);
  IPs.erase(IPs.begin());
  radius_outer_MC10=stod(IPs[0]);
  IPs.erase(IPs.begin());
  radius_inner_MC11=stod(IPs[0]);
  IPs.erase(IPs.begin());
  radius_outer_MC11=stod(IPs[0]);
  IPs.erase(IPs.begin());
  radius_inner_MC20=stod(IPs[0]);
  IPs.erase(IPs.begin());
  radius_outer_MC20=stod(IPs[0]);
  IPs.erase(IPs.begin());
  radius_inner_MC21=stod(IPs[0]);
  IPs.erase(IPs.begin());
  radius_outer_MC21=stod(IPs[0]);
  IPs.erase(IPs.begin());
  radius_inner_MC30=stod(IPs[0]);
  IPs.erase(IPs.begin());
  radius_outer_MC30=stod(IPs[0]);
  IPs.erase(IPs.begin());

  lower_height_MC10=stod(IPs[0]);
  IPs.erase(IPs.begin());
  upper_height_MC10=stod(IPs[0]);
  IPs.erase(IPs.begin());
  lower_height_MC11=stod(IPs[0]);
  IPs.erase(IPs.begin());
  upper_height_MC11=stod(IPs[0]);
  IPs.erase(IPs.begin());
  lower_height_MC20=stod(IPs[0]);
  IPs.erase(IPs.begin());
  upper_height_MC20=stod(IPs[0]);
  IPs.erase(IPs.begin());
  lower_height_MC21=stod(IPs[0]);
  IPs.erase(IPs.begin());
  upper_height_MC21=stod(IPs[0]);
  IPs.erase(IPs.begin());
  lower_height_MC30=stod(IPs[0]);
  IPs.erase(IPs.begin());
  upper_height_MC30=stod(IPs[0]);
  IPs.erase(IPs.begin());

  n_YOKE=stoi(IPs[0]);
  IPs.erase(IPs.begin());
  n_MC10=stoi(IPs[0]);
  IPs.erase(IPs.begin());
  n_MC11=stoi(IPs[0]);
  IPs.erase(IPs.begin());
  n_MC20=stoi(IPs[0]);
  IPs.erase(IPs.begin());
  n_MC21=stoi(IPs[0]);
  IPs.erase(IPs.begin());
  n_MC30=stoi(IPs[0]);
  IPs.erase(IPs.begin());  
  n_YOKEandMainCoils=stoi(IPs[0]);
  IPs.erase(IPs.begin());
  RequiredAzimuthallyAveragedUniformity=stod(IPs[0]);
  IPs.erase(IPs.begin());
  RequiredLocalUniformity=stod(IPs[0]);
  IPs.erase(IPs.begin());
  RotationalDivisionNumber=stoi(IPs[0]);
  IPs.erase(IPs.begin());
  radius_ShimCoil_Bz=stod(IPs[0]);
  IPs.erase(IPs.begin());
  top_ShimCoil_Bz=stod(IPs[0]);
  IPs.erase(IPs.begin());
  bottom_ShimCoil_Bz=stod(IPs[0]);
  IPs.erase(IPs.begin());
  radius_ShimCoil_Br=stod(IPs[0]);
  IPs.erase(IPs.begin());
  top_ShimCoil_Br=stod(IPs[0]);
  IPs.erase(IPs.begin());
  bottom_ShimCoil_Br=stod(IPs[0]);
  IPs.erase(IPs.begin());
  threshold_PassRate_IronShim=stod(IPs[0]);
  IPs.erase(IPs.begin());
  kappa_CostFunction=stod(IPs[0]);
  IPs.erase(IPs.begin());
  upsilon_CostFunction=stod(IPs[0]);
  IPs.erase(IPs.begin());
  div_scan_target=stod(IPs[0]);
  IPs.erase(IPs.begin());

  if(!(IPs.empty()))
    {
      int RemainedElements=IPs.size();
      cout<<"\033[31m"<< RemainedElements;
      cout<<" global variable(s) have not read yet.\033[m"<<endl;
      puts("Something is wrong...");
    }
}












