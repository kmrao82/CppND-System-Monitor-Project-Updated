#include "processor.h"
#include <string>
#include <vector>
#include <iostream> 

#include "linux_parser.h"
using std::string;
using std::vector;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    vector<string> cpuData = LinuxParser::CpuUtilization();
    unsigned long int user = std::stoi(cpuData[0]);
    unsigned long int nice = std::stoi(cpuData[1]); 
    unsigned long int system = std::stoi(cpuData[2]);
    unsigned long int idle = std::stoi(cpuData[3]);   
    unsigned long int iowait = std::stoi(cpuData[4]);
    unsigned long int irq = std::stoi(cpuData[5]);
    unsigned long int softirq = std::stoi(cpuData[6]);
    unsigned long int steal = std::stoi(cpuData[7]);
    unsigned long int guest = std::stoi(cpuData[8]);
    unsigned long int guest_nice = std::stoi(cpuData[9]);

  unsigned long int idleAll = idle + iowait;
  unsigned long int systemAllTime = system + irq + softirq;
  unsigned  long int virtallTime = guest + guest_nice;
  unsigned  long int nonIdle = user + nice + systemAllTime + steal + virtallTime;
  unsigned  long int total = idleAll + nonIdle;

  // # differentiate: actual value minus the previous one
  float totald = total - prevTotal;
  float idled = idleAll - prevIdleAll;

  float cpuPercentage = (totald - idled) / totald;

  prevTotal = total;
  prevIdleAll = idleAll;

  return cpuPercentage;  
 }