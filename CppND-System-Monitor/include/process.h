#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int id);
  int Pid();                               
  std::string User();                      
  std::string Command();                   
  float CpuUtilization() const;                  
  std::string Ram();                       
  float UpTime();                       
  bool operator<(Process const& a) const;  

  // Declare any necessary private members
 private:
 int pid_;
};

#endif