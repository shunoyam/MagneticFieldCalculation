
#pragma once

#include"../../../src/include.h"
using namespace std;

void ResidualBx();

void shimming_U_lc(magnet m,mms* shims);

void correlation_U_lc_residualBx();

static void draw_correlation(vector<double> vector1,vector<double> vector2,
                             const char* ofn_root,vector<double> U_theta_vec);

void MostBxSearch();
