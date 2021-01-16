#ifndef PROCESSOR_H
#define PROCESSOR_H

class CPUData
{
public:
      int user{0};
      int nice{0};
      int system{0};
      int idle{0};
      int iowait{0};
      int irq{0};
      int softirq{0};
      int steal{0};
      int guest{0};
      int guest_nice{0};
      int Idle{0};
      int NonIdle{0};
      int Total{0};
      void GetDataFromString();
};

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp
 private:
  CPUData prev_, current_;
};

#endif
