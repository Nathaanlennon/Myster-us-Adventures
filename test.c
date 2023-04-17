#include <stdio.h>
#include <stdlib.h>
//#include "RGR_curses/inc/libGameRGR2.h"


int main() {
    printf("oui\n");
    int a;
    scanf("%d", &a);
    printf("\033[0;0H");
    system("clear");

    return 0;
}