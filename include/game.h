#ifndef MYSTER_US_ADVENTURES_GAME_H
#define MYSTER_US_ADVENTURES_GAME_H
#include <stdio.h>
#include "struct.h"

int number_players();
void print_board(Square **board, int boardSize, Player player);
void print_board_total(Square **board, int boardSize);
int choose_board_dimensions();
void game(int boardSize, int gridSize, Player players[], int n, const Entity weapons[], const Entity monsters[], const Entity treasures[]);
void launch_game();

#endif //MYSTER_US_ADVENTURES_GAME_H
