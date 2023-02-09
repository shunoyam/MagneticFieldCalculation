
#ifndef _IN_COUTINPUTPARAMETERS
#define _IN_COUTINPUTPARAMETERS

#include"../../../src/include.h"
using namespace std;

void coutInputParameters(vector<XYZVector>* dis_vec,
                         vector<double>* def_in_vec,vector<double>* def_out_vec,
                         vector<double>* def_thi_vec,
                         vector<double>* theta_vec,
                         vector<double>* phi_vec);

void cout_1coil(string CoilName,
                int CoilNumber,vector<XYZVector>* dis_vec,
                vector<double>* def_in_vec,vector<double>* def_out_vec,
                vector<double>* def_thi_vec,
                vector<double>* theta_vec,vector<double>* phi_vec);

void SetSeparation(int separation);

double index_dis(vector<XYZVector> dis_vec);

double index_def(vector<double> def_in_vec,vector<double> def_out_vec);

double index_axial(vector<XYZVector> dis_vec,
              vector<double> def_in_vec,vector<double> def_out_vec);

double index_rotational(vector<double> theta_vec,vector<double> phi_vec,
                        vector<XYZVector> dis_vec);

double ToleranceFactor_local(vector<XYZVector> dis_vec,
                        vector<double> def_in_vec,vector<double> def_out_vec,
                       vector<double> theta_vec,vector<double> phi_vec);

double ToleranceFactor_theta(vector<XYZVector> dis_vec,
                        vector<double> def_in_vec,vector<double> def_out_vec,
                       vector<double> theta_vec,vector<double> phi_vec);


#endif
