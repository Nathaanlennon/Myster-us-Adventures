#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h> // ATTENTION SUPPR ÇA AVAIT DE RENDRE, C'EST POUR LES PROBLEMES DE COMPATIBILITÉ WINDOWS DE CLION

#define BOARD_SIZE 5

#define HIDDEN "\u25A6"
#define STICK "\u222B"
#define DAGGER "\u2197"
#define SWORD "\u2694"
#define SPELLBOOK "\u2A70"
#define CHEST "\u229F"
#define TROLL "\u2689"
#define BASILISK "\u267E"
#define ZOMBIE "\u2623"
#define HARPY "\u29EC"
#define PORTAL "\u2145"
#define TOTEM "\u25EE"

// Structure pour une case du plateau de jeu
typedef struct {
    char symbol[10];
    int flipped; //variable booléenne qui vaut 1 si la case est retournée, 0 sinon
    int emptied; //variable booléenne qui vaut 1 si la case est vidée (monstre battu/objet pris), 0 sinon
} Square;

typedef struct {
    int number;
    char name[10];
    Square progression[BOARD_SIZE][BOARD_SIZE]; //stocke la carte d'un joueur (gardant en mémoire les cases retournées)
    int score;
} Player;

//Menu
int menu(){
    int bouton_d = 0;
    int nombre_p = 0 ;
    do {
        printf("Bienvenue voyageur(s) ! \nSouhaitez-vous jouer ? Appuyez sur 1. \nSouhaitez-vous accéder aux scores ? Appuyez sur 2. ");
        scanf("%d", &bouton_d);
        printf("\n\n");
    }while(bouton_d <=0 || bouton_d >= 3);


    if(bouton_d == 1){
        printf("Etes-vous prêt à parcourir ce labyrinthe rempli d'épreuves ?\nTrès bien !\nMais avant tout ! Combien êtes-vous ? ");
        scanf("%d", &nombre_p);
        printf("\n\n");
        if(nombre_p<=1 || nombre_p>=5){
            do{
                printf("Vous vous êtes probablement trompé ...\nUn sacré début d'aventure ma parole !\nRecommençons, combien êtes_vous? ");
                scanf("%d", &nombre_p);
                printf("\n\n");
            }while(nombre_p<=1 || nombre_p>=5);

        }
        printf("C'est parti ! Bonne chance à vous <3"); // Il faut retirer le coeur je trouvais ça drôle sur le moment T-T
        printf("\n\n");
    }
    else if(bouton_d == 2){
        do {
            printf("On a pas encore fait ... oops\nSouhaitez-vous jouer? Appuyez sur 1. ");
            scanf("%d", &bouton_d);
            printf("\n\n");
        }while(bouton_d !=1);
        printf("Etes-vous prêt à parcourir ce labyrinthe rempli d'épreuves ?\nTrès bien !\nMais avant tout ! Combien êtes-vous ? ");
        scanf("%d", &nombre_p);
        printf("\n\n");
        if(nombre_p<=1 || nombre_p>=5){
            do{
                printf("Vous vous êtes probablement trompé ...\nUn sacré début d'aventure ma parole !\nRecommençons, combien êtes_vous? ");
                scanf("%d", &nombre_p);
                printf("\n\n");
            }while(nombre_p<=1 || nombre_p>=5);

        }
        printf("C'est parti ! Bonne chance à vous <3"); // Il faut retirer le coeur je trouvais ça drôle sur le moment T-T
        printf("\n\n");
    }
    return nombre_p;
}



Player nb_player(){
    Player f;
    printf("Quel est votre nom ?\n");
    scanf("%s", f.name);
    f.number += 1; /// ATTENTION LE PREMIER F.NUMBER = 2 A NE PAS OUBLIER POUR PLUS TARD ////
    printf("\n\n");

}

//Initialisation de la map aléatoire de symboles
void initialize_map(Square **board, int size) {

    char treasures[9][10] = {STICK, DAGGER, SWORD, SPELLBOOK, CHEST, CHEST, PORTAL, TOTEM,
                             TOTEM}; //Symboles des éléments au nombre précis à placer (trésors, portails, totem)
    char monsters[4][10] = {TROLL, BASILISK, HARPY,
                            ZOMBIE}; //Monstres qui rempliront le reste du labyrinthe de façon aléatoire

    //remplissage du labyrinthe par des monstres aléatoires et initialisation de l'état de base du plateau (cartes retournées et remplies)
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            strcpy(board[i][j].symbol, monsters[rand() % 4]);
            board[i][j].flipped = 0;
            board[i][j].emptied = 0;
        }
    }

    int randomRow, randomCol;

    //placement aléatoire des trésors (y'a moyen d'améliorer mais jsuis fatiguée)
    for (int i = 0; i < 9; i++) {
        randomRow = rand() % size;
        randomCol = rand() % size;
        for (int k = 0; k < 9; k++) { //vérification si la coordonnée aléatoire n'est pas déjà l'emplacement d'un trésor
            if (strcmp(board[randomRow][randomCol].symbol, treasures[k]) == 0) {
                randomRow = rand() % size;
                randomCol = rand() % size;
            }
        }
        strcpy(board[randomRow][randomCol].symbol, treasures[i]);
    }
}

//Création et initialisation d'un tableau 2D de structures Square <=> création d'un plateau de jeu
Square **create_board(int size) {
    Square **board = malloc(size * sizeof(Square *));
    if (board == NULL) { //gérer si l'allocation a échoué
        exit(1);
    }

    //pour chaque ligne du plateau
    for (int i = 0; i < size; i++) {
        board[i] = malloc(size *
                          sizeof(Square)); //allouer de l'espace pour chacune des 5 lignes du plateau (pour chacun des 5 tableaux de 5 char*)
        if (board[i] == NULL) { //gérer si l'allocation a échoué
            exit(1);
        }
    }

    initialize_map(board, size);

    return board;
}

//libération de la mémoire allouée pour le plateau
void free_board(Square **board, int size) {
    for (int i = 0; i < size; i++) {
        free(board[i]); //libération de la mémoire des lignes
    }
    free(board); //libération du plateau
}

//Afficher le plateau
void print_board(Square **board, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (board[i][j].flipped == 1) {
                printf("%s ", board[i][j].symbol);
            } else {
                printf("%s ", HIDDEN);
            }
        }
        printf("\n");
    }
}

void flip_card(Square **board, int size) {
    int x, y;
    do {
        printf("Retourner quelle carte ? donner coordonnées x, y : ");
        scanf("%d, %d", &x, &y);
    } while (x < 0 || x > 4 || y < 0 || y > 4);

    board[x][y].flipped = 1;
}


void next_player(Square **board, int size){
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%s ", HIDDEN);
        }
        printf("\n");
    }
///Player à la place ini///
}


int main() {

    srand(time(NULL));

    // ATTENTION SUPPR ÇA AVAIT DE RENDRE, C'EST POUR LES PROBLEMES DE COMPATIBILITÉ WINDOWS DE CLION
#ifdef WIN32
    SetConsoleOutputCP(65001);
#endif

    ///////////// MENU + NB DE PLAYER + NOM PLAYER ///////////
    int i = 0;
    int x = menu();
    printf("%d", x);
    printf("\n\n");
    do{
        nb_player();
        i++;
    }while(i<x);


    ////////////        CREATION ET INITIALISATION DU PLATEAU DE CASES ////////////
    Square **board = create_board(BOARD_SIZE);

    ////////////        AFFICHAGE        ////////////
    print_board(board, BOARD_SIZE);

    //test de flip_card
    for (int i = 0; i < 5; i++) {
        flip_card(board, BOARD_SIZE);
        print_board(board, BOARD_SIZE);
    }

    ////////////        LIBERATION DE LA MEMOIRE        ////////////
    free_board(board, BOARD_SIZE);

    return 0;
}
