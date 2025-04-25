#ifndef _INCLUDE_EXCEPTIONS_CLASS_H_
#define _INCLUDE_EXCEPTIONS_CLASS_H_

#include <stdexcept>
#include <string>

class ClubException : public std::runtime_error {
 public:
  explicit ClubException(const std::string &message)
      : std::runtime_error(message) {}
};

class IncorrectNumberOfArgumentsException : public std::runtime_error {
 public:
  explicit IncorrectNumberOfArgumentsException()
      : std::runtime_error("Incorrect number of arguments") {};
};

class InvalidEventFormatException : public std::runtime_error {
 public:
  InvalidEventFormatException(const std::string &str)
      : std::runtime_error("Invalid event format: " + str) {};
};

class InvalidTimeFormatException : public std::runtime_error {
 public:
  InvalidTimeFormatException(const std::string &str)
      : std::runtime_error("Invalid time format: " + str) {};
};

class InvalidTableIndexException : public std::out_of_range {
 public:
  InvalidTableIndexException(const int &index)
      : std::out_of_range("Invalid table index: " + std::to_string(index)) {};
};

#endif