#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::stol;
using std::string;
using std::to_string;
using std::vector;

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

// DONE: Read and return the system memory utilization
// Based on feedback in discussion forum I focus only on total and available memory
float LinuxParser::MemoryUtilization() { 
  string key, value, line;
  float memtotal, memfree; 
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "MemTotal:") {
        linestream >> memtotal;
      }
      if (key == "MemAvailable:") {
        linestream >> memfree;
      }
    } 
  }     
  return (memtotal-memfree)/memtotal;
}


// Read and return the system uptime
long LinuxParser::UpTime() { 
  // Read and return the system uptime
  long uptime;
  string line,uptimeString;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream uptimeStream(line);
    uptimeStream >> uptimeString;
  }
  uptime = std::stol(uptimeString);
  return uptime;
}

// I see an issue here with the call of the functions
// CpuUtilization is called twice to determine idle and active jiffies. 
// I think it should be called once and from that call Idle and active should be determined

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  return ActiveJiffies()+IdleJiffies(); 
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  auto cpu_util=CpuUtilization();
  return std::stol(cpu_util[CPUStates::kUser_]) + std::stol(cpu_util[CPUStates::kNice_]) 
  + std::stol(cpu_util[CPUStates::kSystem_]) + std::stol(cpu_util[CPUStates::kIRQ_])
  + std::stol(cpu_util[CPUStates::kSoftIRQ_]) + std::stol(cpu_util[CPUStates::kSteal_]);
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  auto cpu_util=CpuUtilization();
  return std::stol(cpu_util[CPUStates::kIdle_])+std::stol(cpu_util[CPUStates::kIOwait_]);
}

// Read and return CPU utilization
// define the function to extract all the ten values associated with the key cpu and return a vector of string
// reference explained here:  https://knowledge.udacity.com/questions/151964

vector<string> LinuxParser::CpuUtilization() { 
  string line,value;
  vector<string> data;

  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream my_stream(line);
    my_stream >> value; // read "cpu"
    for (int  i = 0; i < 10; i++)
          {
            my_stream >> value ;
            data.push_back(value);
          }
  }
  return data; 
  }

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line, key;
  int processes;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)){
      std::istringstream my_stream(line);
      my_stream >> key;
      if (key == "processes"){
        my_stream >> processes;
      }
    }
  }
  return processes;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { return 0; }

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) { return 0; }
