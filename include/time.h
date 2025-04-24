#ifndef _SRC_TIME_H_
#define _SRC_TIME_H_

#include <string>

struct Time {
  int minutes = 0;

  Time() = default;
  Time(const std::string &str);
  Time(const int &minute);

  void fromString(const std::string &str);
  std::string toString() const;
  
  bool operator>(const Time &other) const;
  int operator-(const Time &other) const;
  int operator+(const Time &other) const;

};

#endif