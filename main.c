#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "include/macro.h"
#include "include/struct.h"
#include "include/usual.h"
#include "include/init.h"
#include "include/scoremanager.h"
#include "include/menu.h"
#include "include/gameplay.h"
#include "include/game.h"

int main() {
    srand(time(NULL));
    background(20, 40);
    title_screen();

    return 0;
}
