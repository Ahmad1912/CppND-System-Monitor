#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;
// constructor
Process::Process(int id) : pid_(id){}
// Return this process's ID
int Process::Pid() { return pid_; }

//Return this process's CPU utilization
float Process::CpuUtilization() const { return LinuxParser::CpuUtilization(pid_);}
    
// Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_); }

// Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid_); }

// Return the age of this process (in seconds)
float Process::UpTime() { return LinuxParser::UpTime(pid_); }

//Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const { return LinuxParser::CpuUtilization(a.pid_) < LinuxParser::CpuUtilization(this->pid_); }