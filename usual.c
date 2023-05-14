#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>
#include "external/rgr/inc/libGameRGR2.h"
#include "macro.h"
#include "struct.h"
#include "usual.h"

// efface tout le terminal visible
void clear_all() {
    printf("\033[0;0H\033[J");
}

//efface une partie du terminal
void clear_part(int line, int column) {
    printf("\033[%d;%dH\033[J", line, column); // code ANSI pour déplacer le curseur puis efface
}

//déplace le curseur dans la direction et la valeur indiquée
// A: up, B: down, C: forward, D: backward
void cursor_move(char direction, int num) {
    printf("\033[%d%c", num, direction);
}

// Va vider le "buffer" pour éviter les fuites de donnée quand on fait des getchar notamment
void flush_input_buffer() {
    int c;
    char *str = malloc(sizeof(char) * 10); //copie le contenu du buffer dans un tableau

    //vérifie que le buffer n'est pas vide
    if (strlen(str) != 0) {
        while ((c = getchar()) != '\n' && c != EOF);
    }
    free(str);
}

// retourne un simple int d'un seul caractère, utile pour les cas de choix pour par exemple de 1 à 5, moins
// d'utilisations de ressources qu'un scanf
int getint() {
    return getchar() - '0'; // ascii du chiffre transformé en le chiffre lui meme en lui enlevant le code de 0
}

// permet d'écrire un "commentaire" d'une couleur differente et de remettre à la bonne couleur pour la suite
// exemple : appuyez sur entrée pour continuer
void commentary(char tab[]) { // tab est la chaine de caractère désiré en commentaire
    printf("%s%s%s\n", "\033[90m", tab, C_WHT); // le premier %s met la couleur en gris et le 3e la remet en blanc
}

// permet de faire un "entrer pour continuer", "presser une touche pour continuer"
void waiting() {
    commentary("(Press 'enter' to continue..)");
    getchar();
    flush_input_buffer(); // efface la mémoire tampon pour éviter les fuites de mémoire d'input
}

int get_color(char *code) {
    if (strcmp(code, C_BLK) == 0 || strcmp(code, B_BLK) == 0) {
        return COLOR_BLACK;
    } else if (strcmp(code, C_RED) == 0 || strcmp(code, B_RED) == 0) {
        return COLOR_RED;
    } else if (strcmp(code, C_GRN) == 0 || strcmp(code, B_GRN) == 0) {
        return COLOR_GREEN;
    } else if (strcmp(code, C_YEL) == 0 || strcmp(code, B_YEL) == 0) {
        return COLOR_YELLOW;
    } else if (strcmp(code, C_BLU) == 0 || strcmp(code, B_BLU) == 0) {
        return COLOR_BLUE;
    } else if (strcmp(code, C_MAG) == 0 || strcmp(code, B_MAG) == 0) {
        return COLOR_MAGENTA;
    } else if (strcmp(code, C_CYN) == 0 || strcmp(code, B_CYN) == 0) {
        return COLOR_CYAN;
    } else if (strcmp(code, C_WHT) == 0 || strcmp(code, B_WHT) == 0) {
        return COLOR_WHITE;
    }
}

void draw_printf(Data *data, const char *format, ...) {
    va_list args;
    va_start(args, format);

    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), format, args);


    // code pour dessiner le texte avec curses
    //drawText(data->screen, 0,0,buffer,1);

    va_end(args);
}