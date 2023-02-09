

#ifndef _IN_BATTLE
#define _IN_BATTLE

#include"../../src/include.h"
using namespace std;
using namespace Eigen;

void svd()
{
  ofstream ofn("SVD_TestMatrix.log");
  ofn<<fixed<<setprecision(15);
  int size=10;
  MatrixXd matrix(size,size);
  const char* path_to_ifn="../config/SVD_TestMatrix_1113_oyama.dat";
  ifstream ifn(path_to_ifn);
  if(ifn.fail())
    {
      puts("There is no such file");
      return;
    }
  string line;
  string elements_str;
  double element_double;
  vector<double> RowVector(size);
  for(int row=0;getline(ifn,line);row++)
    {
      istringstream stream(line);
      for(int col=0;stream>>RowVector[0] >>RowVector[1] >>RowVector[2] >>RowVector[3] >>RowVector[4] >>RowVector[5] >>RowVector[6] >>RowVector[7] >>RowVector[8] >>RowVector[9] ;col++)
        {
          for(int i_column=0;i_column<size;i_column++)
            {
              matrix(row,i_column)=RowVector[i_column];
            }
        }
    }
  ofn<<"target matrix"<<endl;
  ofn<<matrix<<endl;
  ofn<<"--------------------------------------------------------"<<endl;
  JacobiSVD<MatrixXd> SVD(matrix,ComputeFullU | ComputeFullV);
  MatrixXd U(size,size);
  MatrixXd V(size,size);
  V=SVD.matrixV();
  U=SVD.matrixU();
  ofn<<"singular values"<<endl;
  for(int i_mode=0;i_mode<size;i_mode++)
    {
      ofn<<SVD.singularValues()[i_mode]<<endl;
    }

  ofn<<"--------------------------------------------------------"<<endl;
  ofn<<"matrix U"<<endl;
  ofn<<U<<endl;
  
  ofn<<"--------------------------------------------------------"<<endl;
  ofn<<"matrix V"<<endl;
  ofn<<V<<endl;

  VectorXd sigma_vec(size);
  for(int i_size=0;i_size<size;i_size++)
    {
      sigma_vec(i_size)=SVD.singularValues()[i_size];
    }
  MatrixXd Sigma=sigma_vec.asDiagonal();
  
  MatrixXd matrix_re(size,size);
  matrix_re=U*Sigma*V.transpose();
  MatrixXd matrix_error=matrix-matrix_re;
  double sum_of_error=0.;

  for(int i_column=0;i_column<size;i_column++)
    {
      for(int i_row=0;i_row<size;i_row++)
        {
          sum_of_error+=abs(matrix_error(i_row,i_column));
        }
    }
  ofn<<"--------------------------------------------------------"<<endl;
  ofn<<"Target matrix is reconstructed from U,V,and singular values."<<endl;
  ofn<<"I defined error matrix as difference between target matrix and reconstructed matrix"<<endl;
  ofn<<"Absolutely sum of error matrix element: "<<sum_of_error<<endl;
}





#endif

