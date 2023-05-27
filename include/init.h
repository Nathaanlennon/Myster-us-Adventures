#ifndef MYSTER_US_ADVENTURES_INIT_H
#define MYSTER_US_ADVENTURES_INIT_H

#include <stdio.h>
#include "struct.h"

void random_placement(Square **board, int gridSize, const Entity tab[], int tabSize, const Entity monsters[]);
void initialize_map(Square **board, int boardSize, int gridSize, const Entity monsters[], const Entity weapons[], const Entity treasures[]);
Square **create_board(int boardSize, int gridSize, const Entity monsters[], const Entity weapons[], const Entity treasures[]);
void free_board(Square **board, int size);
void init_player(Player* player, int num, const Entity symbol, int start_x, int start_y);


#endif //MYSTER_US_ADVENTURES_INIT_H
