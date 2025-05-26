#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
#include "save_load.h"
#include "player.h"
#include "dungeon.h"

void SaveGame(struct Player *player, struct Dungeon *dungeon, const char *filename)
{
    cJSON *root = cJSON_CreateObject();

    // Save player data
    cJSON *playerData = cJSON_CreateObject();
    cJSON_AddStringToObject(playerData, "name", player->name);
    cJSON_AddNumberToObject(playerData, "currentRoom", player->currentRoom);
    cJSON_AddNumberToObject(playerData, "hp", player->hp);
    cJSON_AddNumberToObject(playerData, "damage", player->damage);
    cJSON_AddItemToObject(root, "player", playerData);

    // Save dungeon data
    cJSON *dungeonData = cJSON_CreateObject();
    cJSON_AddNumberToObject(dungeonData, "roomCount", dungeon->roomCount);

    cJSON *roomsArray = cJSON_CreateArray();
    for (int i = 0; i < dungeon->roomCount; i++)
    {
        struct Room *room = &dungeon->rooms[i];
        cJSON *roomData = cJSON_CreateObject();
        cJSON_AddNumberToObject(roomData, "id", room->id);
        cJSON_AddItemToObject(roomData, "connectedRooms",
                              cJSON_CreateIntArray(room->connectedRooms, room->ConnectedRoomsCount));

        // Save Crown location in save file
        if (room->Crown)
        {
            cJSON_AddBoolToObject(roomData, "Crown", 1);
        }

        // Save Item location in save file
        if (room->Items == ITEM_HEALTH_POTION)
        {
            cJSON_AddStringToObject(roomData, "Item", "Health Potion");
        }
        else if (room->Items == ITEM_DAMAGE_BOOST)
        {
            cJSON_AddStringToObject(roomData, "Item", "Damage Boost");
        }

        // Add monster details if present
        if (room->monster)
        {
            cJSON *monsterData = cJSON_CreateObject();
            if (room->monsterType == 1)
            {
                cJSON_AddStringToObject(monsterData, "type", "Low-level Minion");
            }
            else if (room->monsterType == 2)
            {
                cJSON_AddStringToObject(monsterData, "type", "Mini Boss");
            }
            else if (room->monsterType == 3)
            {
                cJSON_AddStringToObject(monsterData, "type", "Boss");
            }
            cJSON_AddNumberToObject(monsterData, "hp", room->monsterHp);
            cJSON_AddNumberToObject(monsterData, "damage", room->monsterDamage);
            cJSON_AddItemToObject(roomData, "monster", monsterData);
        }

        cJSON_AddItemToArray(roomsArray, roomData);
    }
    cJSON_AddItemToObject(dungeonData, "rooms", roomsArray);
    cJSON_AddItemToObject(root, "dungeon", dungeonData);

    // Write JSON to file
    char *jsonString = cJSON_Print(root);
    FILE *file = fopen(filename, "w");
    if (!file)
    {
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

// Load the game state from a JSON file
int LoadGame(struct Player *player, struct Dungeon *dungeon, const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        printf("\n+------------------------------------------+\n");
        printf("| Error: Failed to load the game.          |\n");
        printf("+------------------------------------------+\n");
        return 0;
    }

    // Read the entire file into a string
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    rewind(file);
    char *jsonString = (char *)malloc(fileSize + 1);
    fread(jsonString, 1, fileSize, file);
    jsonString[fileSize] = '\0';
    fclose(file);

    // Parse JSON
    cJSON *root = cJSON_Parse(jsonString);
    if (!root)
    {
        printf("\n+------------------------------------------+\n");
        printf("| Error: Failed to parse the save file.    |\n");
        printf("+------------------------------------------+\n");
        free(jsonString);
        return 0;
    }

    // Load player data
    cJSON *playerData = cJSON_GetObjectItem(root, "player");
    strcpy(player->name, cJSON_GetObjectItem(playerData, "name")->valuestring);
    player->currentRoom = cJSON_GetObjectItem(playerData, "currentRoom")->valueint;
    player->hp = cJSON_GetObjectItem(playerData, "hp")->valueint;
    player->damage = cJSON_GetObjectItem(playerData, "damage")->valueint;

    // Load dungeon data
    cJSON *dungeonData = cJSON_GetObjectItem(root, "dungeon");
    dungeon->roomCount = cJSON_GetObjectItem(dungeonData, "roomCount")->valueint;
    dungeon->rooms = (struct Room *)malloc(sizeof(struct Room) * dungeon->roomCount);

    cJSON *roomsArray = cJSON_GetObjectItem(dungeonData, "rooms");
    for (int i = 0; i < dungeon->roomCount; i++)
    {
        cJSON *roomData = cJSON_GetArrayItem(roomsArray, i);
        dungeon->rooms[i].id = cJSON_GetObjectItem(roomData, "id")->valueint;

        // Load connected rooms
        cJSON *connectedRoomsArray = cJSON_GetObjectItem(roomData, "connectedRooms");
        dungeon->rooms[i].ConnectedRoomsCount = cJSON_GetArraySize(connectedRoomsArray);
        for (int j = 0; j < dungeon->rooms[i].ConnectedRoomsCount; j++)
        {
            dungeon->rooms[i].connectedRooms[j] = cJSON_GetArrayItem(connectedRoomsArray, j)->valueint;
        }

        // Crown
        cJSON *crown = cJSON_GetObjectItem(roomData, "Crown");
        dungeon->rooms[i].Crown = (crown && cJSON_IsTrue(crown)) ? 1 : 0;

        // Item
        cJSON *item = cJSON_GetObjectItem(roomData, "Item");
        if (item && cJSON_IsString(item))
        {
            if (strcmp(item->valuestring, "Health Potion") == 0)
            {
                dungeon->rooms[i].Items = ITEM_HEALTH_POTION;
            }
            else if (strcmp(item->valuestring, "Damage Boost") == 0)
            {
                dungeon->rooms[i].Items = ITEM_DAMAGE_BOOST;
            }
            else
            {
                dungeon->rooms[i].Items = ITEM_NONE;
            }
        }
        else
        {
            dungeon->rooms[i].Items = ITEM_NONE;
        }

        // Monster
        cJSON *monster = cJSON_GetObjectItem(roomData, "monster");
        if (monster)
        {
            dungeon->rooms[i].monster = 1;
            cJSON *type = cJSON_GetObjectItem(monster, "type");
            if (type && cJSON_IsString(type))
            {
                if (strcmp(type->valuestring, "Low-level Minion") == 0)
                {
                    dungeon->rooms[i].monsterType = 1;
                }
                else if (strcmp(type->valuestring, "Mini Boss") == 0)
                {
                    dungeon->rooms[i].monsterType = 2;
                }
                else if (strcmp(type->valuestring, "Boss") == 0)
                {
                    dungeon->rooms[i].monsterType = 3;
                }
            }
            dungeon->rooms[i].monsterHp = cJSON_GetObjectItem(monster, "hp")->valueint;
            dungeon->rooms[i].monsterDamage = cJSON_GetObjectItem(monster, "damage")->valueint;
        }
        else
        {
            dungeon->rooms[i].monster = 0;
            dungeon->rooms[i].monsterType = 0;
            dungeon->rooms[i].monsterHp = 0;
            dungeon->rooms[i].monsterDamage = 0;
        }
    }

    printf("\n+------------------------------------------+\n");
    printf("| Game loaded successfully from %s.        |\n", filename);
    printf("+------------------------------------------+\n");

    // Clean up
    cJSON_Delete(root);
    free(jsonString);
    return 1;
}