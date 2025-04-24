#include "../include/parser_file.h"

#include <fstream>
#include <iostream>
#include <sstream>

ParserFile::ParserFile(const std::string& filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    throw std::runtime_error("Failed to open file: " + filename);
  }

  std::string line;

  if (!std::getline(file, line))
    throw std::runtime_error("Missing number of tables");

  int numberOfTables;

  try {
    numberOfTables = std::stoi(line);
  } catch (...) {
    throw std::runtime_error("Invalid number of tables");
  }

  if (!std::getline(file, line))
    throw std::runtime_error("Missing open and close time");

  std::stringstream iss(line);
  std::string open, close;

  iss >> open >> close;
  Time openTime(open);
  Time closeTime(close);

  if (!std::getline(file, line))
    throw std::runtime_error("Missing price per hour");
  int price;
  try {
    price = std::stoi(line);
  } catch (...) {
    throw std::runtime_error("Invalid price format");
  }

  ComputerClub tmp(numberOfTables, openTime, closeTime, price);

  while (std::getline(file, line)) {
    if (line.empty()) continue;

    try {
      Event event(line);
      tmp.handleEvent(event);
    } catch (const std::exception& ex) {
      std::cerr << "Error parsing event: " << ex.what() << std::endl;
    }
  }
  club = std::move(tmp);
}

void ParserFile::print() { club.printResults(); }