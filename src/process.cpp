#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid):pid_(pid)
{
    user_ = LinuxParser::User(pid);
    command_ = LinuxParser::Command(pid);
}

// TODO: Return this process's ID
int Process::Pid() { return pid_;  }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization()  const{
    long uptime = LinuxParser::UpTime();
    auto proc_data = LinuxParser::CpuUtilization(pid_);
    float cpu_usage {0.0};
    long utime = proc_data[0];
    long stime = proc_data[1];
    long starttime = proc_data[4];
    long total_time = utime + stime;
    long seconds = uptime - (starttime / sysconf(_SC_CLK_TCK));
    cpu_usage = ((1.0 *total_time / sysconf(_SC_CLK_TCK))/seconds);
    return cpu_usage;
}
// TODO: Return the command that generated this process
string Process::Command() { return command_; }

// TODO: Return this process's memory utilization
string Process::Ram() {  return LinuxParser::Ram(pid_); }

// TODO: Return the user (name) that generated this process
string Process::User() { return user_; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() {return LinuxParser::UpTime(pid_); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a[[maybe_unused]]) const {
  bool less_than = CpuUtilization() < a.CpuUtilization();
  return less_than;
 }

 bool Process::Compare (Process &a, Process &b) {return b<a;}
