
#pragma once

#include"../../../src/include.h"
using namespace std;


void HigherCorrection(string ofn_root,int mode,string NormalOrSkew);
//for (mode=2,NormalOrSkew="normal"), Bx is generated.

void analysis_HigherCorrection(string ofn_root);

void ExtractShimCoilInformation(TTree* tr,string element,TTree* tr_ref);
//tr_ref is data without tolerances only 1 entry

void IntegratedShimCoilStudy();
