
#pragma once

#include"../../include.h"
using namespace std;

vector<double> solution_inverse_TSVD(vector<vector<double>> ResponseMatrix,
                                     Bs B_target,int n_mode);

int evalerror(cb FilamentLoopCoil,Bs B_TG
                  ,int output=0,const char* outputfilename_Bdistribution="");
//calculate B_z on the surface of the sphere and check if
//(p-p amplitude of Bz of error field)/(mean value of Bz of target field)
//is under 1ppm or not 

void DesignFLCandDrawB(cb& FilamentLoopCoil,int n_coil,double length,
                       Bs B_Target,
                       const char* outputfilename_Idistribution="",
                       const char* outputfilename_Bdistribution="");
//Input solenoid coil length and number of coils.
//Current distribution will be calculated,
//so that the uniformity of B_z is under 1ppm on the surface of 40cm-DSV.
//2 graphs will be created. One is a graph of current distribution of each coil.
//The another is contour of equal flux near the coils.

void setCB(cb& CoilBlock,int r_mesh,int z_mesh,
           double lowerend,double upperend,double r_in,double r_out);

void search_storage_region_Bz(vector<cb> mcyoke);

void setVfromTSVDdata(mms& mms,
                      SVD result_SVD,
                      Bs B_TG,double threshold_ModeStrength=1.e-16,
                      int n_mode=TruncatedEigenmode);

vector<double> get_theta_vec(TRandom2* random);

vector<double> get_phi_vec(TRandom2* random);

vector<double> get_deformation_inner_vec(TRandom2* random);

vector<double> get_deformation_outer_vec(TRandom2* random);

vector<double> get_deformation_thickness_vec(TRandom2* random);

vector<TVector3> get_dis_vec(TRandom2* random);









































