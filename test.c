#include <stdio.h>
#include <stdlib.h>

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

typedef struct {
    int number;
    char name[10];
    int weapon;
    //Square progression[BOARD_SIZE][BOARD_SIZE]; //stocke la carte d'un joueur (gardant en mémoire les cases retournées)
} Player;

// efface tout le terminal visible
void clear_all(){
    printf("\033[0;0H\033[J");
}

//efface une partie du terminal
void clear_part(int line, int column){
    printf("\033[%d;%dH\033[J", line, column); // code ANSI pour déplacer le curseur puis efface
}

//déplace le curseur dans la direction et la valeur indiquée
// A: up, B: down, C: forward, D: backward
void cursor_move(char direction, int num){
    printf("\033[%d%c", num, direction);
}
// Va vider le "buffer" pour éviter les fuites de donnée quand on fait des getchar notamment
void flush_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// retourne un simple int d'un seul caractère, utile pour les cas de choix pour par exemple de 1 à 5, moins
// d'utilisations de ressources qu'un scanf
int getint(){
    return getchar() - '0'; // ascii du chiffre transformé en le chiffre lui meme en lui enlevant le code de 0
}

// permet d'écrire un "commentaire" d'une couleur differente et de remettre à la bonne couleur pour la suite
// exemple : appuyez sur entrée pour continuer
void commentary(char tab[]){ // tab est la chaine de caractère désiré en commentaire
    printf("%s%s%s\n", "\033[90m", tab, C_WHT); // le premier %s met la couleur en gris et le 3e la remet en blanc
}

// permet de faire un "entrer pour continuer", "presser une touche pour continuer"
void waiting(){
    commentary("(Press 'enter' to continue..)");
    getchar();
    flush_input_buffer(); // efface la mémoire tampon pour éviter les fuites de mémoire d'input
}


// demande au joueur l'arme qu'il désire pour ce tour
void weapon_choice(Player *player) {
    printf("Another chamber..you have to choose a weapon for this potential fight..\n");
    printf("1 : Torch\n2 : Shield\n3 : Axe\n4 : Bow\n");
    int i = 0; // i est le nombre d'erreurs que le joueur commet. C'est-à-dire quand ils donnent autre chose que demandé
    player->weapon = getint(); //
    flush_input_buffer();
    while (player->weapon < 1 || player->weapon > 4) {
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

// démare la partie
void start_game(){
    Player player;
    weapon_choice(&player);
    clear_part(3,0);
    printf("%d\n", player.weapon);
}

void title_screen() {
    printf("%s%sWelcome to %sThe Myster'us Adventures\n", B_BLK, C_WHT, C_RED);
    printf("\n\n\n%s[1] Let's play !\n[2] HightScore\n", C_WHT);
    int choice;
    while ((choice = getchar()) != EOF) {
        switch (choice) {
            case 49: // touche 1 pressée
                clear_part(3,0);
            start_game();
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
    start_game();


    return 0;
}