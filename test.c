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

//déplace le curseur dans la direction et la valeur indiquée
void cursor_move(char direction, int num){
    printf("\033[%d%c", num, direction);
}


void title_screen() {
    printf("%s%sWelcome to %sThe Myster'us Adventures\n", B_BLK, C_WHT, C_RED);
    printf("\n\n\n%s[1] Let's play !\n[2] HightScore\n", C_WHT);
    int choice;
    while ((choice = getchar()) != EOF) {
        switch (choice) {
            case 49: // touche 1 pressée
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
    title_screen();

    return 0;
}