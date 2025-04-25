#ifndef _INCLUDE_CLIENT_H_
#define _INCLUDE_CLIENT_H_

#include "time.h"

struct Client {
  std::string clientName;
  int occupyTable = -1;
  Time timeToSit;

  Client() = default;
  Client(const std::string &name) : clientName(name) {};
};

#endif