#include "../include/parser_file.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include "../include/exceptions_class.h"

ParserFile::ParserFile(const std::string& filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    throw ClubException(FILE_NOT_FOUND + filename);
  }

  std::string line;
  if (!std::getline(file, line)) throw ClubException(MISSING_TABLES);

  int numberOfTables;
  try {
    numberOfTables = std::stoi(line);
  } catch (...) {
    throw ClubException(INVALID_TABLES);
  }

  if (numberOfTables <= 0) {
    throw ClubException(INVALID_TABLES);
  }

  if (!std::getline(file, line)) throw ClubException(INVALID_TIME);

  std::stringstream iss(line);
  std::string open;
  std::string close;
  iss >> open >> close;

  Time openTime(open);
  Time closeTime(close);

  if (!std::getline(file, line)) throw ClubException(INVALID_PPH);

  int price;
  try {
    price = std::stoi(line);
  } catch (...) {
    throw ClubException(INVALID_PRICE);
  }

  ComputerClub tmp(numberOfTables, openTime, closeTime, price);
  while (std::getline(file, line)) {
    if (line.empty()) continue;

    try {
      Event event(line);
      tmp.handleEvent(event);
    } catch (const std::exception& ex) {
      std::cerr << ERROR_EVENT << ex.what() << std::endl;
    }
  }

  club = std::move(tmp);
}

void ParserFile::print() { club.printResults(); }