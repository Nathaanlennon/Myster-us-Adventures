#ifndef MYSTER_US_ADVENTURES_SCOREMANAGER_H
#define MYSTER_US_ADVENTURES_SCOREMANAGER_H
#include <stdio.h>
#include "struct.h"

int printFile(char filename[]);
void updatePlayerStats(Player player);
void registerScores(Player players[], int size);

#endif //MYSTER_US_ADVENTURES_SCOREMANAGER_H
