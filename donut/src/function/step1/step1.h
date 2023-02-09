
#pragma once

#include"../../include.h"
using namespace std;
using namespace ROOT::Math;
using namespace Eigen;


void ReadCBsData(vector<cb>& CoilBlocks,
                 const char* path_to_magnet_data="../config/M20210727-Abe.DAT");

void ReadCBsData_sub(vector<cb>& mcyoke);

//void ReadShimsData(mms& shims,const char* filename_shimdata);

void CreateShimsGeometry(const char* path_to_outputfilename);

MatrixXd construct_response_matrix_mms(Bs Bs_for_MFEP_extraction,
                                       mms mms);

TVector3 get_uni_random_TVector3(TRandom2* random,
                                 double range_x,
                                 double range_y,
                                 double range_z);

double get_phi(TRandom2* random);

void InputTolerance(vector<cb>& mcyoke,
                    vector<double> theta_vec,
                    vector<double> phi_vec,
                    vector<TVector3> dis_vec,
                    vector<double> deformation_inner_vec,
                    vector<double> deformation_outer_vec,
                    vector<double> deformation_thickness_vec
                    );

TVector3 get_dis(TRandom2* random,double tolerance_r,double tolerance_z);

double get_deformation_inner(TRandom2* random);

double get_deformation_outer(TRandom2* random);

double get_deformation_thickness(TRandom2* random);

double get_random_discrete(double LowerEnd,double UpperEnd,TRandom2* random,
                           int separation=separation_random_tolerance);

void Adjustment_MinMode(double factor_adjustment,VectorXd& I_eigen,
                        int i_mode_min_ms,SVD result_SVD,double ModeStrength_min);

