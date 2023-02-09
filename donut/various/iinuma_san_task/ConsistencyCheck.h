
#ifndef IN_CONSISTENCYCHECK
#define IN_CONSISTENCYCHECK
#include"../../src/include.h"
using namespace std;


void ConsistencyCheck(const char* path_to_MFEP_config_file,
                      const char* ofn_max_current_density,
                      const char* ofn_decreasing_error_dat="decreasing_error.dat",
                      const char* ofn_root="reproducted_magnetic_field.root");

void set_coil_miniSol(cb& Coil);

void draw_B_miniSol(cb IronYoke,cb Coil,const char* ofn_pdf,
                    const char* path_to_MFEP_config_file);

void set_coil_miniSol(cb& cb);

void create_yoke_geometry(double pitch,const char* ofn_dat="iron_shape_1mm.dat");

static void draw(Bs B_target,const char* ofn_root);





#endif
