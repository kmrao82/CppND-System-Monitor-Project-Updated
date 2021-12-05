#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream> 
#include <unistd.h>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;
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
  string os, version, kernel;
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

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string line;
  string key;
  string value;
  string memtotal, memfree;
  int memTotal, memFree;
  
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal:") {
           memtotal=value;
          break;
        }
        if(key=="MemFree:"){
           memfree = value;
           break;
        }        
       }
    }
  }
 
 memTotal=std::stoi(memtotal);
 memFree= std::stoi(memfree);

 return ((memTotal-memFree)*100/memTotal)/100.00;
}

//DONE:: TODO: Read and return the system uptime
long int LinuxParser::UpTime() { 
  string line; 
  string uptimeStr;
  long int uptimeLong;
  std::string::size_type size_t;
 
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptimeStr;
  }
 
  uptimeLong = std::stol(uptimeStr,&size_t);
  return uptimeLong;
  
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// DONE:Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 

  string line;
  string key;
  string value;
  vector<string> cpuData;
  bool flag=true;

  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);

      while (linestream >> key) {
        if (!flag)
        {
          cpuData.push_back(key);
        }
        else{
          flag=false;
        }
      }
    }
  }
  return cpuData; 

}

  


// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses(){ 
  string line;
  string key;
  string value;
  
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes"){
          return std::stoi(value);
        }
      }
    }
  }
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string line;
  string key;
  string value;
  
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running"){
          return std::stoi(value);
        }
      }
    }
  }
}

// DONE: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  // /proc/[pid]/cmdline
    string line;
    string key;
    string value;
    string PID = std::to_string(pid);

  std::ifstream filestream(kProcDirectory + PID + kCmdlineFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      return line;
    }
  }
}

// DONE: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  // /proc/pid/status
 
    string line;
    string key;
    string value;
    string PID = std::to_string(pid);

  std::ifstream filestream(kProcDirectory + PID + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize:"){
          return to_string(std::stol(value)/1024.0);
        }
      }
    }
  }
}

// DONE: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
    //proc/pid/status  
    string line;
    string key;
    string value;
    string PID = std::to_string(pid);

  std::ifstream filestream(kProcDirectory + PID + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid"){
          return value;
        }
      }
    }
  }
}

// DONE: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
    // /etc/passwd/
    string line;
    string key;
    string value;
    string PID = std::to_string(pid);
    string x;

  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> x >> value) {
        if(value==Uid(pid))
          return key;
      }
    }
  }
}

// DONE: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
// /proc/[pid]/stat/

    string line;
    string key;
    string value;
    string PID = std::to_string(pid);
    vector<string> filecontents;
    std::string::size_type size_t;
    string ticks;
  std::ifstream filestream(kProcDirectory + PID + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)){
      std::istringstream linestream(line);
      while(linestream >> key)
        filecontents.push_back(key);
      }
    } 
    ticks =filecontents[21]; 
    //std::cout << ticks << std::endl;
    long uptime = std::stol(ticks,&size_t);
    //std::cout << to_string(uptime/sysconf(_SC_CLK_TCK)) << std::endl;
      
    return uptime/sysconf(_SC_CLK_TCK);  
    //return 0.0;
    }
  

  
  

