
#include"multijob.cpp"

using namespace std;

void EigenModeScan_tilt()
{
  int i_line_ConfigFile=20;
  vector<int> i_line_CMakeList_vec{3,119,120};
  int n_scan=100;
  int mode_start=30;
  string path_to_config_file="../config/ConfigurationFile/global.h";
  int result=0;
  vector<string> lines_before{"project(it","add_executable(it","target_link_libraries(it"};
  vector<string> lines_after{")"," ${HEADER_FILE} ${SOURCE_FILES})",
                             " ${ROOT_LIBRARIES} ${Boost_LIBRARIES})"};
  try
    {
      for(int i_scan=0;i_scan<n_scan;i_scan++)
        {
          int TruncatedMode=mode_start+i_scan;
          string line_ConfigFile="TruncatedEigenmode\t";
          line_ConfigFile+=to_string(TruncatedMode);
          ReplaceToNumberedVariable(path_to_config_file,i_line_ConfigFile,line_ConfigFile);
          for(int i_change=0;i_change<3;i_change++)
            {
              string line=lines_before[i_change]+to_string(i_scan)+lines_after[i_change];
              ReplaceToNumberedVariable("CMakeLists.txt",
                                        i_line_CMakeList_vec[i_change],line);
            }
          string command="rm CMakeCache.txt&&cmake .&&make&&job ./it";
          command+=to_string(i_scan);
          int result=system(command.c_str());
          if(result)
            {
              throw i_scan;
            }
        }
    }
  catch(int ScanNumber)
    {
      cout<<"process stopped at "<<ScanNumber<<"th scan"<<endl;
    }
}
