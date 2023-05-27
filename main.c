#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "include/macro.h"
#include "include/usual.h"
#include "include/menu.h"
#include "include/tuto.h"

int main() {
    srand(time(NULL));
    background(20, 40);
    title_screen();

    return 0;
}
