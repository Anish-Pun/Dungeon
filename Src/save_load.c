
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
#include "save_load.h"
#include "player.h"
#include "dungeon.h"

void SaveGame(struct Player* player, struct Dungeon* dungeon, const char* filename) 
{
    cJSON *root = cJSON_CreateObject();

    // Save player data
    cJSON *playerData = cJSON_CreateObject();
    cJSON_AddStringToObject(playerData, "name", player->name);
    cJSON_AddNumberToObject(playerData, "currentRoom", player->currentRoom);
    cJSON_AddNumberToObject(playerData, "hp", player->hp);
    cJSON_AddNumberToObject(playerData, "damage", player->damage);
    cJSON_AddItemToObject(root, "player", playerData);
    
    // Write JSON to file
    char* jsonString = cJSON_Print(root);
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("\nError: Could not save the game.\n");
        cJSON_Delete(root);
        free(jsonString);
        return;
    }
    fprintf(file, "%s", jsonString);
    fclose(file);

    printf("\nGame saved successfully to %s.\n", filename);

    // Clean up
    cJSON_Delete(root);
    free(jsonString);
}


