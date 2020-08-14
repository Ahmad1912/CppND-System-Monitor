#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <unistd.h>
#include <iostream>
#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;
using std::stol;
using std::stoi;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

//Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  float memT, memF;
  string line;
  string name, number;
  std::ifstream stream(kProcDirectory  + kMeminfoFilename);
  if (stream.is_open()){
    while (std::getline(stream, line))
    {
     std::istringstream linestr(line);
     linestr >> name >> number;
     if(name == "MemTotal:"){
      memT = stof(number);
     }else if(name == "MemFree:"){
       memF = stof(number);
       break;
     }
    }
  }
  return ((memT-memF)/memT); }

//Read and return the system uptime
long LinuxParser::UpTime() { 
  string strTime, line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if(stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestr(line);
    linestr >> strTime;
  }
  return stol(strTime); }

//  Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  string line, cpu;
  long sum, jiffy;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestr(line);
    linestr >> cpu;
    while (linestr >> jiffy)
    {
      sum += jiffy;
    }
    
  }
  return sum; }

// Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) { 
  string line, notRelevant;
  long sum = 0, actJ;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if(stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestr(line);
    for(int i = 1; i <= 17; i++ ){
      if( i > 13 && i <= 17){
        linestr >> actJ;
        sum += actJ;
      }else
      {
        linestr >> notRelevant;
      }
    }
  }
  return sum/sysconf(_SC_CLK_TCK); }

// Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
vector<string> numberActJiffies = CpuUtilization();

  return stol(numberActJiffies[CPUStates::kUser_]) + stol(numberActJiffies[CPUStates::kNice_]) + stol(numberActJiffies[CPUStates::kSystem_]) + 
 + stol(numberActJiffies[CPUStates::kIRQ_]) + stol(numberActJiffies[CPUStates::kSoftIRQ_]) + stol(numberActJiffies[CPUStates::kSteal_]); }

//Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  vector<string> idleJiffies = CpuUtilization();

  return stol(idleJiffies[CPUStates::kIdle_]) + stol(idleJiffies[CPUStates::kIOwait_]); }

//Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string line, cpu, jiffy;
  vector<string> jiffies;

  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestr(line);
    linestr >> cpu;
    while (linestr >> jiffy)
    {
      jiffies.push_back(jiffy);
    } 
   }
   return jiffies;
}

//Read and return CPU utilization of processes
float LinuxParser::CpuUtilization(int pid) {
  string line, notRelevant;
  long total_time = LinuxParser::ActiveJiffies(pid);
  long seconds =  LinuxParser::UpTime(pid);
  
  return float(total_time/seconds); }

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  int total = 0;
  string line, key_total;
  std::ifstream stream(kProcDirectory  + kStatFilename);
  if (stream.is_open()){
    while (std::getline(stream, line))
    {
     std::istringstream linestr(line);
     linestr >> key_total;
     if(key_total == "processes"){
       linestr >> total;
       return total;
     }
    }
  }
   return total; 
   }

// Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  int RP = 0;
  string line, key_RP;
  std::ifstream stream(kProcDirectory  + kStatFilename);
  if (stream.is_open()){
    while (std::getline(stream, line))
    {
     std::istringstream linestr(line);
     linestr >> key_RP;
     if(key_RP == "procs_running"){
       linestr >> RP;
     return RP; 
     }
    }
  }
   return 0; 
 }

// Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  string line;
  std::ifstream stream(kProcDirectory  + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()){
     std::getline(stream, line);
  }
  return line; }

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
  int mem;
  string line, key;
  std::ifstream stream(kProcDirectory  + to_string(pid) + kStatusFilename);
  if (stream.is_open()){
    while (std::getline(stream, line))
    {
     std::istringstream linestr(line);
     linestr >> key;
     if(key == "VmSize:"){
       linestr >> mem;
       mem /= 1000;
       break;
     }
    }
  }
  return std::to_string(mem); 
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string line, key, Uid;
  std::ifstream stream(kProcDirectory  + to_string(pid) + kStatusFilename);
  if (stream.is_open()){
    while (std::getline(stream, line))
    {
     std::istringstream linestr(line);
     linestr >> key;
     if(key == "Uid:"){
       linestr >> Uid;
       break;
     }
    }
  }
  return Uid; }

// Read and return the user associated with a process
string LinuxParser::User(int pid) { 
  string str_Id, x, user, Id, line;
  str_Id = Uid(pid);
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()){
    while (std::getline(stream, line))
    {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestr(line);
      linestr >> user >> x >> Id;
      if(Id == str_Id){
        break;
     }
    }
  }
  return user; 
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) { 
  string line, notRelevant;
  long time;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if(stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestr(line);
    for(int i = 0; i < 22; i++ ){
      linestr >> time;
      }
    }
  return UpTime() - (time / sysconf(_SC_CLK_TCK)) ;
  //return time / sysconf(_SC_CLK_TCK);
}