#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/macro.h"
#include "include/struct.h"
#include "include/usual.h"

//placement aléatoire de symboles
void random_placement(Square **board, int gridSize, const Entity tab[], int tabSize, const Entity monsters[]){

    if(board == NULL){
        write_crash_report("pointer in parameters is NULL");
        exit(1);
    }

    int randomRow, randomCol;
    for (int i = 0; i < tabSize; i++) {
        do{
            randomRow = rand() % gridSize+1;
            randomCol = rand() % gridSize+1;
        }while(SymbolIdInArray(board[randomRow][randomCol], monsters, 4) == -1); //vérification que la case choisie aléatoirement ne soit pas déjà une arme/trésor
        strcpy(board[randomRow][randomCol].symbol.name, tab[i].name);
        strcpy(board[randomRow][randomCol].symbol.color, tab[i].color);
    }
}

//Initialisation de la map aléatoire de symboles
void initialize_map(Square **board, int boardSize, int gridSize, const Entity monsters[], const Entity weapons[], const Entity treasures[]) {

    if(board == NULL){
        write_crash_report("pointer in parameters is NULL");
        exit(1);
    }
    int count = 0; //compteur pour les couleurs des cases de départ associées à chaque joueur
    //remplissage du labyrinthe par des monstres aléatoires et initialisation de l'état de base du plateau (cartes retournées et remplies)
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            board[i][j].emptied = 0;
            board[i][j].flipped = 1;
            if((i == 0 && j == gridSize-1) || (i == 2 && j == 0) || (i == gridSize+1 && j == 2) || (i == gridSize-1 && j == gridSize+1)){
                strcpy(board[i][j].symbol.name, START); //placement des cases de départ
                if(count == 2){
                    strcpy(board[i][j].symbol.color, weapons[3].color);
                }
                else if (count == 3){
                    strcpy(board[i][j].symbol.color, weapons[2].color);
                }
                else{
                    strcpy(board[i][j].symbol.color, weapons[count].color);
                }
                count++;
            }
            else if((i%(boardSize-1) == 0) || (j%(boardSize-1) == 0)){
                strcpy(board[i][j].symbol.name, " "); //vide autour des cases de départ
                strcpy(board[i][j].symbol.color, C_BLK);
            }
            else{
                int randMon = rand() % 4;
                strcpy(board[i][j].symbol.name, monsters[randMon].name); //placement des monstres
                strcpy(board[i][j].symbol.color, monsters[randMon].color);
                board[i][j].flipped = 0;
            }
        }
    }

    //placement aléatoire des trésors
    random_placement(board, gridSize, treasures, 5, monsters);
    //placement aléatoire des armes antiques
    random_placement(board, gridSize, weapons, 4, monsters);
}

//Création et initialisation d'un tableau 2D de structures Square <=> création d'un plateau de jeu
Square **create_board(int boardSize, int gridSize, const Entity monsters[], const Entity weapons[], const Entity treasures[]) {
    Square **board = malloc(boardSize * sizeof(Square*));
    if (board == NULL) { //gérer si l'allocation a échoué
        write_crash_report("board memory allocation failed");
        exit(1);
    }

    //pour chaque ligne du plateau
    for (int i = 0; i < boardSize; i++) {
        board[i] = malloc(boardSize * sizeof(Square)); //allouer de l'espace pour chacune des lignes du plateau (pour chacun des tableaux de char*)
        if (board[i] == NULL) { //gérer si l'allocation a échoué
            write_crash_report("board memory allocation failed");
            exit(1);
        }
    }

    initialize_map(board, boardSize, gridSize, monsters, weapons, treasures); //placement des symboles dans le labyrinthe

    return board;
}

//libération de la mémoire allouée pour le plateau
void free_board(Square **board, int size) {

    if(board == NULL){
        write_crash_report("pointer in parameters is NULL");
        exit(1);
    }

    for (int i = 0; i < size; i++) {
        free(board[i]); //libération de la mémoire des lignes
    }
    free(board); //libération du plateau
}


//intialisation d'un joueur
void init_player(Player* player, int num, const Entity symbol, int start_x, int start_y){

    if(player == NULL){
        write_crash_report("pointer in parameters is NULL");
        exit(1);
    }

    player->number = num;
    strcpy(player->symbol.name, symbol.name);
    strcpy(player->symbol.color, symbol.color);

    printf("Entrer un prénom pour le joueur %d : ", player->number);
    scanf("%s", player->name);
    discardInput(); //au cas où le joueur entre un nom avec des espaces

    player->start_x = start_x;
    player->start_y = start_y;
    player->position_x = player->start_x;
    player->position_y = player->start_y;

    player->ancientWeapon_found = 0;
    player->treasure_found = 0;

    player->score = 0;
    player->total_treasures = 0;
    player->killed_monsters = 0;
    player->flip_cards = 0;
}

