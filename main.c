#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "include/macro.h"
#include "usual.c"

// Structure pour une case du plateau de jeu
typedef struct {
    char symbol[10];
    int flipped; //variable booléenne qui vaut 1 si la case est retournée, 0 sinon
    int emptied; //variable booléenne qui vaut 1 si la case est vidée (monstre battu/objet pris), 0 sinon
} Square;

typedef struct {
    int number; //numéro du joueur
    char symbol[10]; //symbole du joueur
    int weapon; //arme antique attribuée

    int start_x; //case de départ attribuée abscisse
    int start_y; //case de départ attribuée ordonnée
    int position_x; //position durant le tour abscicsse
    int position_y; //position durant le tour abscicsse

    int score; //booléen qui vaudra 1 si la victoire est remportée, 0 sinon
    int flip_cards; //nombre de cartes retournées au cours de la partie
    int killed_monsters; //nombre de monstres tués au cours de la partie
    int total_treasures; //nombre de trésors trouvés au cours de la partie

    int ancientWeapon_found; //booléen si l'arme antique a été trouvée (condition nécessaire à la victoire)
    int treasure_found; //booléen si au moins 1 trésor a été trouvé (condition nécessaire à la victoire)

    char name[30];
    //color mais j'ai la flemme pour l'instant
} Player;

//Menu qui va être remplacé par Nathan, c'est triste
int menu(){
    int bouton_d = 0;
    int nombre_p = 0 ;
    do {
        printf("Bienvenue voyageur(s) ! \nSouhaitez-vous jouer ? Appuyez sur 1. \nSouhaitez-vous accéder aux scores ? Appuyez sur 2. ");
        scanf("%d", &bouton_d);
        printf("\n\n");
    }while(bouton_d <=0 || bouton_d >= 3);


    if(bouton_d == 1){
        printf("Etes-vous prêt à parcourir ce labyrinthe rempli d'épreuves ?\nTrès bien !\nMais avant tout ! Combien êtes-vous ? ");
        scanf("%d", &nombre_p);
        printf("\n\n");
        if(nombre_p<=1 || nombre_p>=5){
            do{
                printf("Vous vous êtes probablement trompé ...\nUn sacré début d'aventure ma parole !\nRecommençons, combien êtes_vous? ");
                scanf("%d", &nombre_p);
                printf("\n\n");
            }while(nombre_p<=1 || nombre_p>=5);

        }
        printf("C'est parti ! Bonne chance à vous <3"); // Il faut retirer le coeur je trouvais ça drôle sur le moment T-T
        printf("\n\n");
    }
    else if(bouton_d == 2){
        printFile("scores.txt");
    }
    return nombre_p;
}

void score(Player player){
    printf("Voici votre score :\n%d cartes retournées\n%d monstres tués\n%d de trésors trouvés\n", player.flip_cards, player.killed_monsters, player.total_treasures);
}

//Enregistre un joueur et ses scores dans le fichier des scores. Si le joueur est déjà enregistré dans le fichier, met à jour ses scores
void updatePlayerStats(Player player){
    FILE* file = fopen("scores.txt", "r+");
    if (file == NULL){ //si le fichier n'existe pas, le créer
        file = fopen("scores.txt", "w");
        if (file == NULL){
            write_crash_report("failed to create file");
            exit(1);
        }
    }

    char line[100]; //on estime que les lignes ne feront pas plus de 100 caractères
    int found = 0;

    int numGames = 0;
    int victories = 0;
    int killedMonsters = 0;
    int flippedCards = 0;
    int treasuresFound = 0;

    long int cursorPositionStart;

    //recherche du joueur dans le fichier
    while (fgets(line, sizeof(line), file) != NULL){
        if (strstr(line, player.name) != NULL){ //rechercher une sous chaine nom du joueur dans la chaine complète de la ligne
            found = 1;

            cursorPositionStart = ftell(file);

            fgets(line, sizeof(line), file); //lire la prochaine ligne
            sscanf(line, "Nombre de parties jouées : %d",&numGames); // récupérer la valeur à cette ligne

            fgets(line, sizeof(line), file);
            sscanf(line, "Victoires : %d",&victories);

            fgets(line, sizeof(line), file);
            sscanf(line, "Monstres terrassés : %d",&killedMonsters);

            fgets(line, sizeof(line), file);
            sscanf(line, "Trésors découverts : %d",&treasuresFound);

            fgets(line, sizeof(line), file);
            sscanf(line, "Cases révélées : %d",&flippedCards);
        }
    }

    //si le joueur n'a pas été trouvé, l'ajouter
    if (!found){
        fprintf(file, "%s\n", player.name);
        fprintf(file, "Nombre de parties jouées : %4d\n", 1); //%4d pour laisser de la place quand le nombre de digits de la valeur augmente sinon ça cause un problème dans l'écriture des données pour le prochain personnage
        fprintf(file, "Victoires : %4d\n", player.score);
        fprintf(file, "Monstres terrassés : %4d\n", player.killed_monsters);
        fprintf(file, "Trésors découverts : %4d\n", player.total_treasures);
        fprintf(file, "Cases révélées : %4d\n", player.flip_cards);
    }
    else{ //si le joueur a été trouvé, mettre à jour ses scores
        fseek(file, cursorPositionStart, SEEK_SET);
        fprintf(file, "Nombre de parties jouées : %4d\n", numGames+1);
        fprintf(file, "Victoires : %4d\n", victories+player.score);
        fprintf(file, "Monstres terrassés : %4d\n", killedMonsters+player.killed_monsters);
        fprintf(file, "Trésors découverts : %4d\n", treasuresFound+player.total_treasures);
        fprintf(file, "Cases révélées : %4d\n", flippedCards+player.flip_cards);
    }
    fseek(file, 0, SEEK_SET);
    fclose(file);
}

//Enregistre tous les scores des joueurs de la partie. Fonction à mettre à la fin d'une partie pour éviter d'enregistrer des données d'une partie non terminée
void registerScores(Player players[], int size){
    for(int i=0;i<size;i++){
        updatePlayerStats(players[i]);
    }
}

// Cherche l'indice du symbole de la case dans un tableau spécifié, renvoie l'indice de la première occurrence du symbole dans le tableau, sinon renvoie -1
int SymbolIdInArray(Square square, const char array[][10], int size){
    for(int i = 0; i<size; i++){
        if(strcmp(square.symbol, array[i]) == 0)
            return i; // retourne le premier indice correspondant si trouvé dans le tableau
    }
    return -1; //si pas dans le tableau
}

//placement aléatoire de symboles
void random_placement(Square **board, int gridSize, const char tab[][10], int tabSize, const char monsters[][10]){

    if(board == NULL){
        write_crash_report("pointer in parameters is NULL");
        exit(1);
    }

    int randomRow, randomCol;
    for (int i = 0; i < tabSize; i++) {
        do{
            randomRow = rand() % gridSize+1;
            randomCol = rand() % gridSize+1;
        }while(SymbolIdInArray(board[randomRow][randomCol], monsters, 4) == -1); //vérification que la case choisie aléatoirement ne soit pas déjà une arme/trésor
        strcpy(board[randomRow][randomCol].symbol, tab[i]);
    }
}

//Initialisation de la map aléatoire de symboles
void initialize_map(Square **board, int boardSize, int gridSize, const char monsters[][10], const char weapons[][10], const char treasures[][10]) {

    if(board == NULL){
        write_crash_report("pointer in parameters is NULL");
        exit(1);
    }
    //remplissage du labyrinthe par des monstres aléatoires et initialisation de l'état de base du plateau (cartes retournées et remplies)
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            board[i][j].emptied = 0;
            board[i][j].flipped = 1;
            if((i == 0 && j == gridSize-1) || (i == 2 && j == 0) || (i == gridSize+1 && j == 2) || (i == gridSize-1 && j == gridSize+1)){
                strcpy(board[i][j].symbol, START); //placement des cases de départ
            }
            else if((i%(boardSize-1) == 0) || (j%(boardSize-1) == 0)){
                strcpy(board[i][j].symbol, " "); //vide autour des cases de départ
            }
            else{
                strcpy(board[i][j].symbol, monsters[rand() % 4]); //placement des monstres
                board[i][j].flipped = 0;
            }
        }
    }

    //placement aléatoire des trésors
    random_placement(board, gridSize, treasures, 5, monsters);
    //placement aléatoire des armes antiques
    random_placement(board, gridSize, weapons, 4, monsters);
}

//Création et initialisation d'un tableau 2D de structures Square <=> création d'un plateau de jeu
Square **create_board(int boardSize, int gridSize, const char monsters[][10], const char weapons[][10], const char treasures[][10]) {
    Square **board = malloc(boardSize * sizeof(Square*));
    if (board == NULL) { //gérer si l'allocation a échoué
        write_crash_report("board memory allocation failed");
        exit(1);
    }

    //pour chaque ligne du plateau
    for (int i = 0; i < boardSize; i++) {
        board[i] = malloc(boardSize * sizeof(Square)); //allouer de l'espace pour chacune des lignes du plateau (pour chacun des tableaux de char*)
        if (board[i] == NULL) { //gérer si l'allocation a échoué
            write_crash_report("board memory allocation failed");
            exit(1);
        }
    }

    initialize_map(board, boardSize, gridSize, monsters, weapons, treasures); //placement des symboles dans le labyrinthe

    return board;
}

//libération de la mémoire allouée pour le plateau
void free_board(Square **board, int size) {

    if(board == NULL){
        write_crash_report("pointer in parameters is NULL");
        exit(1);
    }

    for (int i = 0; i < size; i++) {
        free(board[i]); //libération de la mémoire des lignes
    }
    free(board); //libération du plateau
}

//intialisation d'un joueur
int init_player(Player* player, int num, const char* symbol, int start_x, int start_y) {

    if(symbol == NULL || player == NULL){
        write_crash_report("pointer in parameters is NULL");
        exit(1);
    }

    player->number = num;
    strcpy(player->symbol, symbol);

    printf("Entrer un prénom pour le joueur %d : ", player->number);
    scanf("%s", player->name);
    flush_input_buffer(); //au cas où le joueur entre un nom avec des espaces

    player->start_x = start_x;
    player->start_y = start_y;
    player->position_x = player->start_x;
    player->position_y = player->start_y;

    player->ancientWeapon_found = 0;
    player->treasure_found = 0;

    player->score = 0;
    player->total_treasures = 0;
    player->killed_monsters = 0;
    player->flip_cards = 0;
}

void print_board(Square **board, int boardSize, Player player) { //afficher le plateau avec le joueur actif

    if(board == NULL){
        write_crash_report("pointer in parameters is NULL");
        exit(1);
    }

    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            if(player.position_x == i && player.position_y == j){ //affichage du joueur selon sa position
                printf("%s ", player.symbol);
            }
            else if (board[i][j].flipped == 1){ //affichage des cartes retournées
                if(board[i][j].emptied == 1){ //monstres vaincus (cases vides)
                    printf("%s ", EMPTY);
                }
                else{
                    printf("%s ", board[i][j].symbol); //trésors et armes découverts
                }
            }

            else {
                printf("%s ", HIDDEN); //cartes retournées
            }
        }
        printf("\n");
    }
    printf("\n");
}

////////////        A SUPPRIMER, UNIQUEMENT POUR TESTER        ////////////
void print_board_admin(Square **board, int boardSize, Player player) { //print board mais on voit toutes les cases

    if(board == NULL){
        write_crash_report("pointer in parameters is NULL");
        exit(1);
    }

    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            if(player.position_x == i && player.position_y == j){
                printf("%s ", player.symbol);
            }
            else if(board[i][j].emptied == 1){
                printf("%s ", EMPTY);
            }
            else {
                printf("%s ", board[i][j].symbol);
            }
        }
        printf("\n");
    }
    printf("\n");
}

int weapon_choice(Player *player) {
    if(player == NULL){
        write_crash_report("pointer in parameters is NULL");
        exit(1);
    }

    printf("Choisissez une arme pour vous préparer à un potentiel combat...\n");
    printf("1 : Torche\n2 : Bouclier réfléchissant\n3 : Hâche\n4 : Arc\n");
    int i = 0; // i est le nombre d'erreurs que le joueur commet. C'est-à-dire quand ils donnent autre chose que demandé

    scanf("%d", &(player->weapon));
    //player->weapon = getint()-1;  PROBLEME
    flush_input_buffer();
    player->weapon -= 1;
    while (player->weapon < 0 || player->weapon > 3) {
        i++; // on incrémente le nombre d'erreurs
        if (i >= 3) { //trop d'erreurs alors le jeu prend des mesures
            printf("Ok si vous insistez...pas d'arme...\n");
            player->weapon = -1;
            waiting();
            break;
        }
        cursor_move('A', 1); //déplace le curseur d'une case vers le haut pour remplacer la dernière ligne
        printf("Bien essayé mais vous n'avez pas d'autres armes.\n");
        //player->weapon = getint()-1; PROBLEME
        scanf("%d", &(player->weapon));
        flush_input_buffer();
        player->weapon -= 1;
    }
}

//Choisir des coordonnées x et y ne sortant pas du tableau (allant de 0 à 6). Sera utilisée pour la téléportation et pour le totem de transmutation.
void choose_coordinates(Square **board, int* x, int* y, int boardSize, int gridSize) {

    if(board == NULL || x == NULL ||y == NULL){
        write_crash_report("pointer in parameters is NULL");
        exit(1);
    }

    printf("\nChoisir une case en entrant les numéros de la ligne,colonne : ");
    scanf("%d,%d", x, y);
    flush_input_buffer();
    //vérification que les coordonnées saisies ne soient pas hors du tableau ou une case déjà retournée.
    if (*x < 1 || *x > gridSize || *y < 1 || *y > gridSize || board[*x][*y].flipped == 1) {
        printf("Coordonnées invalides.");
        choose_coordinates(board, x, y, boardSize, gridSize);
    }
}

//Vérifie si la case entrée est déjà retournée, ou est le bord de la map, ou est une case de départ. Renvoie 1 si elle est l'une de ces conditions, 0 sinon.
int CheckSquareInvalid(Square square) {
    return (square.flipped == 1 || strcmp(square.symbol, " ") == 0 || strcmp(square.symbol, START) == 0);
}

//Vérifie si l'index spécifié est valide pour être recherché dans un tableau. Renvoie 1 si hors du tableau, 0 sinon.
int CheckIndexOutOfArray(int i, int arraySize){
    return (i < 0 || i > arraySize);
}

//Fonction de choix de direction dans laquelle se déplacer
void choose_direction(Square **board, Player player, int* x, int* y, int boardSize) {
    if(board == NULL || x == NULL ||y == NULL){
        write_crash_report("pointer in parameters is NULL");
        exit(1);
    }

    printf("Choisir une direction :\n");

    //vérifie les directions possibles à proposer
    int validUp = (CheckIndexOutOfArray(player.position_x-1, boardSize)) ? 0 : !CheckSquareInvalid(board[player.position_x-1][player.position_y]);
    int validLeft = (CheckIndexOutOfArray(player.position_y-1, boardSize)) ? 0 : !CheckSquareInvalid(board[player.position_x][player.position_y-1]);
    int validRight = (CheckIndexOutOfArray(player.position_y+1, boardSize)) ? 0 : !CheckSquareInvalid(board[player.position_x][player.position_y+1]);
    int validDown = (CheckIndexOutOfArray(player.position_x+1, boardSize)) ? 0 : !CheckSquareInvalid(board[player.position_x+1][player.position_y]);

    if(validUp){
        printf("Haut\n[8]\n");
    }
    if(validLeft){
        printf("Gauche [4]");
    }
    if(validRight){
        printf(" [6] Droite");
    }
    if(validDown){
        printf("\n[2]\nBas");
    }

    printf("\n");
    int input;
    scanf("%d",&input);

    //vérification que la direction entrée soit valide
    while ((input != 8 || validUp == 0) && (input != 6 || validRight == 0) && (input != 4 || validLeft == 0) && (input != 2 || validDown == 0)) {
        printf("Direction invalide.\n");
        scanf("%d",&input);
        flush_input_buffer();
    }

    //envoie au reste du programme les coordonnées vers lesquelles le joueur veut se déplacer
    switch(input){
        case 8:
            *x = player.position_x-1;
            *y = player.position_y;
            break;
        case 6:
            *x = player.position_x;
            *y = player.position_y+1;
            break;
        case 4:
            *x = player.position_x;
            *y = player.position_y-1;
            break;
        case 2:
            *x = player.position_x+1;
            *y = player.position_y;
            break;
    }
}

// Vérifie que l'arme choisie est correcte par rapport à l'ennemi affronté. Renvoie 1 si oui, 0 sinon
int fight(Player player, int monster) {
    if (player.weapon == monster) {
        return 1;
    }
    return 0;
}

//Sous fonction pour gérer les découvertes d'armes antiques. Renvoie 1 si l'arme antique correspond à celle du joueur, 0 sinon.
int event_ancient_weapon(Player* player, int ancientWeapon){

    if(player == NULL){
        write_crash_report("pointer in parameters is NULL");
        exit(1);
    }

    printf("Vous avez découvert une arme antique !\n");
    if (player->number == ancientWeapon+1) {
        printf("C'est la vôtre !\n");
        player->ancientWeapon_found = 1;
        return 1;
    }
    else{
        printf("Il semblerait que ce ne soit pas la vôtre... Vous continuez votre chemin.\n");
        return 0;
    }
}

//Fonction qui gère les rencontres de monstres/d'objets lors du déplacement. Renvoie 1 si le joueur peut continuer à se déplacer librement, 0 si le tour doit se finir
int event_manager(int* x, int* y, Square **board, int boardSize, int gridSize, Player* player, const char monsters[][10], const char weapons[][10], const char treasures[][10]) {

    if(board == NULL || player == NULL || x == NULL ||y == NULL){
        write_crash_report("pointer in parameters is NULL");
        exit(1);
    }

    board[*x][*y].flipped = 1;
    print_board(board, boardSize, *player);

    int symbolIdMonsters = SymbolIdInArray(board[*x][*y], monsters, 4);
    int symbolIdWeapons = SymbolIdInArray(board[*x][*y], weapons, 4);
    int symbolIdTreasures = SymbolIdInArray(board[*x][*y], treasures, 5);

    if(symbolIdMonsters != -1){ //si la carte retournée est un monstre, engager un combat
        if(fight(*player, symbolIdMonsters)){ //le joueur a choisi la bonne arme contre ce monstre
            printf("Combat gagné !\n");
            board[*x][*y].emptied = 1;
            //player->killedmonster +1
        }
        else{
            printf("Combat perdu !\n"); //le joueur n'a pas choisi la bonne arme contre ce monstre
            return 0;
        }
    }

    else if(symbolIdWeapons != -1){ //si la carte retournée est une arme antique, vérifier si elle appartient au joueur
        event_ancient_weapon(player, symbolIdWeapons);
    }
    else if(symbolIdTreasures != -1){
        switch(symbolIdTreasures){ //si la carte retournée est un trésor/totem/portail
            case 0:
                printf("Vous avez découvert un trésor !\n");
                player->treasure_found = 1;
                //player treasures total +1
                break;

            case 1:
                printf("Vous avez découvert un portail magique de téléportation. Vous pouvez choisir n'importe quelle case encore cachée où vous téléporter. Mais avant cela...\n");
                weapon_choice(player);
                choose_coordinates(board, x, y, boardSize, gridSize);
                return event_manager(x, y, board, boardSize, gridSize, player, monsters, weapons, treasures);

            case 2:
                printf("Vous avez découvert un totem de transmutation. Vous pouvez choisir n'importe quelle case encore cachée pour l'échanger avec le totem. Cela mettra fin à votre tour.\n");

                int l,c;
                choose_coordinates(board, &l, &c, boardSize, gridSize);
                while((l == 1 && c == gridSize-1) || (l == 2 && c == 1) || (l == gridSize && c == 2) || (l == gridSize-1 && c == gridSize)){ //interdiction de prendre les cases devant celles de départ
                    printf("Coordonnées invalides.");
                    choose_coordinates(board, &l, &c, boardSize, gridSize);
                }

                Square temp = board[*x][*y];
                board[*x][*y] = board[l][c];
                board[l][c] = temp;

                printf("Poof!\n");
                print_board(board, boardSize, *player);
                return 0;
        }
    }
    return 1;
}

//reset du plateau à la mort du joueur ou si le joueur est bloqué
void reset(Square **board, int boardSize, int gridSize, Player* player){
    for(int i = 1; i <= gridSize; i++){
        for (int j = 1; j <= gridSize; j++){
            board[i][j].flipped = 0; //retourne les cartes
            board[i][j].emptied = 0; //les monstres reviennent à la vie
        }
    }
    int next = 1;
    player->number +=1;
    printf("%s ", player->symbol);
    if(next == 1){
        player->position_x = player->start_x+2;
        player->position_y = player->start_y-4;
    }
    if(next == 2){
        player->position_x = player->start_x+4;
        player->position_y = player->start_y+2;
    }
    if(next == 3){
        player->position_x = player->start_x+4;
        player->position_y = player->start_y+2;
    }
    next +=1;
    player->treasure_found = 0; //il doit retrouver son trésor et son arme à nouveau
    player->ancientWeapon_found = 0;
    printf("Vous venez de perdre...\n");
    printf("A vous de jouer %s \n", player->name);
    print_board(board, boardSize, *player);


}

//Déplacement d'un joueur et évènements = tour entier d'un joueur. Ne s'arrête qu'une fois que le joueur meurt
void turn(Square **board, int boardSize, int gridSize, Player* player, const char monsters[][10], const char weapons[][10], const char treasures[][10]) {

    if(board == NULL || player == NULL){
        write_crash_report("pointer in parameters is NULL");
        exit(1);
    }

    weapon_choice(player); // choix de l'arme
    int x, y;
    choose_direction(board, *player, &x, &y, boardSize); //choix de la direction dans laquelle se déplacer

    if(event_manager(&x, &y, board, boardSize, gridSize, player, monsters, weapons, treasures)){ //déplacer le joueur sur la case choisie si il n'est pas mort et peut rejouer
        player->position_x = x;
        player->position_y = y;
        //player case +1

        //avant de rejouer, vérification si le joueur est bloqué dans le labyrinthe
        if(CheckSquareInvalid(board[player->position_x+1][player->position_y]) && CheckSquareInvalid(board[player->position_x-1][player->position_y])
             && CheckSquareInvalid(board[player->position_x][player->position_y+1]) && CheckSquareInvalid(board[player->position_x][player->position_y-1])){
            print_board(board, boardSize, *player);
            printf("Vous êtes bloqué dans le labyrinthe...\n");
            reset(board, boardSize, gridSize, player);
        }
        else{
            print_board(board, boardSize, *player);
            turn(board, boardSize, gridSize, player, monsters, weapons, treasures);
        }
    }
    else{ //cas où le joueur est mort ou a utilisé un totem de transmutation, ce qui réinitialise sa position et réinitialise la carte
        reset(board, boardSize, gridSize, player);
    }
}

int choose_board_dimensions(){
    int dim;
    do{
        printf("Choisissez une dimension pour le labyrinthe (>= 5, version classique : 5) : ");
        scanf("%d", &dim);
        flush_input_buffer();

        if (dim < 5 && dim > 0){
            printf("Vous avez peur de vous perdre..?\n");
        }
        else if(dim <= 0){
            printf("Euh... c'est embêtant... vous ne voulez pas jouer..?\n0 : En effet\n1 : Si\n");
            int ans;
            scanf("%d", &ans);
            flush_input_buffer();
            if(ans == 0){
                printf("Euh... ok...\n");
                waiting();
                exit(1);
            }
        }
        else if (dim > 15){
            printf("Ça fait beaucoup là, non ?\n");
        }
    }while(dim < 5 || dim > 15);
    return dim;
}

int main() {
    srand(time(NULL));

    const char weapons[4][10] = {STICK, SWORD, DAGGER, SPELLBOOK}; //Symboles des armes
    const char treasures[5][10] = {CHEST, PORTAL, TOTEM, TOTEM, CHEST}; //Symboles des objets spéciaux (coffres, totems, portails)
    const char monsters[4][10] = {ZOMBIE, BASILISK, TROLL, HARPY}; //Symboles des monstres
    const char adventurers[4][10] = {RANGER, THIEF, MAGICIAN, WARRIOR}; //Symboles des monstres

    int gridSize = choose_board_dimensions();
    int boardSize = gridSize+2;

    const int start_x[4] = {0, 2, gridSize-1, gridSize+1}; //abscisses des cases de départ de chaque joueur
    const int start_y[4] = {gridSize-1, 0, gridSize+1, 2}; //ordonnées des cases de départ de chaque joueur

    ////////////        CREATION ET INITIALISATION DU PLATEAU DE CASES ////////////
    Square **board = create_board(boardSize, gridSize, monsters, weapons, treasures);

    ///////////// MENU + NB DE PLAYER + NOM PLAYER ///////////
    int j = 0;
    int x = menu();
    printf("%d", x);
    printf("\n\n");
    Player players[x];
    do{
        init_player(&players[j], j+1, adventurers[j], start_x[j], start_y[j]);
        printf("Bienvenue %s !\n", players[j].name);
        j++;
    }while(j<x);


    ////////////        A SUPPRIMER, UNIQUEMENT POUR TESTER        ////////////
   // print_board_admin(board, boardSize, players[0]); //print board mais on voit toutes les cases
    //printf("\n\n");

    //print_board(board, boardSize, players[0]);

    ////////////        WIP TEST GAMEPLAY VERSION SOLO        ////////////
    //for (int i = 0; i < 40; i++) {
        //turn(board, boardSize, gridSize, &players[0], monsters, weapons, treasures);
    //5}

    ////////////        WIP TEST GAMEPLAY        ////////////
    for(int i=0; i< 5; i++){
        while(players[i].treasure_found != 1 && players[i].ancientWeapon_found != 1) {
            for(int j=0; j< 5; j++){
                turn(board, BOARD_SIZE, GRID_SIZE, &players[j], monsters, weapons, treasures);
                if(players[j].treasure_found == 1 && players[j].ancientWeapon_found == 1){
                    printf("Bravo %s ! Vous venez de remporter cette partie ! \n", players[j].name);
                    players[j].score +=1;
                    break;
                }
            }
        }
    }


    ////////////        LIBERATION DE LA MEMOIRE        ////////////
    free_board(board, boardSize);

    return 0;
}
