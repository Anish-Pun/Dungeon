#include "player.h"
#include "dungeon.h"
#include <stdio.h>
#include <stdlib.h>

void WelcomeMsg(void);

int main() {
    printf("Starting the program...\n");
    int roomCount = 10; // 10 rooms in the dungeon

    struct Dungeon* dungeon = CreateDungeon(roomCount);

    struct Player player;
    InitializePlayer(&player);

    // Game Loop
    while (1) { 
        struct Room* currentRoom = &dungeon->rooms[player.currentRoom];

        // Display connected rooms for the current room
        printf("\nYou are in Room %d.\n", player.currentRoom);
        printf("Connected rooms: ");
        for (int i = 0; i < currentRoom->ConnectedRoomsCount; i++) {
            printf("%d ", currentRoom->connectedRooms[i]);
        }
        printf("\n");

        // Prompt the player for an action
        printf("\nEnter your action (D for debug, or choose a room): ");
        char input[10];
        scanf("%s", input);

        // Handle the player's input
        if (input[0] == 'D' || input[0] == 'd') {
            // Show debug information
            PrintDungeon(dungeon);
        } else {
            // Converting input to an integer for room selection
            int chosenRoom = atoi(input);

            // Check if the chosen room is valid
            int isValid = 0;
            for (int i = 0; i < currentRoom->ConnectedRoomsCount; i++) {
                if (currentRoom->connectedRooms[i] == chosenRoom) {
                    isValid = 1;
                    break;
                }
            }

            if (isValid) {
                player.currentRoom = chosenRoom;
                printf("\nYou have moved to Room %d.\n", player.currentRoom);
            } 
            else {
                printf("You cannot go to Room %d from here. Please choose a valid room.\n", chosenRoom);
            }
        }
    }

    // Free the dungeon memory
    FreeDungeon(dungeon);

    return 0;
}


void WelcomeMsg(void)
{
    printf("     __        __   _                          \n");
    printf("    \\ \\      / /__| | ___ ___  _ __ ___   ___ \n");
    printf("     \\ \\ /\\ / / _ \\ |/ __/ _ \\| '_ ` _ \\ / _ \\\n");
    printf("      \\ V  V /  __/ | (_| (_) | | | | | |  __/\n");
    printf("       \\_/\\_/ \\___|_|\\___\\___/|_| |_| |_|\\___|\n");
}