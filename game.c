#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>
#include "external/rgr/inc/libGameRGR2.h"
#include "macro.h"
#include "struct.h"
#include "usual.h"

void launch_game(Data* data) {
    data->scene_type = GAME;
}