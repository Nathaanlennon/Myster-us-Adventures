#ifndef MYSTER_US_ADVENTURES_GAMEPLAY_H
#define MYSTER_US_ADVENTURES_GAMEPLAY_H
#include <stdio.h>

#include "struct.h"

void weapon_choice(Player *player);
void choose_coordinates(Square **board, int* x, int* y, int boardSize, int gridSize);
void choose_direction(Square **board, Player player, int* x, int* y, int boardSize);
int fight(Player player, int monster);
int event_ancient_weapon(Player* player, int ancientWeapon);
int event_manager(int* x, int* y, Square **board, int boardSize, int gridSize, Player* player, const Entity monsters[], const Entity weapons[], const Entity treasures[]);
void reset(Square **board, int boardSize, int gridSize, Player* player);
void turn(Square **board, int boardSize, int gridSize, Player* player, const Entity monsters[], const Entity weapons[], const Entity treasures[]);


#endif //MYSTER_US_ADVENTURES_GAMEPLAY_H
