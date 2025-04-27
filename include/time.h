#ifndef _INCLUDE_TIME_H_
#define _INCLUDE_TIME_H_

#include <string>

struct Time {
  int minutes = 0;

  Time() = default;
  explicit Time(const std::string &str);
  explicit Time(const int &minute);

  void fromString(const std::string &str);
  std::string toString() const;

  bool operator>(const Time &other) const;
  int operator-(const Time &other) const;
  int operator+(const Time &other) const;
};

#endif