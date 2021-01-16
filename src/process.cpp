#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid):pid_(pid)
{
    user_ = LinuxParser::User(pid);
    command_ = LinuxParser::Command(pid);
}

int Process::Pid() { return pid_;  }

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

string Process::Command() { return command_; }

string Process::Ram() {  return LinuxParser::Ram(pid_); }

string Process::User() { return user_; }

long int Process::UpTime() {return LinuxParser::UpTime(pid_); }

bool Process::operator<(Process const& a) const {
  bool less_than = CpuUtilization() < a.CpuUtilization();
  return less_than;
 }

 bool Process::Compare (Process &a, Process &b) {return b<a;}
