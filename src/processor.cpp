#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
    int totald;
    int idled;
    current_.GetDataFromString();
    totald = current_.Total - prev_.Total;
    idled = current_.Idle - prev_.Idle;
    prev_ = current_;
    return (totald - idled)/(totald * 1.0);
}

void CPUData::GetDataFromString()
{
    std::vector<std::string> cpuDataStr = LinuxParser::CpuUtilization();
    user = std::stoi(cpuDataStr.at(1));
    nice = std::stoi(cpuDataStr.at(2));
    system = std::stoi(cpuDataStr.at(3));
    idle = std::stoi(cpuDataStr.at(4));
    iowait = std::stoi(cpuDataStr.at(5));
    irq = std::stoi(cpuDataStr.at(6));
    softirq = std::stoi(cpuDataStr.at(7));
    steal = std::stoi(cpuDataStr.at(8));
    guest = std::stoi(cpuDataStr.at(9));
    guest_nice = std::stoi(cpuDataStr.at(10));
    Idle = idle + iowait;
    NonIdle = user + nice +system + irq + softirq +steal;
    Total = Idle + NonIdle;
}
