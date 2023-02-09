

#include"../../include.h"
using namespace std;

double ellint_3_lib(double m,double alpha,double phase)
{
  double value=ellint_3(m,alpha,phase);
  return value;
}

double ellint_1_lib(double m,double phase)
{
  double value=ellint_1(m,phase);
  return value;
}

double legendre_lib(double n,double x)
{
  double value=legendre(n,x);
  return value;
}







