#include <iostream>

#include "include/parser_file.h"

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: ./club <input_file>" << std::endl;
    return 1;
  }

  try {
    ParserFile computerClub(argv[1]);
    computerClub.print();
  } catch (const std::exception& ex) {
    std::cerr << "Parser error: " << ex.what() << std::endl;
    return 1;
  }

  return 0;
}
