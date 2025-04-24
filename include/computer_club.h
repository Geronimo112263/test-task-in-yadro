#ifndef _SRC_COMPUTER_CLUB_H_
#define _SRC_COMPUTER_CLUB_H_

#include "client.h"
#include "event.h"
#include "table.h"

#include <map>
#include <list>

class ComputerClub{
public:
    ComputerClub() = default;
    ComputerClub(int n, Time open, Time close, int price);
    void handleEvent(const Event&);
    void printResults();

    const std::vector<Event>& getAllEvents() const;
    const Table& getTable(int index) const;
    size_t getWaitQueueSize() const;
private:
    int numberOfTables_;
    Time openTime_, closeTime_;
    int priceOfHour;
    std::vector<Table> tables_;
    std::map<std::string, Client> clients_;
    std::list<Event> waitClients_;
    std::vector<Event> allEvents_;
    std::map<int, std::pair<int, Time>> revenuePerTablePerDay_;

    void handleClientCome(const Event &event, const std::vector<std::string> &args);
    void handleClientSit(const Event &event, const std::vector<std::string> &args);
    void handleClientWait(const Event &event, const std::vector<std::string> &args);
    void handleClientLeave(const Event &event, const std::vector<std::string> &args);
    std::string timeAndIncomeUseTable(const Table &table)const;
};

#endif