#include "../include/event.h"

#include <sstream>

#include "../include/exceptions_class.h"

Event::Event(const std::string& str) {
  std::stringstream iss(str);
  std::string t;
  int id;

  if (!(iss >> t >> id)) {
    throw InvalidEventFormatException(str);
  }

  idEvent = id;
  timeEvent.fromString(t);

  std::string arg;
  while (iss >> arg) {
    args.push_back(arg);
  }
}

Event::Event(const Time& time, int id, const std::vector<std::string>& args)
    : timeEvent(time), idEvent(id), args(args) {}