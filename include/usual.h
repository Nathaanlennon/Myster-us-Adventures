#ifndef MYSTER_US_ADVENTURES_USUAL_H
#define MYSTER_US_ADVENTURES_USUAL_H
#include <stdio.h>
#include "struct.h"
void clear_all();
void clear_part(int line, int column);
void cursor_move(char direction, int num);
int setBlockingFD(int fileDescriptor, int blocking);
void discardInput(void);
int getint();
void commentary(char tab[]);
void waiting();
void write_crash_report(const char* error_message);
int SymbolIdInArray(Square square, const Entity array[], int size);
void background(int i, int j);
int CheckSquareInvalid(Square square);
int CheckIndexOutOfArray(int i, int arraySize);
long get_time();
int compare_time(long t1, long t2);
void format_time(int seconds);
#endif //MYSTER_US_ADVENTURES_USUAL_H