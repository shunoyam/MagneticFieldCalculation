
cmake_minimum_required(VERSION 3.12)
project(it)

set(CMAKE_CXX_STANDARD 17)

list(APPEND CMAKE_PREFIX_PATH  $ENV{ROOTSYS})
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} $ENV{ROOTSYS}/etc/cmake)

set(CMAKE_CXX_FLAGS "-Ofast -Dcxx=17")#O3 O2 O1

find_package(ROOT REQUIRED)
include(${ROOT_USE_FILE})


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

#include_directories(${ROOT_INCLUDE_PATH})
include_directories(${ROOT_INCLUDE_DIRS})
#your path to eigen below
include_directories(../src/eigen3)

set(HEADER_FILES
  ../src/lib/function_lib.h
  ../src/lib/function_lib_cpp17/function_lib_cpp17.h
  ../src/class/magnetic_source/magnetic_source.h
  ../config/ReadConfigFile.h
  ../src/class/BiotSavart/BiotSavart.h
  ../src/class/magnetic_source/coil/coil.h ../src/class/Bfield/Bfield.h
  ../src/class/magnetic_source/mm/mm.h ../src/class/Bs/Bs.h
  ../src/class/info_SVD/info_SVD.h
  ../src/class/cb/cb.h ../src/class/mms/mms.h
  ../src/class/ShimCoil/T_ShimCoil/T_ShimCoil.h
  ../src/class/ShimCoil/shim_coil.h
  ../src/class/ShimCoil/ShimCoil.h
  ../src/class/Legendre/Legendre.h
  ../src/class/svd/svd.h
  ../src/class/eval/eval.h
  ../src/class/magnet/magnet.h
  ../src/class/draw/draw.h
  ../shimming/study/study2.h
  
  ../src/function/step1/step1.h ../src/function/step2/step2.h ../src/function/step3/step3.h
  ../errorstudy/toleranceanalysis/ShimCoilError/ShimCoilError.h
  ../errorstudy/toleranceanalysis/ShimCoilError/HigherMode_ShimCoil.h
  ../processor/desk.h
  ../various/CoilDesign/ShimCoilDesign.h
  ../various/report/report1.h
  ../various/ShimCoil/BestBx.h
  ../various/opera/20220810/FieldMap.h
  ../various/opera/OPERAstudy/OPERAstudy.h
  ../various/multichannelization/study1.h
  ../various/report/report2.h
  )

set(SOURCE_FILES
  ../processor/desk.cpp
  ../src/lib/function_lib.cpp
  ../src/lib/function_lib_cpp17/function_lib_cpp17.cpp
  ../config/ReadConfigFile.cpp
  ../src/class/Legendre/Legendre.cpp
  ../src/class/magnetic_source/magnetic_source.cpp
  ../src/class/BiotSavart/BiotSavart.cpp
  ../src/class/magnetic_source/coil/coil.cpp
  ../src/class/Bfield/Bfield.cpp
  ../src/class/magnetic_source/mm/mm.cpp ../src/class/Bs/Bs.cpp
  ../src/class/info_SVD/info_SVD.cpp
  ../src/class/cb/cb.cpp ../src/class/mms/mms.cpp
  ../src/class/magnet/shimming_iron.cpp
  ../src/class/ShimCoil/T_ShimCoil/T_ShimCoil.cpp
  ../src/class/ShimCoil/shim_coil.cpp
  ../src/class/ShimCoil/ShimCoil.cpp
  ../src/class/ShimCoil/additional_ShimCoil.cpp
  ../src/class/ShimCoil/draw_ShimCoil.cpp  
  ../src/class/Bs/Bs_2.cpp
  ../src/class/svd/svd.cpp
  ../src/class/eval/eval.cpp
  ../src/class/magnet/magnet.cpp
  ../src/class/draw/draw.cpp
  ../shimming/study/study1.cpp
  ../shimming/study/study2.cpp
  ../shimming/src/target_validity.cpp

  ../src/function/step1/step1.cpp ../src/function/step2/step2.cpp
  ../src/function/step3/step3.cpp 
  ../errorstudy/individualstudy/error3.cpp
  ../errorstudy/toleranceanalysis/ShimCoilError/ShimCoilError.cpp
  ../errorstudy/toleranceanalysis/ShimCoilError/HigherMode_ShimCoil.cpp
  ../various/debug/debug.cpp
  ../various/debug/src_debug.cpp
  ../various/debug/cm.cpp
  ../various/CoilDesign/ShimCoilDesign.cpp
  ../various/report/report1.cpp
  ../various/report/report2.cpp
  ../various/multichannelization/study1.cpp
  ../various/ShimCoil/BestBx.cpp
  ../various/opera/OPERAstudy/OPERAstudy.cpp
  ../various/opera/20220810/FieldMap.cpp
  ../processor/main.cpp
  )

add_executable(it ${HEADER_FILE} ${SOURCE_FILES})
target_link_libraries(it ${ROOT_LIBRARIES} ${Boost_LIBRARIES})












