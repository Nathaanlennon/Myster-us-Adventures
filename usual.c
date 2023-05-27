#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <poll.h>
#include <unistd.h>
#include <stdarg.h>
#include <time.h>
#include <fcntl.h>
#include <errno.h>
#include "include/macro.h"
#include "include/struct.h"
#include "include/usual.h"

// efface tout le terminal visible
void clear_all(){
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

/* 0: success -1: error */
int setBlockingFD(int fileDescriptor, int blocking){
    int r=fcntl(fileDescriptor,F_GETFL);
    if(r==-1){
        perror("fcntl(F_GETFL)");
        return -1;
    }
    int flags=(blocking ? r & ~O_NONBLOCK : r | O_NONBLOCK);
    r=fcntl(fileDescriptor,F_SETFL,flags);
    if(r==-1){
        perror("fcntl(F_SETFL)");
        return -1;
    }
    return 0;
}

void discardInput(void){
    setBlockingFD(STDIN_FILENO, 0);
    for(;;){
        int c=fgetc(stdin);
        if(c==EOF){
            if(errno==EAGAIN){
                //vide
            }
            break;
        }
        else{
            //pas vide
        }
    }
    setBlockingFD(STDIN_FILENO, 1);
}

// retourne un simple int d'un seul caractère, utile pour les cas de choix pour par exemple de 1 à 5, moins
// d'utilisations de ressources qu'un scanf
int getint() {
    return (getchar() - '0'); // ascii du chiffre transformé en le chiffre lui meme en lui enlevant le code de 0
}

// permet d'écrire un "commentaire" d'une couleur differente et de remettre à la bonne couleur pour la suite
// exemple : appuyez sur entrée pour continuer
void commentary(char tab[]) { // tab est la chaine de caractère désiré en commentaire
    printf("%s%s%s\n", "\033[90m", tab, C_WHT); // le premier %s met la couleur en gris et le 3e la remet en blanc
}

// permet de faire un "entrer pour continuer", "presser une touche pour continuer"
void waiting() {
    commentary("(Appuyez sur Entrée pour continuer..)");
    getchar();
    discardInput();
}

//permet de créer un fichier de rapport de crash si il y en a un
void write_crash_report(const char* error_message) {
    time_t current_time = time(NULL); //la fonction time renvoie nombre de secondes écoulées depuis le 01/01/1970 00:00:00
    struct tm* time = localtime(&current_time); //conversion du timestamp précédent en date lisible selon le fuseau horaire local
    char filename[50];
    strftime(filename, sizeof(filename), "%Y-%m-%d_%H-%M-%S_crash_report.txt", time); //formatage du nom de fichier en année-mois-jour_heure-min-sec

    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Erreur lors de la création du rapport de crash.\n");
    }

    fprintf(file, "Date et heure du crash : %s", asctime(time));
    fprintf(file, "Message d'erreur : %s\n", error_message);

    fclose(file);
    printf("Le rapport de crash a été créé : %s\n", filename);
}

// Cherche l'indice du symbole de la case dans un tableau spécifié, renvoie l'indice de la première occurrence du symbole dans le tableau, sinon renvoie -1
int SymbolIdInArray(Square square, const Entity array[], int size){
    for(int i = 0; i<size; i++){
        if(strcmp(square.symbol.name, array[i].name) == 0)
            return i; // retourne le premier indice correspondant si trouvé dans le tableau
    }
    return -1; //si pas dans le tableau
}

//crée un background noir pour éviter les problemes de terminal pas noir car on utiliser des couleurs spécifiques
void background(int i, int j) {
    for (int k = 0; k < i; k++) {
        for (int l = 0; l < j; l++) {
            printf("%s ", B_BLK);
        }
        printf("\n");
    }
    cursor_move('A', 10);
    clear_all();
}

//Vérifie si la case entrée est déjà retournée, ou est le bord de la map, ou est une case de départ. Renvoie 1 si elle est l'une de ces conditions, 0 sinon.
int CheckSquareInvalid(Square square) {
    return (square.flipped == 1 || strcmp(square.symbol.name, " ") == 0 || strcmp(square.symbol.name, START) == 0);
}

//Vérifie si l'index spécifié est valide pour être recherché dans un tableau. Renvoie 1 si hors du tableau, 0 sinon.
int CheckIndexOutOfArray(int i, int arraySize){
    return (i < 0 || i >= arraySize);
}

//récupère le temps depuis le 1er janvier 1970 à 00:00:00 UTC
long get_time(){
    return time(NULL);
}

//compare le temps entre t1 et t2 pour retourner combien de temps est passé entre les deux
int compare_time(long t1, long t2){
    return (int)(t2-t1);
}

//transforme le nombre de secondes en heure minute seconde et le print
void format_time(int seconds) {
    int hours = seconds / 3600;
    int minutes = (seconds % 3600) / 60;
    int remaining_seconds = (seconds % 3600) % 60;

    printf("%d heures, %d minutes et %d secondes\n", hours, minutes, remaining_seconds);
}