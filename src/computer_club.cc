#include "../include/computer_club.h"

#include <iomanip>
#include <iostream>
#include <sstream>

#include "../include/exceptions_class.h"

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
    case ARRIVE: {
      handleClientCome(event, args);
      break;
    }
    case SIT: {
      handleClientSit(event, args);
      break;
    }
    case WAIT: {
      handleClientWait(event, args);
      break;
    }
    case LEAVE: {
      handleClientLeave(event, args);
      break;
    }
    default:
      allEvents_.emplace_back(
          Event{event.timeEvent,
                SYS_ERROR,
                {"There is no event number " + std::to_string(event.idEvent)}});
      break;
  }
}

void ComputerClub::handleClientCome(const Event &event,
                                    const std::vector<std::string> &args) {
  const std::string &name = args[0];

  if (clients_.count(name)) {
    allEvents_.emplace_back(
        Event{event.timeEvent, SYS_ERROR, {"YouShallNotPass"}});
    return;
  }

  if (openTime_ > event.timeEvent) {
    allEvents_.emplace_back(Event{event.timeEvent, SYS_ERROR, {"NotOpenYet"}});
    return;
  }

  clients_.insert({name, Client(name)});
}

void ComputerClub::handleClientSit(const Event &event,
                                   const std::vector<std::string> &args) {
  const std::string &name = args[0];
  int numsTable;

  try {
    numsTable = std::stoi(args[1]);
  } catch (...) {
    allEvents_.emplace_back(
        Event{event.timeEvent, SYS_ERROR, {"InvalidTableNumber"}});
    return;
  }

  if (numsTable <= 0 || numsTable > (int)tables_.size()) {
    allEvents_.emplace_back(
        Event{event.timeEvent, event.idEvent, {"InvalidTableNumber"}});
    return;
  }

  if (!clients_.count(name)) {
    allEvents_.emplace_back(
        Event{event.timeEvent, SYS_ERROR, {"ClientUnknown"}});
    return;
  }

  if (clients_[name].occupyTable == numsTable ||
      tables_[numsTable - 1].occupy) {
    allEvents_.emplace_back(Event{event.timeEvent, SYS_ERROR, {"PlaceIsBusy"}});
    return;
  }

  if (clients_[name].occupyTable > 0) {
    tables_[clients_[name].occupyTable - 1].occupy = false;
    tables_[clients_[name].occupyTable - 1].workingHours +=
        clients_[name].sitToTable - event.timeEvent;
    tables_[clients_[name].occupyTable - 1].income +=
        (clients_[name].sitToTable - event.timeEvent + HOUR - 1) / HOUR;
  }

  clients_[name].sitToTable = event.timeEvent;
  clients_[name].occupyTable = numsTable;
  tables_[numsTable - 1].occupy = true;
}

void ComputerClub::handleClientWait(const Event &event,
                                    const std::vector<std::string> &args) {
  const std::string &name = args[0];

  if ((int)waitClients_.size() > numberOfTables_) {
    allEvents_.emplace_back(Event{event.timeEvent, SYS_LEAVE, {name}});
    handleClientLeave(event, args);
    return;
  }

  for (auto &table : tables_) {
    if (!table.occupy) {
      allEvents_.emplace_back(
          Event{event.timeEvent, SYS_ERROR, {"ICanWaitNoLonger!"}});
      return;
    }
  }

  waitClients_.push_back(event);
}

void ComputerClub::handleClientLeave(const Event &event,
                                     const std::vector<std::string> &args) {
  const std::string &name = args[0];

  if (!clients_.count(name)) {
    allEvents_.emplace_back(
        Event{event.timeEvent, SYS_ERROR, {name, "ClientUnknown"}});
    return;
  }

  const int tableNumber = clients_[name].occupyTable;

  if (tableNumber > 0) {
    tables_[tableNumber - 1].occupy = false;

    if (!waitClients_.empty()) {
      auto eventWait = waitClients_.front();
      waitClients_.pop_front();

      const std::string &nextClient = eventWait.args[0];

      if (!clients_.count(nextClient)) {
        clients_[nextClient] = Client(nextClient);
      }

      clients_[nextClient].occupyTable = tableNumber;
      clients_[nextClient].sitToTable = event.timeEvent;
      tables_[tableNumber - 1].occupy = true;

      allEvents_.emplace_back(Event{
          event.timeEvent, SYS_SIT, {nextClient, std::to_string(tableNumber)}});
    }

    tables_[clients_[name].occupyTable - 1].workingHours +=
        clients_[name].sitToTable - event.timeEvent;
    tables_[clients_[name].occupyTable - 1].income +=
        (clients_[name].sitToTable - event.timeEvent + HOUR - 1) / HOUR;
  }

  clients_.erase(name);
}

void ComputerClub::printResults() {
  std::cout << openTime_.toString() << std::endl;

  for (const auto &event : allEvents_) {
    std::cout << event.timeEvent.toString() << " " << event.idEvent;
    for (const auto &arg : event.args) {
      std::cout << " " << arg;
    }
    std::cout << std::endl;
  }

  for (const auto &client : clients_) {
    const std::string &name(client.first);
    std::cout << closeTime_.toString() << " " << SYS_LEAVE << " "
              << client.first << std::endl;

    const int index = clients_[name].occupyTable - 1;
    tables_[index].workingHours += clients_[name].sitToTable - closeTime_;
    tables_[index].income +=
        (clients_[name].sitToTable - closeTime_ + HOUR - 1) / HOUR;
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
  if (index > 0 && static_cast<size_t>(index) <= tables_.size()) {
    return tables_[index - 1];
  } else {
    throw InvalidTableIndexException(index);
  }
}

size_t ComputerClub::getWaitQueueSize() const { return waitClients_.size(); }
