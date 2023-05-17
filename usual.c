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

void set_color(Data *data, char *code) {
    if (code[0] == '3') {
        data->cursor.color = code[1] - '0';
    } else if (code[0] == '4') {
        data->cursor.background = code[1] - '0';
    }
}

short get_color_pair(Data *data) {
    short id = (short) (data->cursor.color << 8 | data->cursor.background);
    init_pair(id, data->cursor.color, data->cursor.background);
    return id;
}

void draw_printf2(Data *data, const char *format, ...) {
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

void draw_printf(Data *data, const char *format, ...) {
    va_list args;
    va_start(args, format);

    char buffer[200];
    vsnprintf(buffer, sizeof(buffer), format, args);
    char ansi[2];
    char *str = malloc(strlen(buffer) * sizeof(char));
    int k = 0;
    for (int i = 0; i < strlen(buffer); i++) {
        if (buffer[i] < ' ') {
            char *str2 = malloc(strlen(str));
            for (int l = 0; l < strlen(str2); l++) {
                str2[l] = str[l];
            }
            drawText(data->screen, 20, 10, "Issou chuech", 1);
            //drawText(data->screen, data->cursor.x, data->cursor.y, str, get_color_pair(data));
            getchar();
            int a = strlen(str);
            printf("%d", a);
            free(str2);
            if (buffer[i] == '\n') {
                data->cursor.x = 0;
                data->cursor.y++;
                i++;
            } else {
                for (int j = 0; j < 2; j++) {
                    ansi[j] = buffer[i + j + 2];
                }
                set_color(data, ansi);
                i += 4;
            }
            free(str);
            k = 0;
            str = malloc(strlen(buffer) - i + 1);
        } else {
            str[k] = buffer[i];
            char a = *str + k;
            k++;
        }

    }
    if (buffer[strlen(buffer) - 1] >= ' ') {
        char *str2 = malloc(strlen(str));
        for (int l = 0; l < strlen(str2); l++) {
            str2[l] = str[l];
        }
        drawText(data->screen, data->cursor.x, data->cursor.y, str, get_color_pair(data));
        data->cursor.x = strlen(str);
        free(str2);
        free(str);
    }
    va_end(args);
}