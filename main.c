#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
//#include <windows.h> // TODO:ATTENTION SUPPR ÇA AVAIT DE RENDRE, C'EST POUR LES PROBLEMES DE COMPATIBILITÉ WINDOWS DE CLION

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

#define C_BLK "\033[30m" // Caractère noir
#define C_RED "\033[31m" // Caractère rouge
#define C_GRN "\033[32m" // Caractère vert
#define C_YEL "\033[33m" // Caractère jaune
#define C_BLU "\033[34m" // Caractère bleu
#define C_MAG "\033[35m" // Caractère magenta
#define C_CYN "\033[36m" // Caractère cyan
#define C_WHT "\033[37m" // Caractère blanc

#define B_BLK "\033[40m" // Fond noir
#define B_RED "\033[41m" // Fond rouge
#define B_GRN "\033[42m" // Fond vert
#define B_YEL "\033[43m" // Fond jaune
#define B_BLU "\033[44m" // Fond bleu
#define B_MAG "\033[45m" // Fond magenta
#define B_CYN "\033[46m" // Fond cyan
#define B_WHT "\033[47m" // Fond blanc

//déplace le curseur dans la direction et la valeur indiquée
void cursor_move(char direction, int num){
    printf("\033[%d%c", num, direction);
}





// Structure pour une case du plateau de jeu
typedef struct {
    char symbol[10];
    int flipped; //variable booléenne qui vaut 1 si la case est retournée, 0 sinon
    int emptied; //variable booléenne qui vaut 1 si la case est vidée (monstre battu/objet pris), 0 sinon
} Square;

typedef struct {
    int number;
    char name[10];
    int weapon;
    Square progression[BOARD_SIZE][BOARD_SIZE]; //stocke la carte d'un joueur (gardant en mémoire les cases retournées)
} Player;

void weapon_choice(Player *player){
    printf("Another chamber..you have to choose a weapon for this potential fight..\n");
    printf("1 : Torch\n2 : Shield\n3 : Axe\n4 : Bow\n");
    scanf("%d", &player->weapon);
}

//efface tout ce qui est visible
void clear_all(){
    printf("\033[0;0H\033[J");
}

//efface une partie du terminal
void clear_part(int line, int column){
    printf("\033[%d;%dH\033[J", line, column);
    //system("cls"); //TODO: remplacer cls par clear lors du passage à linux
}

//Initialisation de la map aléatoire de symboles
void initialize_map(Square **board, int size) {

    char treasures[9][10] = {STICK, DAGGER, SWORD, SPELLBOOK, CHEST, CHEST, PORTAL, TOTEM,
                             TOTEM}; //Symboles des éléments au nombre précis à placer (trésors, portails, totem)
    char monsters[4][10] = {ZOMBIE, BASILISK, TROLL,
                            HARPY}; //Monstres qui rempliront le reste du labyrinthe de façon aléatoire

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
    clear_part(3, 0); //TODO: faut decider de l'interface pour laisser le titre peut-etre, enfin on verra demain
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

// Vérifie que l'arme choisit est correcte par rapport à l'ennemi affronté. Renvoie 1 si oui, 0 sinon
int fight(Player player, int monster){
    if (player.weapon == monster){
        return 1;
    }
    return 0;
}

// effectue le choix de l'arme quand appelé

void launch_game(){
    Player player;
    clear_part(3, 0);
    weapon_choice(&player);
    printf("%d\n", player.weapon);
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

    // TODO:ATTENTION SUPPR ÇA AVAIT DE RENDRE, C'EST POUR LES PROBLEMES DE COMPATIBILITÉ WINDOWS DE CLION
/*#ifdef WIN32
    SetConsoleOutputCP(65001);
#endif*/
    title_screen();

    return 0;
}
