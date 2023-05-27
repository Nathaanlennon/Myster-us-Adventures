#include <stdio.h>

#include "include/macro.h"
#include "include/game.h"
#include "include/scoremanager.h"

void open_scores(){
    if(printFile("scores.txt") == 0){
        printf("Mais personne n'a jamais joué encore... Retournons au menu.\n");
    }
    else{
        printFile("scores.txt");
    }
}

void title_screen() {
    printf("%s%sWelcome to %sThe Myster'us Adventures\n", B_BLK, C_WHT, C_RED);
    printf("\n\n\n%s[1] Let's play !\n[2] HightScore\n", C_WHT);
    int choice;
    while ((choice = getchar()) != EOF) {
        switch (choice) {
            case '1':
                launch_game();
                break;
            case '2':
                open_scores();
                break;
            default:
                //aucun comportement prévu pour cette touche
                break;
        }
    }
}