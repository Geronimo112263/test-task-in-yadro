#ifndef _SRC_TABLE_H_
#define _SRC_TABLE_H_


struct Table{
    int tableNumber;
    bool occupy = false;
    int income = 0;
    int workingHours = 0;

    Table() = default;
    Table(int number) : tableNumber(number){};
};

#endif