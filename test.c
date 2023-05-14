#include <stdio.h>
#include <stdlib.h>
#include "macro.h"
#include "usual.h"
#include "struct.h"
#include "external/rgr/inc/libGameRGR2.h"



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

    setColorPair(9, COLOR_CYAN, COLOR_MAGENTA);
}

void event(void* pUserData, Screen* pScreen, Event* pEvt){
    Data* d = pUserData;
    switch (pEvt->code){
        case('s'):
            d->player.y ++;
            break;
        case ('z'):
            d->player.y --;
            break;
        case ('d'):
            d->player.x ++;
            break;
        case ('q'):
            d->player.x --;
    }

}

int update(void* pUserData, Screen* pScreen, unsigned long deltaTime){

    return 0;
}


void draw(void* pUserData, Screen* pScreen){
    Data* d = pUserData;
    erase();
    switch (d->scene_type) {
        case MAIN_MENU:

            //draw_printf(d,"%s%sWelcome to %sThe Myster'us Adventures\n", B_BLK, C_WHT, C_RED);
            break;
        case GAME:
            drawText(pScreen, d->player.x, d->player.y, "@", 1);
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

    GameData *game = createGame(20,20, &d, &cb, 0);
    gameLoop(game);
    return 0;
}