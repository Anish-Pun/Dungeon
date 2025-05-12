#include "player.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Initialize the player
void InitializePlayer(struct Player *player)
{
    printf("What is your name? ");
    fgets(player->name, sizeof(player->name), stdin);
    // Remove trailing newline character
    size_t len = strlen(player->name);
    if (len > 0 && player->name[len - 1] == '\n')
    {
        player->name[len - 1] = '\0';
    }
    player->currentRoom = 0; // Start in Room 0

    // Welcome message & Instructions
    printf("\n+------------------------------------------+\n");
    printf("| Welcome, %s!                              |\n", player->name);
    printf("+------------------------------------------+\n");
    printf("| You have been spawned in Room 0.         |\n");
    printf("| Your goal is to get the crown and win    |\n");
    printf("| the game, but be careful! There are      |\n");
    printf("| monsters in some rooms, so choose wisely.|\n");
    printf("+------------------------------------------+\n");
}

// Move the player to a different room
void MovePlayer(struct Player *player, struct Dungeon *dungeon)
{
    struct Room *currentRoom = &dungeon->rooms[player->currentRoom];

    // Display connected rooms
    printf("\nIn front of you, you see %d room(s): ", currentRoom->ConnectedRoomsCount);
    for (int i = 0; i < currentRoom->ConnectedRoomsCount; i++)
    {
        printf("Room %d", currentRoom->connectedRooms[i]);
        if (i < currentRoom->ConnectedRoomsCount - 1)
        {
            printf(", ");
        }
    }
    printf(".\n");

    char input[10];
    while (1)
    {
        printf("Which one will you choose? ");
        scanf("%s", input);

        // Convert input to an integer for room selection
        int chosenRoom = atoi(input);

        // Check if the chosen room is valid
        int isValid = 0;
        for (int i = 0; i < currentRoom->ConnectedRoomsCount; i++)
        {
            if (currentRoom->connectedRooms[i] == chosenRoom)
            {
                isValid = 1;
                break;
            }
        }

        if (isValid)
        {
            player->currentRoom = chosenRoom;
            printf("\nYou have moved to Room %d.\n", player->currentRoom);
            break;
        }
        else
        {
            printf("You cannot go to Room %d from here. Please choose a valid room.\n", chosenRoom);
        }
    }
}