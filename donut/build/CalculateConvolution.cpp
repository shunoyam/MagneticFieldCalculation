

#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<vector>
#include<fstream>
#include<iostream>
#include<iomanip>
#include<cstdio>
#include<numeric>
#include<string.h>

using namespace std;

void convolution(string BeamProfile,string FieldMap)
{
  ifstream beam(BeamProfile.c_str());
  ifstream field(FieldMap.c_str());
  vector<double> I_vec;
  vector<double> bz_vec;
  vector<double> r_vec;
  double r,z,I,bz;
  int n_point=0;
  double numerator=0.;
  double dominator=0.;
  while(beam>>r>>z>>I)
    {
      field>>r>>z>>bz;
      I_vec.push_back(I);
      bz_vec.push_back(bz);
      r_vec.push_back(r);
      numerator+=r*I*bz;
      dominator+=r*I;
      n_point++;
    }
  double bz_ave=numerator/dominator;
  cout<<"bz average: "<<bz_ave<<endl;
  double variance=0.;
  for(int i=0;i<I_vec.size();i++)
    {
      variance+=r_vec[i]*pow(bz_vec[i]-bz_ave,2)*I_vec[i];
    }
  double dev=sqrt(variance/dominator);
  cout<<"convoluted deviation: "<<dev<<endl;
}


void CalculateConvolution()
{
  for(int i=0;i<3;i++)
    {
      string BeamProfile=to_string(i)+"distribution.dat";
      string FieldMap="AzimuthalBzDistribution.dat";
      convolution(BeamProfile,FieldMap);
    }

}
