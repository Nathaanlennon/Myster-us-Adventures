#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/struct.h"
#include "include/usual.h"

//permet de print le contenu d'un fichier dans le terminal. Retourne 1 si l'affichage s'est bien passé, 0 sinon.
int printFile(char filename[]){
    clear_all();
    FILE *file = fopen(filename, "r");
    if (file == NULL){
        return 0;
    }

    int character;
    while ((character=fgetc(file)) != EOF){
        printf ("%c", character);
    }
    fclose(file);
    return 1;
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
