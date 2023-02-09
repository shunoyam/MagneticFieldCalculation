

#include"../../src/include.h"
using namespace std;


void EllipticalDeformation()
{
  magnet m;
  vector<double> sign{0.,0.,1.,1.,1.};
  double dev_r=0.1e-3;
  for(int i_coil=1;i_coil<6;i_coil++)
    {
      m.deformation_elliptical(i_coil,dev_r,TMath::Pi()/2.*sign[i_coil-1]);
    }
  // eval probe(m);
  // probe.out();
  Draw before(m,"ell2.root");
  return;
  puts("step 1");
  auto shim=new mms();
  m.shimming_iron(shim);
  puts("step 2");
  Draw after(m,*shim,"after2.root");
  delete shim;
}

void shimming_for_EllipticalDeformation()
{
  const char* ofn_before_root="BeforeShimming_EllipticalDeformation_MC10_0304.root";
  const char* ofn_after_root="AfterShimming_EllipticalDeformation_MC10_0304.root";
  magnet magnet_g2e;
  double deviation_deformation_elliptical_coil=0.1e-3;
  magnet_g2e.deformation_elliptical(n_MC10,deviation_deformation_elliptical_coil,0.);
  auto IronShim=new mms();
  magnet_g2e.shimming_iron(IronShim);
  eval data_after_shimming(magnet_g2e,*IronShim);
  data_after_shimming.out();
  Draw probe(magnet_g2e,*IronShim,ofn_after_root);
}

















