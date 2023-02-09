
#ifndef _IN_SRC_DEBUG
#define _IN_SRC_DEBUG

#include"../../src/include.h"

using namespace std;

void arc_cat(int n_arc,double r,double z_top,double z_bottom,double I,TVector3 mfep,
             TH2D* h,double range,int mesh);

void difference_arc_flc(TH2D* h_dif,TH2D* h_flc,TH2D* h_arc);

#endif
