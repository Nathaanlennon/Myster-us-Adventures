#ifndef HEADER_H
#define HEADER_H

///////            STRUCTURES            //////////

// Structure pour une case du plateau de jeu
typedef struct {
    char symbol[10];
    int flipped; //variable booléenne qui vaut 1 si la case est retournée, 0 sinon
    int emptied; //variable booléenne qui vaut 1 si la case est vidée (monstre battu/objet pris), 0 sinon
} Square;

typedef struct {
    int number;
    char symbol[10];
    int weapon;

    int start_x; //case de départ attribuée abscisse
    int start_y; //case de départ attribuée ordonnée
    int position_x; //position durant le tour abscicsse
    int position_y; //position durant le tour abscicsse

    int ancientWeapon_found;
    int treasure_found;

    char name[];
    //color mais j'ai la flemme pour l'instant
} Player;

///////            USUAL            //////////
/*
void clear_all();
void clear_part(int line, int column);
void cursor_move(char direction, int num);
void flush_input_buffer();
int getint();
void commentary(char tab[]);
void waiting();
void write_crash_report(const char* error_message);
int SymbolIdInArray(Square square, const char array[][10], int size);
void print_board(Square **board, int boardSize, Player* player);
void print_board_admin(Square **board, int boardSize, Player* player);

///////            INIT            //////////
void random_placement(Square **board, int gridSize, const char tab[][10], int tabSize, const char monsters[][10]);
void initialize_map(Square **board, int boardSize, int gridSize, const char monsters[][10], const char weapons[][10], const char treasures[][10]);
Square **create_board(int boardSize, int gridSize, const char monsters[][10], const char weapons[][10], const char treasures[][10]);
void free_board(Square **board, int size);
void init_player(Player* player, int num, const char* symbol, int start_x, int start_y);

///////            PLAYERTURN            //////////
void weapon_choice(Player *player);
void choose_coordinates(Square **board, int* x, int* y, int boardSize, int gridSize);
int fight(Player player, int monster);
int event_ancient_weapon(Player* player, int ancientWeapon);
int event_manager(int* x, int* y, Square **board, int boardSize, int gridSize, Player* player, const char monsters[][10], const char weapons[][10], const char treasures[][10]);
void move(Square **board, int boardSize, int gridSize, Player* player, const char monsters[][10], const char weapons[][10], const char treasures[][10]);

*/
#endif