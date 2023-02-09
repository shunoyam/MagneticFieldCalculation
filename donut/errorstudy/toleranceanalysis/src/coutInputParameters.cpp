
#ifndef _IN_COUTINPUTPARAMETERS
#define _IN_COUTINPUTPARAMETERS

#include"../../../src/include.h"
using namespace std;

void cout_1coil(string CoilName
                ,int CoilNumber,vector<XYZVector>* dis_vec,
                vector<double>* def_in_vec,vector<double>* def_out_vec,
                vector<double>* def_thi_vec,
                vector<double>* theta_vec,vector<double>* phi_vec)
{
  cout<<endl;
  cout<<"magnet::dis(n_"<<CoilName<<",";
  cout<<"TVector3("<<dis_vec->at(CoilNumber).x()*1.e3<<"*1.e-3,";
  cout<<dis_vec->at(CoilNumber).y()*1.e3<<"*1.e-3,";
  cout<<dis_vec->at(CoilNumber).z()*1.e3<<"*1.e-3)";
  cout<<");"<<endl;
  cout<<"magnet::deformation(n_"<<CoilName<<",";
  cout<<def_in_vec->at(CoilNumber-1)*1.e3<<"*1.e-3,";
  cout<<def_out_vec->at(CoilNumber-1)*1.e3;
  cout<<"*1.e-3,"<<def_thi_vec->at(CoilNumber-1)*1.e3<<"*1.e-3);"<<endl;
  cout<<"magnet::tilt(n_"<<CoilName;
  cout<<","<<theta_vec->at(CoilNumber)*1.e3<<"*1.e-3,";
  cout<<phi_vec->at(CoilNumber)<<");"<<endl;

}


void coutInputParameters(vector<XYZVector>* dis_vec,
                         vector<double>* def_in_vec,vector<double> *def_out_vec,
                         vector<double>* def_thi_vec,
                         vector<double>* theta_vec,
                         vector<double>* phi_vec)
{
  cout<<endl;
  cout<<"magnet::dis(n_YOKE,";
  cout<<"TVector3("<<dis_vec->at(n_YOKE).x()*1.e3;
  cout<<"*1.e-3,"<<dis_vec->at(n_YOKE).y()*1.e3<<"*1.e-3,";
  cout<<dis_vec->at(n_YOKE).z()*1.e3<<"*1.e-3)";
  cout<<");"<<endl;
    
  cout<<"magnet::tilt(n_YOKE,";
  cout<<theta_vec->at(n_YOKE)*1.e3<<"*1.e-3,";
  cout<<phi_vec->at(n_YOKE)<<");"<<endl;

  vector<string> CoilName_vec{"MC10","MC11","MC20","MC21","MC30"};
  
  for(int i_coil=1;i_coil<6;i_coil++)
    {
      cout_1coil(CoilName_vec[i_coil-1],
                 i_coil,dis_vec,
                 def_in_vec,def_out_vec,def_thi_vec,
                 theta_vec,phi_vec);
    }
}

void SetSeparation(int separation)
{
  const char* path_to_config_file="../config/ConfigurationFile/global.h";
  int line_separation=42;
  string separation_new=to_string(separation);
  string line_new="separation_random_tolerance "+separation_new;


  string command_1="sed -i '";
  string command_2=to_string(line_separation)+"c ";
  string command_3=line_new+"' "+path_to_config_file;
  string command=command_1+command_2+command_3;
  int result=system(command.c_str());
}

double index_dis(vector<XYZVector> dis_vec)
{
  if(dis_vec.size()!=6)
    {
      puts("Size of displacement vector should be 6");
      abort;
    }
  vector<double> sensitivity_zdis{31.8,74.4,74.4,52.9,52.9,112.};
  vector<int> sign_zdis_vec{-1,-1,-1,1,1,1};
  double z_dis;
  double index=0.;
  for(int i_coil=0;i_coil<6;i_coil++)
    {
      z_dis=dis_vec[i_coil].z();
      index+=sign_zdis_vec[i_coil]*(z_dis*1000)*sensitivity_zdis[i_coil];
    }
  return index;  
}

double index_def(vector<double> def_in_vec,vector<double> def_out_vec)
{
  if(def_in_vec.size()!=5 || def_out_vec.size()!=5)
    {
      puts("Size of deformation vector should be 5");
      abort;
    }
  vector<double> sensitivity_def_in{11.4,11.4,23.9,23.9,6.27};
  vector<double> sensitivity_def_out{11.8,11.8,21.7,21.7,5.84};
  vector<int> sign_def_vec{1,-1,1,-1,0};
  double index=0.;

  for(int i_coil=0;i_coil<5;i_coil++)
    {
      index+=sign_def_vec[i_coil]*1000.*sensitivity_def_in[i_coil]*def_in_vec[i_coil];
      index+=sign_def_vec[i_coil]*1000.*sensitivity_def_out[i_coil]*def_out_vec[i_coil]; 
    }
  return index;
}


double index_axial(vector<XYZVector> dis_vec,
              vector<double> def_in_vec,vector<double> def_out_vec)
{
  double index=index_def(def_in_vec,def_out_vec)-index_dis(dis_vec);
  return index;
}

double index_rotational(vector<double> theta_vec,vector<double> phi_vec,
                        vector<XYZVector> dis_vec)
{
  vector<double> sensitivity_tilt{23.0846,428.956,428.956,237.348,237.348,66.4374};
  vector<int> sign_tilt{0,1,-1,1,-1,0};
  vector<double> sensitivity_radial{126.0856,210.302,210.302,200.584,200.584,282.54};
  vector<int> sign_radial{1,1,1,-1,-1,-1};
  double vector_x=0.,vector_y=0.;
  
  for(int i_coil=0;i_coil<6;i_coil++)
    {
      vector_x+=sign_tilt[i_coil]*sensitivity_tilt[i_coil]*theta_vec[i_coil]*cos(phi_vec[i_coil]);
      vector_x+=sign_radial[i_coil]*sensitivity_radial[i_coil]*dis_vec[i_coil].x();
      vector_y+=sign_tilt[i_coil]*sensitivity_tilt[i_coil]*theta_vec[i_coil]*sin(phi_vec[i_coil]);
      vector_y+=sign_radial[i_coil]*sensitivity_radial[i_coil]*dis_vec[i_coil].y();
    }
  double index=1000.*sqrt(vector_x*vector_x+vector_y*vector_y);
  return index;
}


double ToleranceFactor_local(vector<XYZVector> dis_vec,
                        vector<double> def_in_vec,vector<double> def_out_vec,
                        vector<double> theta_vec,vector<double> phi_vec)
{
  double _index_axial=index_axial(dis_vec,def_in_vec,def_out_vec);
  double _index_rotational=index_rotational(theta_vec,phi_vec,dis_vec);
  double _ToleranceFactor=sqrt(pow(_index_rotational,2)+pow(_index_axial,2));
  return _ToleranceFactor;
}

double ToleranceFactor_theta(vector<XYZVector> dis_vec,
                        vector<double> def_in_vec,vector<double> def_out_vec,
                        vector<double> theta_vec,vector<double> phi_vec)
{
  double _index_axial=index_axial(dis_vec,def_in_vec,def_out_vec);
  double _ToleranceFactor=abs(_index_axial);
  return _ToleranceFactor;
}













#endif

