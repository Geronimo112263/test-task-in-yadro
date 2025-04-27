#ifndef _INCLUDE_TABLE_H_
#define _INCLUDE_TABLE_H_

struct Table {
  int tableNumber;
  bool occupy = false;
  int income = 0;
  int workingHours = 0;

  Table() = default;
  explicit Table(int number) : tableNumber(number) {};
};

#endif