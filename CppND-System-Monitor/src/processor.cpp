#include "processor.h"
#include "linux_parser.h"

//Return the aggregate CPU utilization
Processor::Processor(){
        kActive = LinuxParser::ActiveJiffies();
        kTotal = LinuxParser:: Jiffies();
    }
float Processor::Utilization() { 
    long k1Active = LinuxParser::ActiveJiffies();
    long k1Total = LinuxParser:: Jiffies();
    float cpuUti = (float(k1Active) - float(kActive)) / (float(k1Total) - float(kTotal));
    kActive = k1Active;
    kTotal = k1Total;

    return cpuUti; }