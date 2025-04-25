#ifndef _INCLUDE_EVENT_H_
#define _INCLUDE_EVENT_H_

#include <vector>

#include "time.h"

class Event {
 public:
  Event() = default;
  Event(const std::string& str);
  Event(const Time& time, int id, const std::vector<std::string>& args);

  const Time& getTimeEvent() const;
  int getIdEvent() const;
  const std::vector<std::string>& getArgs() const;

 private:
  Time timeEvent;
  int idEvent;
  std::vector<std::string> args;
};

#endif