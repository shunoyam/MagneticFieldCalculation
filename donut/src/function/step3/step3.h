
#pragma once
#include"../../include.h"
using namespace std;

void optimize_coil(cb& coils,int n_mode,Bs B_target
                             ,int output,const char* outputfilename_Idistribution="");

void DrawEigenDistribution(Eigen::VectorXd v,string ofn_root);

