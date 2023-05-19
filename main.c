#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "include/macro.h"
#include "include/usual.h"

// Structure pour une case du plateau de jeu
typedef struct {
    char symbol[10];
    int flipped; //variable booléenne qui vaut 1 si la case est retournée, 0 sinon
    int emptied; //variable booléenne qui vaut 1 si la case est vidée (monstre battu/objet pris), 0 sinon
} Square;


// Cherche l'indice du symbole de la case dans un tableau spécifié, renvoie l'indice de la première occurrence du symbole dans le tableau, sinon renvoie -1
int SymbolIdInArray(Square square, const char array[][10], int size){
    for(int i = 0; i<size; i++){
        if(strcmp(square.symbol, array[i]) == 0)
            return i; // retourne le premier indice correspondant si trouvé dans le tableau
    }
    return -1; //si pas dans le tableau
}

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

void background(int i, int j){
    for (int k = 0; k<i;k++){
        for (int l=0;l<j;l++){
            printf("%s ",B_BLK);
        }
        printf("\n");
    }
    cursor_move('A', 10);
    waiting();
    //clear_all();
    waiting();
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

void print_board(Square **board, int boardSize, Player* player) { //afficher le plateau avec le joueur actif

    if(board == NULL || player == NULL){
        write_crash_report("pointer in parameters is NULL");
        exit(1);
    }

    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            if(player->position_x == i && player->position_y == j){ //afichage du joueur selon sa position
                printf("%s ", player->symbol);
            }
            else if (board[i][j].flipped == 1){ //afichages des cartes retournées
                if(board[i][j].emptied == 1){ //monstres vaincus (cases vides)
                    printf("%s ", EMPTY);
                }
                else{
                    printf("%s ", board[i][j].symbol); //trésors et armes découverts
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

////////////        A SUPPRIMER, UNIQUEMENT POUR TESTER        ////////////
void print_board_admin(Square **board, int boardSize, Player* player) { //print board mais on voit toutes les cases

    if(board == NULL || player == NULL){
        write_crash_report("pointer in parameters is NULL");
        exit(1);
    }

    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            if(player->position_x == i && player->position_y == j){
                printf("%s ", player->symbol);
            }
            else if(board[i][j].emptied == 1){
                printf("%s ", EMPTY);
            }
            else {
                printf("%s ", board[i][j].symbol);
            }
        }
        printf("\n");
    }
    printf("\n");
}

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
void turn(Square **board, int boardSize, int gridSize, Player* player, const char monsters[][10], const char weapons[][10], const char treasures[][10]) {

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

void launch_game(){
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
        turn(board, BOARD_SIZE, GRID_SIZE, &players[0], monsters, weapons, treasures);
    }

    ////////////        LIBERATION DE LA MEMOIRE        ////////////
    free_board(board, BOARD_SIZE);
}


void title_screen() {
    printf("%s%sWelcome to %sThe Myster'us Adventures\n", B_BLK, C_WHT, C_RED);
    printf("\n\n\n%s[1] Let's play !\n[2] HightScore\n", C_WHT);
    int choice;
    while ((choice = getchar()) != EOF) {
        switch (choice) {
            case 49: // touche 1 pressée
                launch_game();
                //TODO:fonction qui lance le jeu
                break;
            case 50: // touche 2 pressée
                //TODO:fonction qui lance higthscore
                break;
            default:
                //aucun comportement prévu pour cette touche
                break;
        }
    }
}

int main() {
    srand(time(NULL));

    title_screen();

    return 0;
}
