#include <stdio.h>
#include <stdlib.h>

#include "include/struct.h"
#include "include/usual.h"
#include "include/game.h"

void weapon_choice(Player *player) {
    if(player == NULL){
        write_crash_report("pointer in parameters is NULL");
        exit(1);
    }

    printf("Choisissez une arme pour vous préparer à un potentiel combat...\n");
    printf("1 : Torche\n2 : Bouclier réfléchissant\n3 : Hâche\n4 : Arc\n");
    int i = 0; // i est le nombre d'erreurs que le joueur commet. C'est-à-dire quand ils donnent autre chose que demandé

    scanf("%d", &(player->weapon));
    //player->weapon = getint()-1;  PROBLEME
    discardInput();
    player->weapon -= 1;
    while (player->weapon < 0 || player->weapon > 3) {
        i++; // on incrémente le nombre d'erreurs
        if (i >= 3) { //trop d'erreurs alors le jeu prend des mesures
            printf("Ok si vous insistez...pas d'arme...\n");
            player->weapon = -1;
            waiting();
            break;
        }
        cursor_move('A', 1); //déplace le curseur d'une case vers le haut pour remplacer la dernière ligne
        printf("Bien essayé mais vous n'avez pas d'autres armes.\n");
        //player->weapon = getint()-1; PROBLEME
        scanf("%d", &(player->weapon));
        discardInput();
        player->weapon -= 1;
    }
}

//Choisir des coordonnées x et y ne sortant pas du tableau (allant de 0 à 6). Sera utilisée pour la téléportation et pour le totem de transmutation.
void choose_coordinates(Square **board, int* x, int* y, int boardSize, int gridSize) {

    if(board == NULL || x == NULL ||y == NULL){
        write_crash_report("pointer in parameters is NULL");
        exit(1);
    }

    printf("\nChoisir une case en entrant les numéros de la ligne,colonne : ");
    scanf("%d,%d", x, y);
    discardInput();
    //vérification que les coordonnées saisies ne soient pas hors du tableau ou une case déjà retournée.
    if (*x < 1 || *x > gridSize || *y < 1 || *y > gridSize || board[*x][*y].flipped == 1) {
        printf("Coordonnées invalides.");
        choose_coordinates(board, x, y, boardSize, gridSize);
    }
}

//Fonction de choix de direction dans laquelle se déplacer
void choose_direction(Square **board, Player player, int* x, int* y, int boardSize) {
    if(board == NULL || x == NULL ||y == NULL){
        write_crash_report("pointer in parameters is NULL");
        exit(1);
    }

    printf("Choisir une direction :\n");

    //vérifie les directions possibles à proposer
    int validUp = (CheckIndexOutOfArray(player.position_x-1, boardSize)) ? 0 : !CheckSquareInvalid(board[player.position_x-1][player.position_y]);
    int validLeft = (CheckIndexOutOfArray(player.position_y-1, boardSize)) ? 0 : !CheckSquareInvalid(board[player.position_x][player.position_y-1]);
    int validRight = (CheckIndexOutOfArray(player.position_y+1, boardSize)) ? 0 : !CheckSquareInvalid(board[player.position_x][player.position_y+1]);
    int validDown = (CheckIndexOutOfArray(player.position_x+1, boardSize)) ? 0 : !CheckSquareInvalid(board[player.position_x+1][player.position_y]);

    if(validUp){
        printf("Haut\n[8]\n");
    }
    if(validLeft){
        printf("Gauche [4]");
    }
    if(validRight){
        printf(" [6] Droite");
    }
    if(validDown){
        printf("\n[2]\nBas");
    }

    printf("\n");
    int input;
    scanf("%d",&input);

    //vérification que la direction entrée soit valide
    while ((input != 8 || validUp == 0) && (input != 6 || validRight == 0) && (input != 4 || validLeft == 0) && (input != 2 || validDown == 0)) {
        printf("Direction invalide.\n");
        scanf("%d",&input);
        discardInput();
    }

    //envoie au reste du programme les coordonnées vers lesquelles le joueur veut se déplacer
    switch(input){
        case 8:
            *x = player.position_x-1;
            *y = player.position_y;
            break;
        case 6:
            *x = player.position_x;
            *y = player.position_y+1;
            break;
        case 4:
            *x = player.position_x;
            *y = player.position_y-1;
            break;
        case 2:
            *x = player.position_x+1;
            *y = player.position_y;
            break;
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
int event_manager(int* x, int* y, Square **board, int boardSize, int gridSize, Player* player, const Entity monsters[], const Entity weapons[], const Entity treasures[]) {

    if(board == NULL || player == NULL || x == NULL ||y == NULL){
        write_crash_report("pointer in parameters is NULL");
        exit(1);
    }

    board[*x][*y].flipped = 1;
    print_board(board, boardSize, *player);

    int symbolIdMonsters = SymbolIdInArray(board[*x][*y], monsters, 4);
    int symbolIdWeapons = SymbolIdInArray(board[*x][*y], weapons, 4);
    int symbolIdTreasures = SymbolIdInArray(board[*x][*y], treasures, 5);

    if(symbolIdMonsters != -1){ //si la carte retournée est un monstre, engager un combat
        if(fight(*player, symbolIdMonsters)){ //le joueur a choisi la bonne arme contre ce monstre
            printf("Combat gagné !\n");
            board[*x][*y].emptied = 1;
            player->killed_monsters += 1;
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
                player->total_treasures+=1;
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
                while((l == 1 && c == gridSize-1) || (l == 2 && c == 1) || (l == gridSize && c == 2) || (l == gridSize-1 && c == gridSize)){ //interdiction de prendre les cases devant celles de départ
                    printf("Coordonnées invalides.");
                    choose_coordinates(board, &l, &c, boardSize, gridSize);
                }

                Square temp = board[*x][*y];
                board[*x][*y] = board[l][c];
                board[l][c] = temp;

                printf("Poof!\n");
                print_board(board, boardSize, *player);
                return 0;
        }
    }
    return 1;
}

//reset du plateau à la mort du joueur ou si le joueur est bloqué
void reset(Square **board, int boardSize, int gridSize, Player* player){
    for(int i = 1; i <= gridSize; i++){
        for (int j = 1; j <= gridSize; j++){
            board[i][j].flipped = 0; //retourne les cartes
            board[i][j].emptied = 0; //les monstres reviennent à la vie
        }
    }

    player->position_x = player->start_x; //le joueur est remis à sa position de départ
    player->position_y = player->start_y;
    player->treasure_found = 0; //il doit retrouver son trésor et son arme à nouveau
    player->ancientWeapon_found = 0;
}

//Déplacement d'un joueur et évènements = tour entier d'un joueur. Ne s'arrête qu'une fois que le joueur meurt
void turn(Square **board, int boardSize, int gridSize, Player* player, const Entity monsters[], const Entity weapons[], const Entity treasures[]) {

    if(board == NULL || player == NULL){
        write_crash_report("pointer in parameters is NULL");
        exit(1);
    }

    if(player->treasure_found == 1 && player->ancientWeapon_found == 1){
        printf("Bravo %s ! Vous venez de remporter cette partie !\n", player->name);
        player->score +=1;
        return;
    }

    weapon_choice(player); // choix de l'arme
    int x, y;
    choose_direction(board, *player, &x, &y, boardSize); //choix de la direction dans laquelle se déplacer

    if(event_manager(&x, &y, board, boardSize, gridSize, player, monsters, weapons, treasures)){ //déplacer le joueur sur la case choisie si il n'est pas mort et peut rejouer
        player->position_x = x;
        player->position_y = y;
        player->flip_cards+=1;

        //avant de rejouer, vérification si le joueur est bloqué dans le labyrinthe
        if(CheckSquareInvalid(board[player->position_x+1][player->position_y]) && CheckSquareInvalid(board[player->position_x-1][player->position_y])
           && CheckSquareInvalid(board[player->position_x][player->position_y+1]) && CheckSquareInvalid(board[player->position_x][player->position_y-1])){
            print_board(board, boardSize, *player);
            printf("Vous êtes bloqué dans le labyrinthe...\n");
            reset(board, boardSize, gridSize, player);
            printf("Vous retournez au début...\n");
            print_board(board, boardSize, *player);
        }
        else{
            print_board(board, boardSize, *player);
            turn(board, boardSize, gridSize, player, monsters, weapons, treasures);
        }
    }
    else{ //cas où le joueur est mort ou a utilisé un totem de transmutation, ce qui réinitialise sa position et réinitialise la carte
        reset(board, boardSize, gridSize, player);
        printf("Vous retournez au début...\n");
        print_board(board, boardSize, *player);
    }
}