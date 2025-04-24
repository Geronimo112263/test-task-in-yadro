#include "../include/computer_club.h"

#include <iomanip>
#include <iostream>
#include <sstream>

ComputerClub::ComputerClub(int n, Time open, Time close, int price)
    : numberOfTables_(n),
      openTime_(open),
      closeTime_(close),
      priceOfHour(price) {
  for (int i = 0; i < n; i++) {
    tables_.emplace_back(i + 1);
  }
}

void ComputerClub::handleEvent(const Event &event) {
  allEvents_.push_back(event);

  const auto &args = event.args;
  switch (event.idEvent) {
    case 1: {
      handleClientCome(event, args);
      break;
    }
    case 2: {
      handleClientSit(event, args);
      break;
    }
    case 3: {
      handleClientWait(event, args);
      break;
    }
    case 4: {
      handleClientLeave(event, args);
      break;
    }
    default:
      break;
  }
}

void ComputerClub::handleClientCome(const Event &event,
                                    const std::vector<std::string> &args) {
  std::string name = args[0];

  if (clients_.count(name)) {
    allEvents_.emplace_back(Event{event.timeEvent, 13, {"YouShallNotPass"}});
    return;
  }

  if (openTime_ > event.timeEvent) {
    allEvents_.emplace_back(Event{event.timeEvent, 13, {"NotOpenYet"}});
    return;
  }

  clients_.insert({name, Client(name)});
}

void ComputerClub::handleClientSit(const Event &event,
                                   const std::vector<std::string> &args) {
  std::string name = args[0];
  int numsTable;
  try {
    numsTable = std::stoi(args[1]);
  } catch (...) {
    allEvents_.emplace_back(Event{event.timeEvent, 13, {"InvalidTableNumber"}});
    return;
  }

  if (numsTable <= 0 || numsTable > (int)tables_.size()) {
    allEvents_.emplace_back(
        Event{event.timeEvent, event.idEvent, {"InvalidTableNumber"}});
    return;
  }

  if (!clients_.count(name)) {
    allEvents_.emplace_back(Event{event.timeEvent, 13, {"ClientUnknown"}});
    return;
  }

  if (clients_[name].occupyTable == numsTable ||
      tables_[numsTable - 1].occupy) {
    allEvents_.emplace_back(Event{event.timeEvent, 13, {"PlaceIsBusy"}});
    return;
  }

  if (clients_[name].occupyTable > 0) {
    tables_[clients_[name].occupyTable - 1].occupy = false;
    tables_[clients_[name].occupyTable - 1].workingHours +=
        clients_[name].sitToTable - event.timeEvent;
    tables_[clients_[name].occupyTable - 1].income +=
        (clients_[name].sitToTable - event.timeEvent + 59) / 60;
  }

  clients_[name].sitToTable = event.timeEvent;
  clients_[name].occupyTable = numsTable;
  tables_[numsTable - 1].occupy = true;
}

void ComputerClub::handleClientWait(const Event &event,
                                    const std::vector<std::string> &args) {
  std::string name = args[0];

  if ((int)waitClients_.size() > numberOfTables_) {
    allEvents_.emplace_back(Event{event.timeEvent, 11, {name}});
    handleClientLeave(event, args);
    return;
  }

  for (auto &table : tables_) {
    if (!table.occupy) {
      allEvents_.emplace_back(
          Event{event.timeEvent, 13, {"ICanWaitNoLonger!"}});
      return;
    }
  }

  waitClients_.push_back(event);
}

void ComputerClub::handleClientLeave(const Event &event,
                                     const std::vector<std::string> &args) {
  std::string name = args[0];
  if (!clients_.count(name)) {
    allEvents_.emplace_back(
        Event{event.timeEvent, 13, {name, "ClientUnknown"}});
    return;
  }

  int tableNumber = clients_[name].occupyTable;

  if (tableNumber > 0) {
    tables_[tableNumber - 1].occupy = false;
    if (!waitClients_.empty()) {
      auto eventWait = waitClients_.front();
      waitClients_.pop_front();
      std::string nextClient = eventWait.args[0];
      if (!clients_.count(nextClient)) {
        clients_[nextClient] = Client(nextClient);
      }
      clients_[nextClient].occupyTable = tableNumber;
      clients_[nextClient].sitToTable = event.timeEvent;
      tables_[tableNumber - 1].occupy = true;
      allEvents_.emplace_back(Event{
          event.timeEvent, 12, {nextClient, std::to_string(tableNumber)}});
    }
    tables_[clients_[name].occupyTable - 1].workingHours +=
        clients_[name].sitToTable - event.timeEvent;
    tables_[clients_[name].occupyTable - 1].income +=
        (clients_[name].sitToTable - event.timeEvent + 59) / 60;
  }

  clients_.erase(name);
}

void ComputerClub::printResults() {
  std::cout << openTime_.toString() << std::endl;

  for (const auto &event : allEvents_) {
    std::cout << event.timeEvent.toString() << " " << event.idEvent;
    for (size_t i = 0; i < event.args.size(); i++) {
      std::cout << " " << event.args[i];
    }
    std::cout << std::endl;
  }

  for (const auto &client : clients_) {
    std::string name(client.first);
    std::cout << closeTime_.toString() << " 11 " << client.first << std::endl;
    tables_[clients_[name].occupyTable - 1].workingHours +=
        clients_[name].sitToTable - closeTime_;
    tables_[clients_[name].occupyTable - 1].income +=
        (clients_[name].sitToTable - closeTime_ + 59) / 60;
  }

  std::cout << closeTime_.toString() << std::endl;
  for (const auto &table : tables_) {
    std::cout << timeAndIncomeUseTable(table) << std::endl;
  }
}

std::string ComputerClub::timeAndIncomeUseTable(const Table &table) const {
  std::stringstream iss;
  iss << table.tableNumber << " " << table.income * priceOfHour << " "
      << Time(table.workingHours).toString();
  return iss.str();
}

const std::vector<Event> &ComputerClub::getAllEvents() const {
  return allEvents_;
}

const Table &ComputerClub::getTable(int index) const {
  return tables_[index - 1];
}

size_t ComputerClub::getWaitQueueSize() const { return waitClients_.size(); }
