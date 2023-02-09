
#ifndef _IN_CALCTIME
#define _IN_CALCTIME
#include"../../src/include.h"
using namespace std;


void calctime_analysis()
{
  vector<cb> mcyoke;
  ReadCBsData(mcyoke);
  for(int i=0;i<10;i++)
    {
      cout<<i<<"th iteration"<<endl;
      auto data_fiducial=Fiducial(mcyoke,"z");
    }
  
}














#endif
