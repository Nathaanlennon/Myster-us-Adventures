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
void flush_input_buffer();
int getint();
void commentary(char tab[]);
void waiting();
void set_color(Data *data, char *code);
int get_color_pair(Data *data);
void draw_printf(Data* data, const char *format, ...);
#endif //MYSTER_US_ADVENTURES_USUAL_H
