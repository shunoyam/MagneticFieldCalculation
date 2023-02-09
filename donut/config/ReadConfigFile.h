
#pragma once

#include"../src/include.h"
using namespace std;


//muon storage region
extern double r_center_fiducial;
extern double r_width_fiducial;
extern double z_center_fiducial;
extern double z_width_fiducial;

//passive shimming by iron pieces
extern int number_shim_pocket;
extern int number_shim_tray;
extern double radius_shim_tray;
extern double length_shim_tray;
extern double Target_Magnetic_Field;
extern double diameter_of_DSS;
extern int number_of_MFEPs_on_DSS_theta;
extern int number_of_MFEPs_on_DSS_phi;
extern int number_of_MFEPs_radius;
extern int number_of_MFEPs_theta;
extern int number_of_MFEPs_z;
extern int number_of_iteration_shimming;
extern double volume_maximum_iron_shim;
extern double volume_minimum_iron_shim;
extern double unit_iron_shim_plate;
extern int TruncatedEigenmode;

//tolerance analysis
extern double tolerance_radial_MC10;
extern double tolerance_axial_MC10;
extern double tolerance_tilt_MC10;
extern double tolerance_radial_MC11;
extern double tolerance_tilt_MC11;
extern double tolerance_axial_MC11;
extern double tolerance_radial_MC20;
extern double tolerance_axial_MC20;
extern double tolerance_tilt_MC20;
extern double tolerance_radial_MC21;
extern double tolerance_axial_MC21;
extern double tolerance_tilt_MC21;
extern double tolerance_radial_MC30;
extern double tolerance_axial_MC30;
extern double tolerance_tilt_MC30;
extern double tolerance_radial_yoke;
extern double tolerance_axial_yoke;
extern double tolerance_tilt_yoke;

extern double tolerance_expansion_inner_radius;
extern double tolerance_expansion_outer_radius;
extern double tolerance_expansion_thickness;
extern int separation_random_tolerance;

extern double radius_inner_MC10;
extern double radius_outer_MC10;
extern double radius_inner_MC11;
extern double radius_outer_MC11;
extern double radius_inner_MC20;
extern double radius_outer_MC20;
extern double radius_inner_MC21;
extern double radius_outer_MC21;
extern double radius_inner_MC30;
extern double radius_outer_MC30;

extern double lower_height_MC10;
extern double upper_height_MC10;
extern double lower_height_MC11;
extern double upper_height_MC11;
extern double lower_height_MC20;
extern double upper_height_MC20;
extern double lower_height_MC21;
extern double upper_height_MC21;
extern double lower_height_MC30;
extern double upper_height_MC30;

extern int n_YOKE;
extern int n_MC10;
extern int n_MC11;
extern int n_MC20;
extern int n_MC21;
extern int n_MC30;
extern int n_YOKEandMainCoils;
extern double RequiredAzimuthallyAveragedUniformity;
extern double RequiredLocalUniformity;

extern int RotationalDivisionNumber;
extern double radius_ShimCoil_Bz;
extern double top_ShimCoil_Bz;   
extern double bottom_ShimCoil_Bz;
extern double radius_ShimCoil_Br;
extern double top_ShimCoil_Br;   
extern double bottom_ShimCoil_Br;
extern double threshold_PassRate_IronShim;
extern double kappa_CostFunction;
extern double upsilon_CostFunction;
extern double div_scan_target;


void ReadConfigFile(const char* path_InputConfigFile);














