#include <stdio.h>
#include <stdlib.h>

#include "include/macro.h"
#include "include/struct.h"
#include "include/usual.h"
#include "include/init.h"
#include "include/gameplay.h"
#include "include/scoremanager.h"

//Demander le nombre de joueurs
int number_players(){
    int number_p = 0 ;
    printf("Etes-vous prêt à parcourir ce labyrinthe rempli d'épreuves ? Très bien ! Mais avant tout !\nCombien êtes-vous ? : ");
    number_p=getint();
    discardInput();
    while (number_p < 2 || number_p > 4) {
        printf("Vous vous êtes probablement trompé... Un sacré début d'aventure ma parole !\nRecommençons, combien êtes-vous? : ");
        number_p = getint();
        discardInput();
    }
    return number_p;
}

void print_board(Square **board, int boardSize, Player player) { //afficher le plateau avec le joueur actif
    if(board == NULL){
        write_crash_report("pointer in parameters is NULL");
        exit(1);
    }
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            if(player.position_x == i && player.position_y == j){ //affichage du joueur selon sa position
                printf("%s%s%s ", player.symbol.color, player.symbol.name, C_WHT);
            }
            else if (board[i][j].flipped == 1){ //affichage des cartes retournées
                if(board[i][j].emptied == 1){ //monstres vaincus (cases vides)
                    printf("%s ", EMPTY);
                }
                else{
                    printf("%s%s%s ", board[i][j].symbol.color, board[i][j].symbol.name, C_WHT); //monstres, trésors et armes découverts
                }
            }

            else {
                printf("%s ", HIDDEN); //cartes retournées
            }
        }
        printf("\n");
    }
    printf("\n");
}

void print_board_total(Square **board, int boardSize) { //print board mais on voit toutes les cases
    if(board == NULL){
        write_crash_report("pointer in parameters is NULL");
        exit(1);
    }

    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            printf("%s%s%s ", board[i][j].symbol.color, board[i][j].symbol.name, C_WHT); //monstres, trésors et armes découverts
        }
        printf("\n");
    }
    printf("\n");
}

//choisir les dimensions du labyrinthe
int choose_board_dimensions(){
    int dim;
    do{
        printf("Choisissez une dimension pour le labyrinthe (>= 5, version classique : 5) : ");
        scanf("%d", &dim);
        discardInput();

        if (dim < 5 && dim > 0){ //labyrinthe trop petit
            printf("Vous avez peur de vous perdre..?\n");
        }
        else if(dim <= 0){ // si le joueur choisit une dimension de 0
            printf("Euh... c'est embêtant... vous ne voulez pas jouer..?\n0 : En effet\n1 : Si\n");
            char ans = getchar();
            discardInput();
            if(ans == '0'){
                printf("Euh... ok...\n");
                waiting();
                return 0; //revenir au menu
            }
        }
        else if (dim > 15){ // la dimension est trop grande
            printf("Ça fait beaucoup là, non ?\n");
        }
    }while(dim < 5 || dim > 15);
    return dim;
}


//Une partie entière
void game(int boardSize, int gridSize, Player players[], int n, const Entity weapons[], const Entity monsters[], const Entity treasures[], long begin_time){

    ////////////        CREATION ET INITIALISATION DU PLATEAU DE CASES ////////////
    Square **board = create_board(boardSize, gridSize, monsters, weapons, treasures);

    /* ////////////        DEBUG BOARD        ////////////
     print_board_total(board, boardSize); //print board mais on voit toutes les cases
     printf("\n\n");*/

    ////////////        GAMEPLAY        ////////////
    //Vérifie les conditions pour remporter une partie
    int win = 0;
    while(win != 1) {
        for(int j=0; j<n; j++){
            waiting();
            clear_all();
            printf("À vous de jouer %s !\n\n", players[j].name);
            print_board(board, boardSize, players[j]); // Passe au joueur suivant en rénitialisant le tableau
            turn(board, boardSize, gridSize, &players[j], monsters, weapons, treasures, begin_time);
            if(players[j].treasure_found == 1 && players[j].ancientWeapon_found == 1){
                win = 1;
                break;
            }
        }
    }
    print_board_total(board, boardSize); //print board mais on voit toutes les cases
    registerScores(players, n); //enregistrer les scores des joueurs

    for(int i = 0; i < n ; i++){
        reset(board, boardSize, gridSize, &players[i]); //reset la position de tous les joueurs et leurs données
    }
    ////////////        LIBERATION DE LA MEMOIRE        ////////////
    free_board(board, boardSize);

    char ans;
    do{
        printf("Rejouer ?\n0: Non\n1 : Oui\n");
        ans=getchar();
        discardInput();
    }while(ans != '1' && ans != '0');
    if(ans == '1'){
        long begin_time = get_time(); //début du chronométrage pour le temps de la nouvelle partie
        game(boardSize, gridSize, players, n, weapons, monsters, treasures, begin_time); //rejouer avec les mêmes joueurs
    }
}

//lancement du jeu
void launch_game(){
    const Entity weapons[5] = {{STICK,C_GRN},{SPELLBOOK, C_BLU}, {SWORD,C_ORE},{DAGGER,C_LGR}}; //Symboles des armes
    const Entity treasures[5] = {{CHEST,C_YEL}, {PORTAL,C_MAG},{TOTEM,C_MAG}, {TOTEM,C_MAG},CHEST,C_YEL}; //Symboles des objets spéciaux (coffres, totems, portails)
    const Entity monsters[4] = {{ZOMBIE, C_RED}, {BASILISK, C_RED}, {TROLL, C_RED}, {HARPY, C_RED}}; //Symboles des monstres
    const Entity adventurers[4] = {{RANGER,C_GRN},{MAGICIAN, C_BLU}, {WARRIOR,C_ORE},{THIEF,C_LGR}}; //Symboles des aventuriers

    int gridSize = choose_board_dimensions(); // taille du labyrinthe donc la grille
    if(gridSize == 0){
        return;
    }
    int boardSize = gridSize+2; // taille du plateau entier, cases de départ et contour inclus

    const int start_x[4] = {0, 2, gridSize+1, gridSize-1}; //ligne des cases de départ de chaque joueur
    const int start_y[4] = {gridSize-1, 0, 2, gridSize+1}; //colonne des cases de départ de chaque joueur


    ///////////// NOMBRE DE JOUEURS ET INITIALISATION DES JOUEURS ///////////
    //Initialisation des joueurs (en fonction du nombre de joueurs)
    int count = 0;
    int n = number_players();
    Player players[n];
    do{
        init_player(&players[count], count+1, adventurers[count], start_x[count], start_y[count]); // Paramètre le joueur
        printf("Bienvenue %s !\n", players[count].name);
        count++;
        waiting();
    }while(count<n);
    printf("C'est parti ! Bonne chance à vous %s\u2665%s\n", C_RED, C_WHT);
    long begin_time= get_time(); //Lance le chronomètre pour pouvoir voir le temps de jeu du joueur

    game(boardSize, gridSize, players, n, weapons, monsters, treasures, begin_time);
}



