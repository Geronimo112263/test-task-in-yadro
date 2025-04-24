#include "../include/time.h"

#include <iomanip>
#include <sstream>

Time::Time(const std::string &std) { fromString(std); }

Time::Time(const int &minute) : minutes(minute) {};

void Time::fromString(const std::string &str) {
  std::stringstream iss(str);
  int hour;
  int minute;
  char colon;

  if (!(iss >> hour >> colon >> minute) || hour < 0 || hour > 23 ||
      minute < 0 || minute > 59) {
    throw std::runtime_error("Invalid time format: " + str);
  }

  minutes = hour * 60 + minute;
}

std::string Time::toString() const {
  std::stringstream oss;
  oss << std::setw(2) << std::setfill('0') << minutes / 60 << ":"
      << std::setw(2) << std::setfill('0') << minutes % 60;
  return oss.str();
}

int Time::operator-(const Time &other) const {
  int result = minutes - other.minutes;
  return result >= 0 ? result : -result;
}

bool Time::operator>(const Time &other) const {
  return minutes > other.minutes;
}

int Time::operator+(const Time &other) const { return minutes + other.minutes; }