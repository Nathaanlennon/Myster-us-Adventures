#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/header.h"
#include "include/macro.h"

//placement aléatoire de symboles
void random_placement(Square **board, int gridSize, const char tab[][10], int tabSize, const char monsters[][10]){

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
        strcpy(board[randomRow][randomCol].symbol, tab[i]);
    }
}

//Initialisation de la map aléatoire de symboles
void initialize_map(Square **board, int boardSize, int gridSize, const char monsters[][10], const char weapons[][10], const char treasures[][10]) {

    if(board == NULL){
        write_crash_report("pointer in parameters is NULL");
        exit(1);
    }
    //remplissage du labyrinthe par des monstres aléatoires et initialisation de l'état de base du plateau (cartes retournées et remplies)
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            board[i][j].emptied = 0;
            board[i][j].flipped = 1;
            if((i == 0 && j == 4) || (i == 2 && j == 0) || (i == 6 && j == 2) || (i == 4 && j == 6)){
                strcpy(board[i][j].symbol, START); //placement des cases de départ
            }
            else if((i%(boardSize-1) == 0) || (j%(boardSize-1) == 0)){
                strcpy(board[i][j].symbol, " "); //vide autour des cases de départ
            }
            else{
                strcpy(board[i][j].symbol, monsters[rand() % 4]); //placement des monstres
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
Square **create_board(int boardSize, int gridSize, const char monsters[][10], const char weapons[][10], const char treasures[][10]) {
    Square **board = malloc(boardSize * sizeof(Square *));
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
void init_player(Player* player, int num, const char* symbol, int start_x, int start_y) {

    if(symbol == NULL || player == NULL){
        write_crash_report("pointer in parameters is NULL");
        exit(1);
    }

    player->number = num;
    strcpy(player->symbol, symbol);

    printf("Entrer un prénom pour le joueur %d : ", player->number);
    scanf("%s", player->name);
    flush_input_buffer(); //au cas où le joueur entre un nom avec des espaces

    player->start_x = start_x;
    player->start_y = start_y;
    player->position_x = player->start_x;
    player->position_y = player->start_y;

    player->ancientWeapon_found = 0;
    player->treasure_found = 0;
}