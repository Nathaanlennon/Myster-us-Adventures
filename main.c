#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h> // ATTENTION SUPPR ÇA AVAIT DE RENDRE, C'EST POUR LES PROBLEMES DE COMPATIBILITÉ WINDOWS DE CLION


int main() {

    srand(time(NULL));

    // ATTENTION SUPPR ÇA AVAIT DE RENDRE, C'EST POUR LES PROBLEMES DE COMPATIBILITÉ WINDOWS DE CLION
    #ifdef WIN32
        SetConsoleOutputCP(65001);
    #endif

    return 0;
}