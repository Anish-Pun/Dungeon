#include "dungeon.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

// Create a dungeon with the specified number of rooms
struct Dungeon *CreateDungeon(int roomCount)
{
    srand((unsigned int)time(NULL)); // Seed the random number generator

    struct Dungeon *dungeon = (struct Dungeon *)malloc(sizeof(struct Dungeon));
    if (!dungeon)
    {
        perror("Failed to allocate memory for dungeon");
        exit(EXIT_FAILURE);
    }

    dungeon->rooms = (struct Room *)malloc(sizeof(struct Room) * roomCount);
    if (!dungeon->rooms)
    {
        perror("Failed to allocate memory for rooms");
        free(dungeon);
        exit(EXIT_FAILURE);
    }

    dungeon->roomCount = roomCount;

    // Initialize rooms
    for (int i = 0; i < roomCount; i++)
    {
        dungeon->rooms[i].id = i;
        dungeon->rooms[i].ConnectedRoomsCount = 0;
        dungeon->rooms[i].monster = 0;
        dungeon->rooms[i].Crown = 0;
        dungeon->rooms[i].monsterHp = 0;
        dungeon->rooms[i].monsterDamage = 0;
        dungeon->rooms[i].Items = ITEM_NONE; // No item by default
        for (int j = 0; j < MaxConnectedRoom; j++)
        {
            dungeon->rooms[i].connectedRooms[j] = -1; // Initialize with -1 (no connection)
        }
    }

    // Ensure connectivity starting from room 0
    for (int i = 1; i < roomCount; i++)
    {
        int randomRoom = rand() % i;
        ConnectRooms(dungeon, i, randomRoom);
    }

    // Add random additional connections
    int extraConnections = roomCount; // Example: Add as many extra connections as there are rooms
    for (int i = 0; i < extraConnections; i++)
    {
        int room1 = rand() % roomCount;
        int room2 = rand() % roomCount;

        if (room1 != room2)
        {
            ConnectRooms(dungeon, room1, room2);
        }
    }

    // Place Items randomly in rooms
    int itemCount = roomCount / 4; // 1/4 of the rooms will have items
    int placedHealthPotions = 0;
    int placedDamageBoosts = 0;

    while (placedHealthPotions + placedDamageBoosts < itemCount)
    {
        int randomRoom = rand() % roomCount;

        // check if room is valid and not already occupied
        if (randomRoom == 0)
            continue; // Skip Room 0
        if (dungeon->rooms[randomRoom].Items == ITEM_NONE)
        {
            // Alternate between placing health potions and damage boosts
            if (placedHealthPotions < itemCount / 2)
            {
                dungeon->rooms[randomRoom].Items = ITEM_HEALTH_POTION;
                placedHealthPotions++;
            }
            else if (placedDamageBoosts < itemCount / 2)
            {
                dungeon->rooms[randomRoom].Items = ITEM_DAMAGE_BOOST;
                placedDamageBoosts++;
            }
        }
    }

    // Debugging: Ensure both item types are placed
    printf("\nDebug: Items placed - Health Potions: %d, Damage Boosts: %d\n", placedHealthPotions, placedDamageBoosts);

    int monsterCount = 3;  // 3 monsters in the dungeon
    int placedMonster = 0; // Counter for placed monsters

    while (placedMonster < monsterCount)
    {
        int randomRoom = rand() % roomCount;

        if (randomRoom == 0)
            continue; // Skip Room 0
        if (!dungeon->rooms[randomRoom].monster &&
            !dungeon->rooms[randomRoom].monster)
        {

            dungeon->rooms[randomRoom].monster = 1;

            // Assign monster type and stats
            if (placedMonster == 0)
            {
                // minion
                dungeon->rooms[randomRoom].monsterType = 1;
                dungeon->rooms[randomRoom].monsterHp = 25;    // Low HP
                dungeon->rooms[randomRoom].monsterDamage = 3; // Low damage
            }
            else if (placedMonster == 1)
            {
                // mini boss
                dungeon->rooms[randomRoom].monsterType = 2;
                dungeon->rooms[randomRoom].monsterHp = 50;     // Medium HP
                dungeon->rooms[randomRoom].monsterDamage = 10; // Medium damage
            }
            else if (placedMonster == 2)
            {
                // Boss
                dungeon->rooms[randomRoom].monsterType = 3;
                dungeon->rooms[randomRoom].monsterHp = 75;     // High HP
                dungeon->rooms[randomRoom].monsterDamage = 15; // High damage
            }

            placedMonster++;
        }
    }

    // Place the Crown in a random room.
    int crownRoom;
    do
    {
        crownRoom = rand() % roomCount;
    } while (crownRoom == 0 || dungeon->rooms[crownRoom].monster);
    dungeon->rooms[crownRoom].Crown = 1; // Place the crown in the room
    return dungeon;
}

// Connect two rooms in the dungeon
void ConnectRooms(struct Dungeon *dungeon, int room1, int room2)
{
    if (room1 < 0 || room1 >= dungeon->roomCount || room2 < 0 || room2 >= dungeon->roomCount)
    {
        fprintf(stderr, "Invalid room IDs: %d, %d\n", room1, room2);
        return;
    }

    struct Room *r1 = &dungeon->rooms[room1];
    struct Room *r2 = &dungeon->rooms[room2];

    // Check if the rooms are already connected
    for (int i = 0; i < r1->ConnectedRoomsCount; i++)
    {
        if (r1->connectedRooms[i] == room2)
        {
            return; // Already connected
        }
    }

    // Add connection if possible
    if (r1->ConnectedRoomsCount < MaxConnectedRoom && r2->ConnectedRoomsCount < MaxConnectedRoom)
    {
        r1->connectedRooms[r1->ConnectedRoomsCount++] = room2;
        r2->connectedRooms[r2->ConnectedRoomsCount++] = room1;
    }
}

// Free the memory allocated for the dungeon
void FreeDungeon(struct Dungeon *dungeon)
{
    if (dungeon)
    {
        free(dungeon->rooms);
        free(dungeon);
    }
}