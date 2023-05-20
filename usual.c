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
void clear_all(Data* d) {
    erase();
    d->cursor.y =0;
    d->cursor.x=0;

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

void set_color(Data *data, char *code) {
    if (code[0] == '3') {
        data->cursor.color = code[1] - '0';
    } else if (code[0] == '4') {
        data->cursor.background = code[1] - '0';
    }
}

short get_color_pair(Data *data) {
    if (data->colors.color_num==0){
        data->colors.colors_pair[0][0] = data->cursor.color;
        data->colors.colors_pair[0][1]= data->cursor.background;
        init_pair(8, data->cursor.color, data->cursor.background);

        data->colors.color_num ++;
        return 1;
    }
    for (int i = 0; i < data->colors.color_num; i++) {
        if (data->colors.colors_pair[i][0] == data->cursor.color && data->colors.colors_pair[i][1]== data->cursor.background){
            return i+8;
        }
    }
    data->colors.colors_pair[data->colors.color_num][0] = data->cursor.color;
    data->colors.colors_pair[data->colors.color_num][1]= data->cursor.background;
    init_pair(data->colors.color_num+8, data->cursor.color, data->cursor.background);
    data->colors.color_num++;
    return data->colors.color_num+8;

}

void draw_printf(Data *data, const char *format, ...) {
    va_list args;
    va_start(args, format);

    char buffer[200];
    char ansi[2];
    int j = 0;
    vsnprintf(buffer, sizeof(buffer), format, args);
    char *str = malloc(strlen(buffer));
    for (int i = 0; i < strlen(buffer); i++) {
        if (buffer[i] == '\033') {
            drawText(data->screen, data->cursor.x, data->cursor.y, str, get_color_pair(data));
            ansi[0] = buffer[i + 2];
            ansi[1] = buffer[i + 3];
            set_color(data, ansi);
            data->cursor.x += strlen(str);
            free(str);
            str = malloc(strlen(buffer));
            j = 0;
            i += 4;
        } else if (buffer[i] == '\n') {
            drawText(data->screen, data->cursor.x, data->cursor.y, str, get_color_pair(data));
            free(str);
            str = malloc(strlen(buffer));
            j = 0;
            data->cursor.x = 0;
            data->cursor.y += 1;
        } else {
            str[j] = buffer[i];
            j++;
        }
    }
    drawText(data->screen, data->cursor.x, data->cursor.y, str, get_color_pair(data));
    data->cursor.x += strlen(str);

    free(str);
    va_end(args);
}

void curses_scanf(){

}
void