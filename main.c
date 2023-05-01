#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "include/macro.h"
#include "usual.c"

// Structure pour une case du plateau de jeu
typedef struct {
    char symbol[10];
    int flipped; //variable booléenne qui vaut 1 si la case est retournée, 0 sinon
    int emptied; //variable booléenne qui vaut 1 si la case est vidée (monstre battu/objet pris), 0 sinon
} Square;

typedef struct {
    int number;
    char symbol[10];
    int weapon;
    int start_x; //case de départ attribuée abscisse
    int start_y; //case de départ attribuée ordonnée
    int position_x; //position durant le tour abscicsse
    int position_y; //position durant le tour abscicsse
    char name[];
    //color mais j'ai la flemme pour l'instant
} Player;


//placement aléatoire de symboles
void random_placement(Square **board, int gridSize, char tab[][10], int tabSize, char monsters[4][10]){
    int randomRow, randomCol;
    for (int i = 0; i < tabSize; i++) {
        randomRow = rand() % gridSize+1;
        randomCol = rand() % gridSize+1;
        for (int k = 0; k < 4; k++) { //vérification si la coordonnée aléatoire n'est pas déjà l'emplacement d'un objet
            if (strcmp(board[randomRow][randomCol].symbol, monsters[k]) != 0) {
                randomRow = rand() % gridSize+1;
                randomCol = rand() % gridSize+1;
            }
        }
        strcpy(board[randomRow][randomCol].symbol, tab[i]);
    }
}

//Initialisation de la map aléatoire de symboles
void initialize_map(Square **board, int boardSize, int gridSize, char monsters[4][10], char treasures[5][10], char weapons[4][10]) {

    //remplissage du labyrinthe par des monstres aléatoires et initialisation de l'état de base du plateau (cartes retournées et remplies)
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            board[i][j].emptied = 0;
            board[i][j].flipped = 1;
            if((i == 0 && j == boardSize-3) || (i == 2 && j == 0) || (i == boardSize-1 && j == 2) || (i == boardSize-3 && j == boardSize-1)){
                strcpy(board[i][j].symbol, START);
            }
            else if((i%(boardSize-1) == 0) || (j%(boardSize-1) == 0)){
                strcpy(board[i][j].symbol, " ");
            }
            else{
                strcpy(board[i][j].symbol, monsters[rand() % 4]);
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
Square **create_board(int boardSize, int gridSize, char monsters[9][10], char treasures[5][10], char weapons[4][10]) {
    Square **board = malloc(boardSize * sizeof(Square *));
    if (board == NULL) { //gérer si l'allocation a échoué
        exit(1);
    }

    //pour chaque ligne du plateau
    for (int i = 0; i < boardSize; i++) {
        board[i] = malloc(boardSize * sizeof(Square)); //allouer de l'espace pour chacune des lignes du plateau (pour chacun des tableaux de char*)
        if (board[i] == NULL) { //gérer si l'allocation a échoué
            exit(1);
        }
    }

    initialize_map(board, boardSize, gridSize, monsters, treasures, weapons);

    return board;
}

//libération de la mémoire allouée pour le plateau
void free_board(Square **board, int size) {
    for (int i = 0; i < size; i++) {
        free(board[i]); //libération de la mémoire des lignes
    }
    free(board); //libération du plateau
}

//intialisation d'un joueur
void init_player(Player* player, int num, char* name, char* symbol, int start_x, int start_y) {
    player->number = num;
    strcpy(player->name, name);
    strcpy(player->symbol, symbol);
    player->start_x = start_x;
    player->start_y = start_y;
    player->position_x = player->start_x;
    player->position_y = player->start_y;
}

// Cherche l'indice du symbole de la case dans un tableau spécifié, renvoie l'indice de la première occurrence du symbole dans le tableau, sinon renvoie -1
int SymbolIdInArray(Square square, char array[][10], int size){
    for(int i = 0; i<size; i++){
        if(strcmp(square.symbol, array[i]) == 0)
            return i; // retourne l'indice si trouvé dans le tableau
    }
    return -1; //si pas dans le tableau
}

void print_board(Square **board, int boardSize, Player* activePlayer) { //version 2 avec position joueur actif ajouté
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            if(activePlayer->position_x == i && activePlayer->position_y == j){
                printf("%s ", activePlayer->symbol);
            }
            else if (board[i][j].flipped == 1){
                if(board[i][j].emptied == 1){
                    printf("%s ", EMPTY);
                }
                else{
                    printf("%s ", board[i][j].symbol);
                }
            }

            else {
                printf("%s ", HIDDEN);
            }
        }
        printf("\n");
    }
    printf("\n");
}
void weapon_choice(Player *player) {
    printf("Another chamber..you have to choose a weapon for this potential fight..\n");
    printf("1 : Torch\n2 : Shield\n3 : Axe\n4 : Bow\n");
    int i = 0; // i est le nombre d'erreurs que le joueur commet. C'est-à-dire quand ils donnent autre chose que demandé
    player->weapon = getint()-1;
    flush_input_buffer();
    while (player->weapon < 0 || player->weapon > 3) {
        i++; // on incrémente le nombre d'erreurs
        if (i >= 3) { //trop d'erreurs alors le jeu prend des mesures
            printf("Ok fine... No weapon if you really want it...\n");
            player->weapon = 0;
            waiting();
            break;
        }
        cursor_move('A', 1); //déplace le curseur d'une case vers le haut pour remplacer la dernière ligne
        printf("Nice try but you haven't another weapons\n");
        player->weapon = getint();
        flush_input_buffer();
    }
}

// Vérifie que l'arme choisit est correcte par rapport à l'ennemi affronté. Renvoie 1 si oui, 0 sinon
int fight(Player player, int monster) {
    if (player.weapon == monster) {
        return 1;
    }
    return 0;
}

//Choisir des coordonnées x et y ne sortant pas du tableau (allant de 0 à 6). Sera utilisée pour la fonction de mouvement, pour la téléportation, pour le totem de transmutation.
void choose_coordinates(Square **board, int* x, int* y, int boardSize, int gridSize) {
    printf("\nChoisir une case en entrant x,y : ");
    scanf("%d,%d", x, y);
    flush_input_buffer();
    if (*x < boardSize - 1 - gridSize || *x > gridSize || *y < boardSize - 1 - gridSize || *y > gridSize) {
        printf("Coordonnées invalides.");
        choose_coordinates(board, x, y, boardSize, gridSize);
    }
}

//Déplacement d'un joueur (engagement d'un combat et déplacement nécessaire selon le résultat du combat)
void move(Square **board, int boardSize, int gridSize, Player* activePlayer, char monsters[4][10], char weapons[4][10], char treasures[5][10]) {
    int x, y;
    choose_coordinates(board, &x, &y, boardSize, gridSize);
    while(x < activePlayer->position_x - 1 //conditions pour n'avancer que d'une case adjacente à la position actuelle
            || x > activePlayer->position_x + 1
            || y < activePlayer->position_y - 1
            || y > activePlayer->position_y + 1
            || (x != activePlayer->position_x && y != activePlayer->position_y) //ne pas aller en diagonale
            || (x == activePlayer->position_x && y == activePlayer->position_y))//ne pas faire du sur place
    {
        printf("Coordonnées invalides.");
        choose_coordinates(board, &x, &y, boardSize, gridSize);
    }


    board[x][y].flipped = 1;
    print_board(board, boardSize, activePlayer);

    if(SymbolIdInArray(board[x][y], monsters, 4) != -1){
        if(fight(*activePlayer, SymbolIdInArray(board[x][y], monsters, 4)) == 1){
            printf("Combat gagné !\n");
            board[x][y].emptied = 1;
            activePlayer->position_x = x;
            activePlayer->position_y = y;
        }
        else{
            printf("Combat perdu !\n");
            for(int i = 1; i <= gridSize; i++){
                for (int j = 1; j <= gridSize; j++){
                    board[i][j].flipped = 0;
                    board[i][j].emptied = 0;
                }
            }
            activePlayer->position_x = activePlayer->start_x;
            activePlayer->position_y = activePlayer->start_y;
        }
    }
    else{
        activePlayer->position_x = x;
        activePlayer->position_y = y;
        //gérer les trucs spéciaux type armes, coffres etc
    }

    print_board(board, boardSize, activePlayer);
}



int main() {
    srand(time(NULL));

    char weapons[4][10] = {STICK, DAGGER, SWORD, SPELLBOOK}; //Symboles des armes

    char treasures[5][10] = {CHEST, PORTAL, TOTEM, TOTEM, CHEST}; //Symboles des objets spéciaux (coffres, totems, portails)

    char monsters[4][10] = {ZOMBIE, BASILISK, TROLL,
                            HARPY}; //Symboles des monstres

    ////////////        CREATION ET INITIALISATION DU PLATEAU DE CASES ////////////
    Square **board = create_board(BOARD_SIZE, GRID_SIZE, monsters, treasures, weapons);

    ////////////        CREATION PERSONNAGE        ////////////
    Player ranger;
    init_player(&ranger, 1, "Bob", RANGER, 0, BOARD_SIZE-3);

    print_board(board, BOARD_SIZE, &ranger);

    ////////////        WIP TEST GAMEPLAY        ////////////
    for (int i = 0; i < 10; i++) {
        weapon_choice(&ranger);
        move(board, BOARD_SIZE, GRID_SIZE, &ranger, monsters,treasures, weapons);
    }

    ////////////        LIBERATION DE LA MEMOIRE        ////////////
    free_board(board, BOARD_SIZE);

    return 0;
}
