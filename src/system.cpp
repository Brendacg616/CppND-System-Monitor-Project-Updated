#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <algorithm>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

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

float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

std::string System::OperatingSystem() { return os_; }

int System::RunningProcesses() {return LinuxParser::RunningProcesses(); }

int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

long int System::UpTime() { return LinuxParser::UpTime(); }
