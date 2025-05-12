#ifndef DUNGEON_H
#define DUNGEON_H

// Maximum connections for each room
#define MaxConnectedRoom 4

// Item types
#define ITEM_NONE 0
#define ITEM_HEALTH_POTION 1
#define ITEM_DAMAGE_BOOST 2


// Structure to represent a room
struct Room {
    int id;                          // ID for the room
    int connectedRooms[MaxConnectedRoom]; // IDs of the connected rooms
    int ConnectedRoomsCount;         // Number of connected rooms
    int Items;
};

struct Dungeon {
    struct Room* rooms;              // Array of rooms
    int roomCount;                   // Total number of rooms
};

// Function prototypes
struct Dungeon* CreateDungeon(int roomCount);
void ConnectRooms(struct Dungeon* dungeon, int room1, int room2);
void PrintDungeon(struct Dungeon* dungeon);
void FreeDungeon(struct Dungeon* dungeon);

#endif // DUNGEON_H