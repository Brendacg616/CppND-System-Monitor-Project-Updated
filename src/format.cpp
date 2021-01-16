#include <string>

#include "format.h"

using std::string;

string Format::ElapsedTime(long seconds) {
  std::stringstream hh,mm,ss;
  std::string time;
  int h, m, s;
  h = (seconds) / pow(60,2);
  hh << std::setw(2) << std::setfill('0') << h;
  time += hh.str() + ':';
  m = (seconds % (60*60))/60;
  mm << std::setw(2) << std::setfill('0') << m;
  time += mm.str() + ':';
  s = seconds%60;
  ss << std::setw(2) << std::setfill('0') << s;
  time += ss.str();
  return time;
}
