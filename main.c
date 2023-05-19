#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "include/macro.h"
#include "playerturn.c"
#include "init.c"
#include "usual.c"
#include "include/header.h"


int main() {
    srand(time(NULL));

    const char weapons[4][10] = {STICK, SWORD, DAGGER, SPELLBOOK}; //Symboles des armes
    const char treasures[5][10] = {CHEST, PORTAL, TOTEM, TOTEM, CHEST}; //Symboles des objets spéciaux (coffres, totems, portails)
    const char monsters[4][10] = {ZOMBIE, BASILISK, TROLL, HARPY}; //Symboles des monstres
    const char adventurers[4][10] = {RANGER, THIEF, MAGICIAN, WARRIOR}; //Symboles des monstres

    const int start_x[4] = {0, 2, 4, 6}; //abscisses des cases de départ de chaque joueur
    const int start_y[4] = {4, 0, 6, 2}; //ordonnées des cases de départ de chaque joueur

    ////////////        CREATION ET INITIALISATION DU PLATEAU DE CASES ////////////
    Square **board = create_board(BOARD_SIZE, GRID_SIZE, monsters, weapons, treasures);

    ////////////        CREATION PERSONNAGE        ////////////

    Player players[4]; //liste des joueurs
    for(int i=0; i<4; i++){  //initialisation de chaque joueur
        init_player(&players[i], i+1, adventurers[i], start_x[i], start_y[i]);
        printf("Bienvenue %s !\n", players[i].name);
    }

    ////////////        A SUPPRIMER, UNIQUEMENT POUR TESTER        ////////////
    print_board_admin(board, BOARD_SIZE, &players[0]); //print board mais on voit toutes les cases
    printf("\n\n");

    print_board(board, BOARD_SIZE, &players[0]);

    ////////////        WIP TEST GAMEPLAY        ////////////
    for (int i = 0; i < 40; i++) {
        weapon_choice(&players[0]);
        move(board, BOARD_SIZE, GRID_SIZE, &players[0], monsters, weapons, treasures);
    }

    ////////////        LIBERATION DE LA MEMOIRE        ////////////
    free_board(board, BOARD_SIZE);

    return 0;
}
