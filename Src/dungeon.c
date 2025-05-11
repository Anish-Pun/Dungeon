#include "dungeon.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

// Create a dungeon with the specified number of rooms
struct Dungeon* CreateDungeon(int roomCount) {
    srand((unsigned int)time(NULL)); // Seed the random number generator

    struct Dungeon* dungeon = (struct Dungeon*)malloc(sizeof(struct Dungeon));
    if (!dungeon) {
        perror("Failed to allocate memory for dungeon");
        exit(EXIT_FAILURE);
    }

    dungeon->rooms = (struct Room*)malloc(sizeof(struct Room) * roomCount);
    if (!dungeon->rooms) {
        perror("Failed to allocate memory for rooms");
        free(dungeon);
        exit(EXIT_FAILURE);
    }

    dungeon->roomCount = roomCount;

    // Initialize rooms
    for (int i = 0; i < roomCount; i++) {
        dungeon->rooms[i].id = i;
        dungeon->rooms[i].ConnectedRoomsCount = 0;
        for (int j = 0; j < MaxConnectedRoom; j++) {
            dungeon->rooms[i].connectedRooms[j] = -1; // Initialize with -1 (no connection)
        }
    }

    // Ensure connectivity starting from room 0
    for (int i = 1; i < roomCount; i++) {
        int randomRoom = rand() % i;
        ConnectRooms(dungeon, i, randomRoom);
    }

    // Add random additional connections
    int extraConnections = roomCount; // Example: Add as many extra connections as there are rooms
    for (int i = 0; i < extraConnections; i++) {
        int room1 = rand() % roomCount;
        int room2 = rand() % roomCount;

        if (room1 != room2) {
            ConnectRooms(dungeon, room1, room2);
        }
    }

    return dungeon;
}

// Connect two rooms in the dungeon
void ConnectRooms(struct Dungeon* dungeon, int room1, int room2) {
    if (room1 < 0 || room1 >= dungeon->roomCount || room2 < 0 || room2 >= dungeon->roomCount) {
        fprintf(stderr, "Invalid room IDs: %d, %d\n", room1, room2);
        return;
    }

    struct Room* r1 = &dungeon->rooms[room1];
    struct Room* r2 = &dungeon->rooms[room2];

    // Check if the rooms are already connected
    for (int i = 0; i < r1->ConnectedRoomsCount; i++) {
        if (r1->connectedRooms[i] == room2) {
            return; // Already connected
        }
    }

    // Add connection if possible
    if (r1->ConnectedRoomsCount < MaxConnectedRoom && r2->ConnectedRoomsCount < MaxConnectedRoom) {
        r1->connectedRooms[r1->ConnectedRoomsCount++] = room2;
        r2->connectedRooms[r2->ConnectedRoomsCount++] = room1;
    }
}

// Print the dungeon structure
void PrintDungeon(struct Dungeon* dungeon) {
    printf("\n+------------------------------------------+\n");
    printf("| Dungeon Structure                        |\n");
    printf("+------------------------------------------+\n");

    for (int i = 0; i < dungeon->roomCount; i++) {
        struct Room* room = &dungeon->rooms[i];
        printf("Room %d: ", room->id);
        for (int j = 0; j < room->ConnectedRoomsCount; j++) {
            printf("%d ", room->connectedRooms[j]);
        }
        printf("\n");
    }
    printf("+------------------------------------------+\n");
}

// Free the memory allocated for the dungeon
void FreeDungeon(struct Dungeon* dungeon) {
    if (dungeon) {
        free(dungeon->rooms);
        free(dungeon);
    }
}