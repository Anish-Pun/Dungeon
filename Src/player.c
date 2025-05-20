#include "player.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Initialize the player
void InitializePlayer(struct Player *player)
{
    printf("\n+------------------------------------------+\n");
    printf("| Welcome, hero!                           |\n");
    printf("| You stand at the entrance of the dungeon |\n");
    printf("| where legends are born and lost.         |\n");
    printf("+------------------------------------------+\n");
    printf("| What is your name, brave adventurer?     |\n");
    printf("+------------------------------------------+\n");
    printf("> ");
    while (getchar() != '\n'); // Clear the input buffer
    fgets(player->name, sizeof(player->name), stdin);
    // Remove trailing newline character
    size_t len = strlen(player->name);
    if (len > 0 && player->name[len - 1] == '\n')
    {
        player->name[len - 1] = '\0';
    }
    player->currentRoom = 0; // Start in Room 0
    player->hp = 100;        // Default health points
    player->damage = 20;     // Default damage
    printf("\n+------------------------------------------+\n");
    printf("| Welcome, %s!                              |\n", player->name);
    printf("+------------------------------------------+\n");
    printf("| You awaken in the depths of Room 0.      |\n");
    printf("| Your quest: Find the legendary crown     |\n");
    printf("| and escape the dungeon alive.            |\n");
    printf("| Beware: Monsters lurk in the shadows.    |\n");
    printf("+------------------------------------------+\n");
    printf("| Your stats:                              |\n");
    printf("|   HP     = %d                            |\n", player->hp);
    printf("|   Damage = %d                            |\n", player->damage);
    printf("+------------------------------------------+\n");
}

// Move the player to a different room
void MovePlayer(struct Player *player, struct Dungeon *dungeon)
{
    struct Room *currentRoom = &dungeon->rooms[player->currentRoom];

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

void Combat(struct Player *player, struct Room *room)
{
    // Display monster
    printf("\n+----------------------------------+\n");
    if (room->monsterType == 1)
    {
        printf("| A low-level minion appears!      |\n");
    }
    else if (room->monsterType == 2)
    {
        printf("| A mid-level mini boss appears!   |\n");
    }
    else if (room->monsterType == 3)
    {
        printf("| A high-level boss appears!       |\n");
    }
    printf("| Combat begins!                   |\n");
    printf("+----------------------------------+\n");

    while (player->hp > 0 && room->monsterHp > 0)
    {

        int randomNumber = rand() % 17; // Random Number { 0 - 16}
        printf("\n+----------------------------------+\n");
        printf("| Random number for this round:%2d  |(binary: ", randomNumber);
        for (int i = 4; i >= 0; i--)
        {
            printf("%d", (randomNumber >> i) & 1);
        }
        printf(")\n");
        printf("+----------------------------------+\n");

        // processing the bits

        for (int i = 4; i >= 0; i--)
        {
            int bit = (randomNumber >> i) & 1;

            if (bit == 0)
            {
                // Monster attacks player
                player->hp -= room->monsterDamage;
                printf("| Monster attacks! You lose %2d HP. |\n", room->monsterDamage);
                printf("| Your HP: %3d                     |\n", player->hp);

                if (player->hp <= 0)
                {
                    printf("+----------------------------------+\n");
                    printf("| You have been defeated by the    |\n");
                    printf("| monster :/. Game over!              |\n");
                    printf("+----------------------------------+\n");
                    exit(0); // End the game
                }
            }
            else
            {
                // Player attacks monster
                room->monsterHp -= player->damage;
                printf("| You attack! Monster loses %2d HP. |\n", player->damage);
                printf("| Monster HP: %3d                  |\n", room->monsterHp);

                if (room->monsterHp <= 0)
                {
                    printf("+----------------------------------+\n");
                    printf("| You defeated the monster!        |\n");
                    printf("+----------------------------------+\n");
                    room->monster = 0; // Monster is defeated

                    // Display player's remaining HP after the battle
                    printf("+----------------------------------+\n");
                    printf("| Your remaining HP: %3d           |\n", player->hp);
                    printf("+----------------------------------+\n");

                    return; // Exit the combat
                }
            }
        }
    }
}