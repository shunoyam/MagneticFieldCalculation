
#include"../../include.h"
using namespace std;

void optimize_coil(cb& coils,int n_mode,Bs B_target,
                   int output,const char* outputfilename_Idistribution)
{
  try{
    cout<<"Current distributions of "<<n_mode<<" modes are being summed."<<endl;
    auto B_TG=(vector<Bfield>)B_target;
    ofstream f("Bz.dat");
    for(Bfield B:B_TG)
      {
      f<<B.Bz()<<endl;
      }
    int n_MFEP=B_TG.size();
    vector<coil> _coil=coils.coil_vec();
    int n_coil=_coil.size();
    if(n_MFEP<n_mode)
      {
        string text="Expected number of eigenmodes is larger than number of MFEPs.";
        cout<<"\033[31m error \033[m :"<<text<<endl;
        throw exception();
      }
    if(n_coil<n_mode)
      {
        string text="Expected number of eigenmodes is larger than number of coils.";
        cout<<"\033[31m error \033[m :"<<text<<endl;
        throw exception();
      }
    vector<vector<double>> _A_vec(n_MFEP);
    TVector3 temp_A;
    for(int i_MFEP=0;i_MFEP<n_MFEP;i_MFEP++)
      {
        for(int i_coil=0;i_coil<n_coil;i_coil++)
          {
            temp_A=_coil[i_coil].ResponseElement(B_TG[i_MFEP].MFEP());
            _A_vec[i_MFEP].push_back(temp_A.z());
          }
      }
    //
    vector<double> I_vec=solution_inverse_TSVD(_A_vec,B_target,n_mode);
    //
    for(int i_coil=0;i_coil<n_coil;i_coil++)
      {
        _coil[i_coil].setI(I_vec[i_coil]);
      }
    coils.set(_coil);
    if(output)
      {
        coils.draw(outputfilename_Idistribution);
      }
  }
  catch(exception e)
    {
      abort;
    }
}

void DrawEigenDistribution(Eigen::VectorXd v,string ofn_root)
{
  mms irons;
  irons.read("../config/shim_20210923.dat");
  irons.set(v);
  irons.draw(ofn_root.c_str());
}



