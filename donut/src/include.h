
#pragma once


#include<TFile.h>
#include<TVector3.h>
#include<TTree.h>
#include<TGraph.h>
#include<TCanvas.h>
#include<TH2.h>
#include<TF1.h>
#include<TStyle.h>
#include<TPaveStats.h>
#include<TMultiGraph.h>
#include<TPaletteAxis.h>
#include<TRandom2.h>
#include<Math/Vector3D.h>
#include<TFrame.h>
#include<TArrow.h>

#include<bits/stdc++.h>

#include<Eigen/SVD>
#include<Eigen/Core>
#include"constants.h"

#include"../config/ReadConfigFile.h"

#include"../processor/desk.h"

#include"../src/lib/function_lib.h"
#include"../src/lib/function_lib_cpp17/function_lib_cpp17.h"
#include"class/Legendre/Legendre.h"
#include"class/magnetic_source/magnetic_source.h"
#include"class/BiotSavart/BiotSavart.h"
#include"class/magnetic_source/coil/coil.h"
#include"class/Bfield/Bfield.h"
#include"class/magnetic_source/mm/mm.h"
#include"class/cb/cb.h"
#include"class/info_SVD/info_SVD.h"
#include"class/mms/mms.h"

#include"class/svd/svd.h"
#include"class/ShimCoil/T_ShimCoil/T_ShimCoil.h"
#include"class/ShimCoil/shim_coil.h"
#include"class/ShimCoil/ShimCoil.h"
#include"class/magnet/magnet.h"
#include"class/Bs/Bs.h"

#include"class/eval/eval.h"
#include"class/draw/draw.h"

#include"function/step1/step1.h"
#include"function/step2/step2.h"
#include"function/step3/step3.h"

#include"../errorstudy/individualstudy/error3.h" 
#include"../errorstudy/toleranceanalysis/ShimCoilError/ShimCoilError.h"
#include"../errorstudy/toleranceanalysis/ShimCoilError/HigherMode_ShimCoil.h"
#include"../various/debug/cm.h"
 #include"../shimming/study/study2.h" 

#include"../various/debug/debug.h"
#include"../various/debug/src_debug.h"
#include"../various/debug/jan26/src_jan26.h"
#include"../various/debug/jan26/jan26.h"
#include"../various/CoilDesign/ShimCoilDesign.h"
#include"../various/report/report1.h"
#include"../various/report/report2.h"
#include"../various/ShimCoil/BestBx.h"
#include"../various/opera/20220810/FieldMap.h"
#include"../various/multichannelization/study1.h"
#include"../various/opera/OPERAstudy/OPERAstudy.h"













