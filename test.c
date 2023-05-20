#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "macro.h"
#include "usual.h"
#include "struct.h"
#include "game.h"

//change de scène quand appelé
void scene_changer(Data *data, Scene scene){
    data->scene_type = scene;
}

void init(void* pUserData, Screen* pScreen){
    Data* d = pUserData;
    d->screen = pScreen;
    d->scene_type = MAIN_MENU;
    d->player.x=0;
    d->player.y=0;
    d->cursor.x = 0;
    d->cursor.y=0;
    d->cursor.color = 7;
    d->cursor.background = 0;
    d->colors.color_num=0;

    setColorPair(9, COLOR_CYAN, COLOR_MAGENTA);
}

void event(void* pUserData, Screen* pScreen, Event* pEvt){
    Data* d = pUserData;
    switch (d->scene_type) {
        case MAIN_MENU:
            switch (pEvt->code){
                case('&'):
                case('1'):
                    launch_game(d);
                    break;
            }
            break;
        case GAME:
            if(d->_bool){
                d->buffer = malloc(100);
                if(pEvt->code == '\n'){
                    d->_bool=0;
                }
                strcat(d->buffer, (const char *) pEvt->code);

            }
            break;
        case HIGHS_SCORE:
            break;
    }

}

int update(void* pUserData, Screen* pScreen, unsigned long deltaTime){

    return 0;
}


void draw(void* pUserData, Screen* pScreen){
    Data* d = pUserData;
    clear_all(d);

    draw_printf(d,"%s%sWelcome to %sThe Myster'us Adventures\n", B_BLK, C_WHT, C_RED);
    switch (d->scene_type) {
        case MAIN_MENU:
            draw_printf(d,"\n\n\n%s[1] Let's play !\n[2] HighScore\n", C_WHT);
            break;
        case GAME:

            break;
        case HIGHS_SCORE:
            break;
    }

}

void finish(void* pUserData){
}



int main() {
    Data d;
    Callbacks cb;
    cb.cbDraw = &draw;
    cb.cbEvent = &event;
    cb.cbUpdate = &update;
    cb.cbInit = &init;
    cb.cbFinish = &finish;

    GameData *game = createGame(50,50, &d, &cb, 0);
    gameLoop(game);
    return 0;
}
