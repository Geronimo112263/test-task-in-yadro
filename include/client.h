#ifndef _SRC_CLIENT_H_
#define _SRC_CLIENT_H_

#include "time.h"

struct Client{
    std::string clientName;
    int occupyTable = -1;
    Time sitToTable;

    Client() = default;
    Client(const std::string &name) : clientName(name){};    
};

#endif