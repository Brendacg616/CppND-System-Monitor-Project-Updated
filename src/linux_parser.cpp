#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
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
  string os, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel;
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string line;
    string tag;
    std::vector<int> Data(4);
    std::ifstream stream(kProcDirectory + kMeminfoFilename);
    if (stream.is_open()) {
      for (int i = 0; i < 2; i++)
      {
        std::getline(stream, line);
        std::istringstream linestream(line);
        linestream >> tag >> Data[i];
      }
    }
    double TotalUsedMem = Data[0] - Data[1];
    return TotalUsedMem/(Data[0]*1.0);
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  string line;
  long int upTime{0};
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> upTime;
  }
  return upTime;
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

vector<string> LinuxParser::CpuUtilization() {
  vector<string> cpuData(11);
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
      std::getline(stream, line);
      std::istringstream linestream(line);
      for (int i = 0; i < 11; i++)
    {
      linestream >> cpuData[i];
    }
  }
  return cpuData;
}

int LinuxParser::TotalProcesses() {
  string line;
  string tag;
  int Data;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (tag!="processes" &&  std::getline(stream, line))
    {
      std::istringstream linestream(line);
      linestream >> tag >> Data;
    }
  }
  return Data;
}

int LinuxParser::RunningProcesses() {
  string line;
  string tag;
  int Data;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (tag!="procs_running" &&  std::getline(stream, line))
    {
      std::istringstream linestream(line);
      linestream >> tag >> Data;
    }
  }
  return Data;
}

string LinuxParser::Command(int pid) {
  string command;
  std::ifstream stream(kProcDirectory + to_string(pid) +kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, command);
  }
  return command;
}

string LinuxParser::Ram(int pid) {
  string line;
  string tag;
  int mem_kB;
  std::ifstream stream(kProcDirectory + to_string(pid) +kStatusFilename);
  if (stream.is_open()) {
    while (tag!="VmSize:" &&  std::getline(stream, line))
    {
      std::istringstream linestream(line);
      linestream >> tag >> mem_kB;
    }
  }
  return to_string(mem_kB/1024);
}

string LinuxParser::Uid(int pid) {
  string line;
  string tag;
  string uid;
  std::ifstream stream(kProcDirectory + to_string(pid) +kStatusFilename);
  if (stream.is_open()) {
    while (tag!="Uid:" &&  std::getline(stream, line))
    {
      std::istringstream linestream(line);
      linestream >> tag >> uid;
    }
  }
  return uid;
}

string LinuxParser::User(int pid) {
  string line;
  string current_uid,x;
  string user;
  char delim{':'};
  string uid = Uid(pid);
  std::ifstream stream(kPasswordPath );
  if (stream.is_open()) {
    while (current_uid!=uid &&  std::getline(stream, line))
    {
      std::replace(line.begin(), line.end(), delim, ' ');
      std::istringstream linestream(line);
      linestream >> user >> x >>current_uid;
    }
  }
  return user;
}

long LinuxParser::UpTime(int pid) {
  long total_uptime = LinuxParser::UpTime();
  string line;
  string data;
  long up_time{0};
  long clock_ticks;
  std::ifstream stream(kProcDirectory + to_string(pid) +kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    for (int i = 0; i < 21 ; i++)
    {
      linestream >> data;
    }
    linestream >> clock_ticks;
    up_time = total_uptime - (clock_ticks / sysconf(_SC_CLK_TCK));
  }
  return up_time;
}

vector<long> LinuxParser::CpuUtilization(int pid)
{
  string data;
  vector<long> cpuData(5);
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) +kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    int j{0};
    for (int i = 0; i < 22; i++)
    {
      if ((i >= 13 && i <= 16) || i == 21)
      {
        linestream >> cpuData[j];
        j++;
      }
      else
        linestream >> data;
    }
  }
  return cpuData;
}
