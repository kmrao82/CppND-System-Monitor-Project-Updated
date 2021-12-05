#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
Process::Process(int pid){
     proc_pid= pid;
     proc_ram=std::stof(this->Ram());
}

int Process::Pid() { return proc_pid;}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
    // /proc/PID/stat
    
    string line;
    string key;
    string value;
    string PID = std::to_string(this->Pid());
    vector<string> filecontents;
    std::string::size_type size_t;
    string ticks;
    std::ifstream filestream(LinuxParser::kProcDirectory + PID + LinuxParser::kStatFilename);
    if (filestream.is_open()) {
        while (std::getline(filestream, line)){
        std::istringstream linestream(line);
        while(linestream >> key)
            filecontents.push_back(key);
        }
    }     
    long utime = std::stol(filecontents[13],&size_t);///sysconf(_SC_CLK_TCK);
    long stime = std::stol(filecontents[14],&size_t);///sysconf(_SC_CLK_TCK);
    long cutime = std::stol(filecontents[15],&size_t);///sysconf(_SC_CLK_TCK);
    long cstime = std::stol(filecontents[16],&size_t);///sysconf(_SC_CLK_TCK);
    long starttime = std::stol(filecontents[21],&size_t);///sysconf(_SC_CLK_TCK);

    long total_time=utime+stime+cutime+cstime;
    long seconds= starttime/sysconf(_SC_CLK_TCK);
   // long seconds= LinuxParser::UpTime(this->Pid())-starttime;
    /*std::cout << to_string(Process::UpTime()) << std::endl;
    std::cout << to_string(starttime) << std::endl;
    std::cout << to_string(seconds) << std::endl;
    */
    float cpu_usage = 100*((total_time/sysconf(_SC_CLK_TCK))/seconds);
    
    return cpu_usage;
 }

// TODO: Return the command that generated this process
string Process::Command() { 
    int pid= this->Pid();
    return LinuxParser::Command(pid);
    
}

// TODO: Return this process's memory utilization
string Process::Ram() { 
    int pid = this ->Pid();
    return LinuxParser::Ram(pid);
}

// TODO: Return the user (name) that generated this process
string Process::User() { 
    int pid= this->Pid();
    return LinuxParser::User(pid); 
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { 
    int pid =this->Pid();
    return LinuxParser::UpTime(pid);
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { 
    
    return a.proc_ram < this->proc_ram;

    //< std::this->Ram(this->Pid());
}