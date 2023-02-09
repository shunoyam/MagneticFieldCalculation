
//use root without compile

#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<cstdlib>
#include<math.h>
#include<time.h>
#include<vector>
#include<fstream>
#include<iostream>
#include<iomanip>
#include<cstdio>
#include<numeric>
#include<cmath>


void ReplaceToNumberedVariable(string path_to_file,int ChangingLine,string ChangingString)
{
  ifstream InputFile(path_to_file.c_str());
  if(InputFile.fail())
    {
      cout<<path_to_file<<" not found"<<endl;
      return;
    }
  string line;
  vector<string> string_vec;
  for(int i_line=0;getline(InputFile,line);i_line++)
    {
      if(i_line+1==ChangingLine)
        {
          string_vec.push_back(ChangingString);
        }
      else
        {
          string_vec.push_back(line);
        }
    }
  ofstream OutputFile(path_to_file.c_str());
  for(string _string:string_vec)
    {
      OutputFile<<_string<<endl;
    }
}
