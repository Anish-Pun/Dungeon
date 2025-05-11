#include <stdio.h>
#include <stdlib.h>
#include "dungeon.h"
#include "player.h"

// Function to print the dungeon structure
int main() {
    int roomCount = 10; // Example: Create a dungeon with 10 rooms
    struct Dungeon* dungeon = CreateDungeon(roomCount);

    // Print the dungeon structure
    PrintDungeon(dungeon);

    // Free the dungeon memory
    FreeDungeon(dungeon);

    return 0;
}