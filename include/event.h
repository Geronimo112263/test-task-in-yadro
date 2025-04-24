#ifndef _SRC_EVENT_H_
#define _SRC_EVENT_H_

#include "time.h"
#include <vector>

struct Event{
    Time timeEvent;
    int idEvent;    
    std::vector<std::string> args;

    Event() = default;
    Event(const std::string &str);
    Event(const Time& time, int id, const std::vector<std::string>& args);
};

#endif