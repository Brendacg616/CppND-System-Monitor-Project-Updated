#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

System::System()
{
  kernel_ = LinuxParser::Kernel();
  os_ = LinuxParser::OperatingSystem();
}
Processor& System::Cpu() { return cpu_; }

vector<Process>& System::Processes() {
  auto pids = LinuxParser::Pids();
    vector<Process> temp_processes;
    for(int pid : pids)
    {
        temp_processes.emplace_back(pid);
    }
    std::sort(temp_processes.begin(), temp_processes.end(), Process::Compare);
    processes_ = temp_processes;
    return processes_;
}

std::string System::Kernel() { return kernel_; }

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// TODO: Return the operating system name
std::string System::OperatingSystem() { return os_; }

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() {return LinuxParser::RunningProcesses(); }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }
