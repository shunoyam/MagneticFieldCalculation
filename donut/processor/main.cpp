
#ifndef _IN_MAIN
#define _IN_MAIN
#include"../src/include.h"
using namespace std;

int main()
{
  clock_t start=clock();
  puts("Time measurement started");
  dl(3);
  const char* path_to_configfile="../config/ConfigurationFile/global.h";
  ReadConfigFile(path_to_configfile);
  desk();
  
  clock_t end=clock();
  double CPUtime=(double)(end-start)/CLOCKS_PER_SEC;
  dl(3);
  cout<<"CPU time: "<<CPUtime<<"s"<<endl;
  return 0;
}


#endif



