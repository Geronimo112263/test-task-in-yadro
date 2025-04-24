#ifndef _INCLUDE_PARSER_FILE_H_
#define _INCLUDE_PARSER_FILE_H_

#include "computer_club.h"

class ParserFile {
 public:
  ParserFile() = default;
  ParserFile(const std::string& filename);
  void print();

 private:
  ComputerClub club;
};

#endif