#include "player.h"
#include "dungeon.h"
#include <stdio.h>
#include <stdlib.h>

// Function to print the dungeon structure
int main() {
    int roomCount = 10; // Example: Create a dungeon with 10 rooms
    struct Dungeon* dungeon = CreateDungeon(roomCount);

    // Print the dungeon structure
    PrintDungeon(dungeon);

    struct Player player;
    InitializePlayer(&player);

    while (1){
        MovePlayer(&player, dungeon);
    }

    // Free the dungeon memory
    FreeDungeon(dungeon);

    return 0;
}