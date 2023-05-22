//
// Created by cacac on 12/05/2023.
//

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
void printFile(char filename[]);
#endif //MYSTER_US_ADVENTURES_USUAL_H
