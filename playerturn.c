#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/header.h"
#include "include/macro.h"


void weapon_choice(Player *player) {

    if(player == NULL){
        write_crash_report("pointer in parameters is NULL");
        exit(1);
    }

    printf("Choisissez une arme pour vous préparer à un potentiel combat...\n");
    printf("1 : Torche\n2 : Bouclier réfléchissant\n3 : Hâche\n4 : Arc\n");
    int i = 0; // i est le nombre d'erreurs que le joueur commet. C'est-à-dire quand ils donnent autre chose que demandé
    player->weapon = getint()-1;
    flush_input_buffer();
    while (player->weapon < 0 || player->weapon > 3) {
        i++; // on incrémente le nombre d'erreurs
        if (i >= 3) { //trop d'erreurs alors le jeu prend des mesures
            printf("Ok si vous insistez...pas d'armes...\n");
            player->weapon = 0;
            waiting();
            break;
        }
        cursor_move('A', 1); //déplace le curseur d'une case vers le haut pour remplacer la dernière ligne
        printf("Bien essayé mais vous n'avez pas d'autres armes.\n");
        player->weapon = getint();
        flush_input_buffer();
    }
}

//Choisir des coordonnées x et y ne sortant pas du tableau (allant de 0 à 6). Sera utilisée pour la fonction de mouvement, pour la téléportation, pour le totem de transmutation.
void choose_coordinates(Square **board, int* x, int* y, int boardSize, int gridSize) {

    if(board == NULL || x == NULL ||y == NULL){
        write_crash_report("pointer in parameters is NULL");
        exit(1);
    }

    printf("\nChoisir une case en entrant les numéros de la ligne,colonne : ");
    scanf("%d,%d", x, y);
    flush_input_buffer();
    if (*x < boardSize - 1 - gridSize || *x > gridSize || *y < boardSize - 1 - gridSize || *y > gridSize || board[*x][*y].flipped == 1) { //vérification que les coordonnées saisies ne soint pas hors du tableau ou une case déjà vidée
        printf("Coordonnées invalides.");
        choose_coordinates(board, x, y, boardSize, gridSize);
    }
}

// Vérifie que l'arme choisie est correcte par rapport à l'ennemi affronté. Renvoie 1 si oui, 0 sinon
int fight(Player player, int monster) {
    if (player.weapon == monster) {
        return 1;
    }
    return 0;
}

//Sous fonction pour gérer les découvertes d'armes antiques. Renvoie 1 si l'arme antique correspond à celle du joueur, 0 sinon.
int event_ancient_weapon(Player* player, int ancientWeapon){

    if(player == NULL){
        write_crash_report("pointer in parameters is NULL");
        exit(1);
    }

    printf("Vous avez découvert une arme antique !\n");
    if (player->number == ancientWeapon+1) {
        printf("C'est la vôtre !\n");
        player->ancientWeapon_found = 1;
        return 1;
    }
    else{
        printf("Il semblerait que ce ne soit pas la vôtre... Vous continuez votre chemin.\n");
        return 0;
    }
}

//Fonction qui gère les rencontres de monstres/d'objets lors du déplacement. Renvoie 1 si le joueur peut continuer à se déplacer librement, 0 si le tour doit se finir
int event_manager(int* x, int* y, Square **board, int boardSize, int gridSize, Player* player, const char monsters[][10], const char weapons[][10], const char treasures[][10]) {

    if(board == NULL || player == NULL || x == NULL ||y == NULL){
        write_crash_report("pointer in parameters is NULL");
        exit(1);
    }

    board[*x][*y].flipped = 1;
    print_board(board, boardSize, player);

    int symbolIdMonsters = SymbolIdInArray(board[*x][*y], monsters, 4);
    int symbolIdWeapons = SymbolIdInArray(board[*x][*y], weapons, 4);
    int symbolIdTreasures = SymbolIdInArray(board[*x][*y], treasures, 5);

    if(symbolIdMonsters != -1){ //si la carte retournée est un monstre, engager un combat
        if(fight(*player, symbolIdMonsters)){ //le joueur a choisi la bonne arme contre ce monstre
            printf("Combat gagné !\n");
            board[*x][*y].emptied = 1;
        }
        else{
            printf("Combat perdu !\n"); //le joueur n'a pas choisi la bonne arme contre ce monstre
            return 0;
        }
    }

    else if(symbolIdWeapons != -1){ //si la carte retournée est une arme antique, vérifier si elle appartient au joueur
        event_ancient_weapon(player, symbolIdWeapons);
    }
    else if(symbolIdTreasures != -1){
        switch(symbolIdTreasures){ //si la carte retournée est un trésor/totem/portail
            case 0:
                printf("Vous avez découvert un trésor !\n");
                player->treasure_found = 1;
                break;

            case 1:
                printf("Vous avez découvert un portail magique de téléportation. Vous pouvez choisir n'importe quelle case encore cachée où vous téléporter. Mais avant cela...\n");
                weapon_choice(player);
                choose_coordinates(board, x, y, boardSize, gridSize);
                return event_manager(x, y, board, boardSize, gridSize, player, monsters, weapons, treasures);

            case 2:
                printf("Vous avez découvert un totem de transmutation. Vous pouvez choisir n'importe quelle case encore cachée pour l'échanger avec le totem. Cela mettra fin à votre tour.\n");

                int l,c;
                choose_coordinates(board, &l, &c, boardSize, gridSize);
                while((l == 1 && c == 3) || (l == 2 && c == 1) || (l == 5 && c == 2) || (l == 4 && c == 5)){ //interdiction de prendre les cases devant celles de départ
                    printf("Coordonnées invalides.");
                    choose_coordinates(board, &l, &c, boardSize, gridSize);
                }

                Square temp = board[*x][*y];
                board[*x][*y] = board[l][c];
                board[l][c] = temp;

                printf("Poof!\n");
                print_board(board, boardSize, player);
                return 0;
        }

    }
    return 1;
}

//Déplacement d'un joueur et évènements
void move(Square **board, int boardSize, int gridSize, Player* player, const char monsters[][10], const char weapons[][10], const char treasures[][10]) {

    if(board == NULL || player == NULL){
        write_crash_report("pointer in parameters is NULL");
        exit(1);
    }

    int x, y;

    choose_coordinates(board, &x, &y, boardSize, gridSize); //saisie des coordonnées vers où se déplacer
    while(abs(x - player->position_x) > 1 //conditions pour n'avancer que d'une case adjacente à la position actuelle
          || abs(y - player->position_y) > 1
          || (x != player->position_x && y != player->position_y) //ne pas aller en diagonale
          || (x == player->position_x && y == player->position_y))//ne pas faire du sur place
    {
        printf("Coordonnées invalides.");
        choose_coordinates(board, &x, &y, boardSize, gridSize);
    }

    if(event_manager(&x, &y, board, boardSize, gridSize, player, monsters, weapons, treasures)){ //déplacer le joueur sur la case choisie si il n'est pas mort
        player->position_x = x;
        player->position_y = y;
    }
    else{
        for(int i = 1; i <= gridSize; i++){ //reset du plateau à la mort du joueur
            for (int j = 1; j <= gridSize; j++){
                board[i][j].flipped = 0;
                board[i][j].emptied = 0;
            }
        }
        player->position_x = player->start_x; //le joueur est remis à sa position de départ
        player->position_y = player->start_y;
        printf("Vous revenez à votre position de départ...\n");
    }
    print_board(board, boardSize, player);
}