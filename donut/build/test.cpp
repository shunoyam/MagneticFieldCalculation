

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

void test()
{
  vector<double> ds{0.3,.4,0.5,14.};
  double d_total=0.;
  for(double d:ds)
    {
     d_total+=d*d;
    }
  cout<<sqrt(d_total)<<endl;
}
