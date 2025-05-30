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
  if (event.getArgs().size() > 2) {
    throw IncorrectNumberOfArgumentsException();
  }

  allEvents_.push_back(event);
  const auto &args = event.getArgs();

  switch (event.getIdEvent()) {
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
          Event{event.getTimeEvent(),
                SYS_ERROR,
                {NOT_EVENT + std::to_string(event.getIdEvent())}});
      break;
  }
}

void ComputerClub::handleClientCome(const Event &event,
                                    const std::vector<std::string> &args) {
  if (event.getArgs().size() != 1) {
    throw IncorrectNumberOfArgumentsException();
  }

  const std::string &name = args[0];

  if (clients_.count(name)) {
    allEvents_.emplace_back(Event{event.getTimeEvent(), SYS_ERROR, {NOT_PASS}});
    return;
  }

  if (openTime_ > event.getTimeEvent()) {
    allEvents_.emplace_back(Event{event.getTimeEvent(), SYS_ERROR, {NOT_OPEN}});
    return;
  }

  clients_.insert({name, Client(name)});
}

void ComputerClub::handleClientSit(const Event &event,
                                   const std::vector<std::string> &args) {
  if (event.getArgs().size() != 2) {
    throw IncorrectNumberOfArgumentsException();
  }

  const std::string &name = args[0];
  int numsTable;

  try {
    numsTable = std::stoi(args[1]);
  } catch (...) {
    allEvents_.emplace_back(
        Event{event.getTimeEvent(), SYS_ERROR, {INVALID_TABLE}});
    return;
  }

  if (numsTable <= 0 || numsTable > (int)tables_.size()) {
    allEvents_.emplace_back(
        Event{event.getTimeEvent(), event.getIdEvent(), {INVALID_TABLE}});
    return;
  }

  if (!clients_.count(name)) {
    allEvents_.emplace_back(
        Event{event.getTimeEvent(), SYS_ERROR, {CLIEN_UNKNOWN}});
    return;
  }

  if (clients_[name].occupyTable == numsTable ||
      tables_[numsTable - 1].occupy) {
    allEvents_.emplace_back(Event{event.getTimeEvent(), SYS_ERROR, {IS_BUSY}});
    return;
  }

  if (clients_[name].occupyTable > 0) {
    tables_[clients_[name].occupyTable - 1].occupy = false;
    tables_[clients_[name].occupyTable - 1].workingHours +=
        clients_[name].timeToSit - event.getTimeEvent();
    tables_[clients_[name].occupyTable - 1].income +=
        (clients_[name].timeToSit - event.getTimeEvent() + HOUR - 1) / HOUR;
  }

  clients_[name].timeToSit = event.getTimeEvent();
  clients_[name].occupyTable = numsTable;
  tables_[numsTable - 1].occupy = true;
}

void ComputerClub::handleClientWait(const Event &event,
                                    const std::vector<std::string> &args) {
  const std::string &name = args[0];

  if ((int)waitClients_.size() > numberOfTables_) {
    allEvents_.emplace_back(Event{event.getTimeEvent(), SYS_LEAVE, {name}});
    handleClientLeave(event, args);
    return;
  }

  for (auto &table : tables_) {
    if (!table.occupy) {
      allEvents_.emplace_back(
          Event{event.getTimeEvent(), SYS_ERROR, {NOT_LONGER}});
      return;
    }
  }

  waitClients_.push_back(event);
}

void ComputerClub::handleClientLeave(const Event &event,
                                     const std::vector<std::string> &args) {
  if (event.getArgs().size() != 1) {
    throw IncorrectNumberOfArgumentsException();
  }

  const std::string &name = args[0];

  if (!clients_.count(name)) {
    allEvents_.emplace_back(
        Event{event.getTimeEvent(), SYS_ERROR, {name, CLIEN_UNKNOWN}});
    return;
  }

  const int tableNumber = clients_[name].occupyTable;

  if (tableNumber > 0) {
    tables_[tableNumber - 1].occupy = false;

    if (!waitClients_.empty()) {
      auto eventWait = waitClients_.front();
      waitClients_.pop_front();

      const std::string &nextClient = eventWait.getArgs()[0];

      if (!clients_.count(nextClient)) {
        clients_[nextClient] = Client(nextClient);
      }

      clients_[nextClient].occupyTable = tableNumber;
      clients_[nextClient].timeToSit = event.getTimeEvent();
      tables_[tableNumber - 1].occupy = true;

      allEvents_.emplace_back(Event{event.getTimeEvent(),
                                    SYS_SIT,
                                    {nextClient, std::to_string(tableNumber)}});
    }

    tables_[clients_[name].occupyTable - 1].workingHours +=
        clients_[name].timeToSit - event.getTimeEvent();
    tables_[clients_[name].occupyTable - 1].income +=
        (clients_[name].timeToSit - event.getTimeEvent() + HOUR - 1) / HOUR;
  }

  clients_.erase(name);
}

void ComputerClub::printResults() {
  std::cout << openTime_.toString() << std::endl;

  eventInformation();
  profitInformation();

  std::cout << closeTime_.toString() << std::endl;

  for (const auto &table : tables_) {
    std::cout << getIncomeReport(table) << std::endl;
  }
}

void ComputerClub::eventInformation() {
  for (const auto &event : allEvents_) {
    std::cout << event.getTimeEvent().toString() << " " << event.getIdEvent();
    for (const auto &arg : event.getArgs()) {
      std::cout << " " << arg;
    }
    std::cout << std::endl;
  }
}

void ComputerClub::profitInformation() {
  for (const auto &client : clients_) {
    const std::string &name(client.first);
    std::cout << closeTime_.toString() << " " << SYS_LEAVE << " "
              << client.first << std::endl;

    const int index = clients_[name].occupyTable - 1;
    tables_[index].workingHours += clients_[name].timeToSit - closeTime_;
    tables_[index].income +=
        (clients_[name].timeToSit - closeTime_ + HOUR - 1) / HOUR;
  }
}

std::string ComputerClub::getIncomeReport(const Table &table) const {
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

size_t ComputerClub::getWaitClientsCount() const { return waitClients_.size(); }
