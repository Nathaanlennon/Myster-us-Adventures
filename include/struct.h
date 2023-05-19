//
// Created by cacac on 12/05/2023.
//

#ifndef MYSTER_US_ADVENTURES_STRUCT_H
#define MYSTER_US_ADVENTURES_STRUCT_H

#include <stdio.h>
#include "../external/rgr/inc/libGameRGR2.h"

typedef struct {
    int number;
    char symbol[10];
    int weapon;

    int start_x; //case de départ attribuée abscisse
    int start_y; //case de départ attribuée ordonnée
    int position_x; //position durant le tour abscicsse
    int position_y; //position durant le tour abscicsse

    int ancientWeapon_found;
    int treasure_found;

    char name[];
    //color mais j'ai la flemme pour l'instant
} Player;

typedef struct {
    int number;
    char symbol[10];
    int weapon;

    int start_x; //case de départ attribuée abscisse
    int start_y; //case de départ attribuée ordonnée
    int position_x; //position durant le tour abscicsse
    int position_y; //position durant le tour abscicsse

    int ancientWeapon_found;
    int treasure_found;

    char name[];
    //color mais j'ai la flemme pour l'instant
} Player2;

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
    Player2 player;
    Colors colors;
} Data;
#endif //MYSTER_US_ADVENTURES_STRUCT_H


