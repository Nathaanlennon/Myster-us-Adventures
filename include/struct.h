//
// Created by cacac on 12/05/2023.
//

#ifndef MYSTER_US_ADVENTURES_STRUCT_H
#define MYSTER_US_ADVENTURES_STRUCT_H

#include <stdio.h>


typedef struct {
    int number; //numéro du joueur
    char symbol[10]; //symbole du joueur
    int weapon; //arme antique attribuée

    int start_x; //case de départ attribuée abscisse
    int start_y; //case de départ attribuée ordonnée
    int position_x; //position durant le tour abscicsse
    int position_y; //position durant le tour abscicsse

    int score; //booléen qui vaudra 1 si la victoire est remportée, 0 sinon
    int flip_cards; //nombre de cartes retournées au cours de la partie
    int killed_monsters; //nombre de monstres tués au cours de la partie
    int total_treasures; //nombre de trésors trouvés au cours de la partie

    int ancientWeapon_found; //booléen si l'arme antique a été trouvée (condition nécessaire à la victoire)
    int treasure_found; //booléen si au moins 1 trésor a été trouvé (condition nécessaire à la victoire)

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
    int position_y; //position durant le tour ordonnée

    int ancientWeapon_found;
    int treasure_found;

    char name[];
    //color mais j'ai la flemme pour l'instant
} Player2;
// Structure pour une case du plateau de jeu
typedef struct {
    char symbol[10];
    int flipped; //variable booléenne qui vaut 1 si la case est retournée, 0 sinon
    int emptied; //variable booléenne qui vaut 1 si la case est vidée (monstre battu/objet pris), 0 sinon
} Square;
typedef struct {
    short colors_pair[20][2];
    int color_num;
}Colors;
typedef enum {
    MAIN_MENU,
    GAME,
    HIGHS_SCORE
} Scene;

#endif //MYSTER_US_ADVENTURES_STRUCT_H


