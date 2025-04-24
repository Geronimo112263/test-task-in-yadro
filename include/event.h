#ifndef _INCLUDE_EVENT_H_
#define _INCLUDE_EVENT_H_

#include <vector>

#include "time.h"

struct Event {
  Time timeEvent;
  int idEvent;
  std::vector<std::string> args;

  Event() = default;
  Event(const std::string& str);
  Event(const Time& time, int id, const std::vector<std::string>& args);
};

#endif