#include "../include/event.h"

#include <sstream>

Event::Event(const std::string& str) {
  std::stringstream iss(str);
  std::string t;
  int id;

  if (!(iss >> t >> id)) {
    throw std::runtime_error("Invalid event format: " + str);
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