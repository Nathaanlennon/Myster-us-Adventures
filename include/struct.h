//
// Created by cacac on 12/05/2023.
//

#ifndef MYSTER_US_ADVENTURES_STRUCT_H
#define MYSTER_US_ADVENTURES_STRUCT_H

#include <stdio.h>
#include "../external/rgr/inc/libGameRGR2.h"

typedef struct {
    int x;
    int y;
} Player;
typedef struct {
    short colors_pair[20][2];
    int color_num;
}Colors;
typedef enum {
    MAIN_MENU,
    GAME,
    HIGHS_SCORE
} Scene;
typedef struct {
    int x;
    int y;
    short color;
    short background;
} Cursor;
typedef struct Data {
    Screen *screen;
    Scene scene_type;
    Cursor cursor;
    Player player;
    Colors colors;
    int _bool;
    char* buffer;
} Data;
#endif //MYSTER_US_ADVENTURES_STRUCT_H


