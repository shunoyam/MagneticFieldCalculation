
#ifndef _IN_CONSTRUCTION_MRI
#define _IN_CONSTRUCTION_MRI

#include"../../src/include.h"
using namespace std;

static double Bz_MRI(vector<cb> MRI,TVector3 mfep);

void seeU_MRI();

static void Fill_Bz(TH2D* hist,cb coil,double range,int n);

void check_GaussNode();

void GaussStudy();


#endif
