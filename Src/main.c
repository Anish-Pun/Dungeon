#include <stdio.h>
#include "dungeon.h"
#include "player.h"
#include <stdlib.h>
#include "save_load.h"


void DebugDungeon(struct Dungeon *dungeon);

int main()
{
    int choice;
    struct Dungeon *dungeon = NULL;
    struct Player player;

    while (1)
    {
        printf("\n+------------------------------------------+\n");
        printf("| Options:                                 |\n");
        printf("| 1. New game                              |\n");
        printf("| 2. Load game                             |\n");
        printf("| 3. Exit                                  |\n");
        printf("+------------------------------------------+\n");
        printf("Choose an option: ");

        if (scanf("%d", &choice) != 1)
        {
            while (getchar() != '\n')
                ; // Clear invalid input
            printf("\nInvalid input. Please try again.\n");
            continue;
        }

        if (choice == 1)
        {
            printf("\nStarting a new game...\n");
            int roomCount = 10; // Example: Create a dungeon with 10 rooms
            dungeon = CreateDungeon(roomCount);
            // Initialize the player
            InitializePlayer(&player);
            break; // Proceed to the game loop
        }

        else if (choice == 2)
        {
            printf("\nLoading a saved game...\n");
            printf("\nGame loaded successfully!\n");
            printf("Nah jk this does not work gg\n");
            continue; // Go back to the menu
        }
        else if (choice == 3)
        {
            printf("\nExiting the game. Goodbye!\n");
            return 0; // Exit the program
        }
        else
        {
            printf("\nInvalid choice. Please try again.\n");
        }
    }

    // Game loop
    while (1)
    {
        // Display connected rooms
        struct Room *currentRoom = &dungeon->rooms[player.currentRoom];
        printf("\nIn front of you, you see %d room(s): ", currentRoom->ConnectedRoomsCount);

        // Display connected rooms for the current room
        printf("\nYou are in Room %d.\n", player.currentRoom);
        printf("Connected rooms: ");
        for (int i = 0; i < currentRoom->ConnectedRoomsCount; i++)
        {
            printf("%d ", currentRoom->connectedRooms[i]);
        }
        printf("\n");

        // Prompt for action
        printf("\nEnter your action | D for debug | S for save |, or choose a room): ");
        char input[10];
        scanf("%s", input);

        // Check if the user wants to debug
        if (input[0] == 'D' || input[0] == 'd')
        {
            DebugDungeon(dungeon);
            continue; // Return to the game loop
        }

        // Save game
        if (input[0] == 'S' || input[0] == 's')
        {
            SaveGame(&player, dungeon, "savegame.json");
            continue; // Return to the game loop
        }

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
            player.currentRoom = chosenRoom;
            struct Room *currentRoom = &dungeon->rooms[player.currentRoom];
            printf("\nYou have moved to Room %d.\n", player.currentRoom);

            // Check for the crown
            if (currentRoom->Crown)
            {
                printf("\nCongratulations, %s! You have found the crown in Room %d and won the game!\n", player.name, player.currentRoom);
                break;
            }

            // Check for a monster
            if (currentRoom->monster)
            {
                Combat(&player, currentRoom);
            }

            // Check for an item
            if (currentRoom->Items != 0)
            {
                if (currentRoom->Items == ITEM_HEALTH_POTION)
                {
                    printf("\nYou found a Health Potion! Your HP is restored by 30.\n");
                    player.hp += 30;
                    if (player.hp > 100)
                        player.hp = 100; // Cap HP at 100
                    printf("Your HP is now: %d\n", player.hp);
                }
                else if (currentRoom->Items == ITEM_DAMAGE_BOOST)
                {
                    printf("\nYou found a Damage Boost! Your damage is permanently increased by 10.\n");
                    player.damage += 10; // Permanently increase damage
                    printf("Your damage is now: %d\n", player.damage);
                }
                currentRoom->Items = 0; // Remove the item from the room
            }
        }
        else
        {
            printf("You cannot go to Room %d from here. Please choose a valid room.\n", chosenRoom);
        }
    }

    FreeDungeon(dungeon);
    return 0;
}

void DebugDungeon(struct Dungeon *dungeon)
{
    printf("\n+------------------------------------------+\n");
    printf("| Dungeon Structure                        |\n");
    printf("+------------------------------------------+\n");

    // Print the structure of each room and its connected rooms
    for (int i = 0; i < dungeon->roomCount; i++)
    {
        struct Room *room = &dungeon->rooms[i];
        printf("Room %d: ", room->id);
        for (int j = 0; j < room->ConnectedRoomsCount; j++)
        {
            printf("%d ", room->connectedRooms[j]);
        }
        printf("\n");
    }

    printf("\n+------------------------------------------+\n");
    printf("| Debug: Monster and Crown Locations       |\n");
    printf("+------------------------------------------+\n");

    // Print monster and crown locations
    for (int i = 0; i < dungeon->roomCount; i++)
    {
        struct Room *room = &dungeon->rooms[i];
        if (room->monster)
        {
            if (room->monsterType == 1)
            {
                printf("Room %d: Low-level Minion (HP %d, Damage %d)\n", room->id, room->monsterHp, room->monsterDamage);
            }
            else if (room->monsterType == 2)
            {
                printf("Room %d: Mini Boss (HP %d, Damage %d)\n", room->id, room->monsterHp, room->monsterDamage);
            }
            else if (room->monsterType == 3)
            {
                printf("Room %d: Boss (HP %d, Damage %d)\n", room->id, room->monsterHp, room->monsterDamage);
            }
        }
        if (room->Crown)
        {
            printf("Room %d: Crown\n", room->id);
        }
    }

    printf("\n+------------------------------------------+\n");
    printf("| Debug: Item Locations                    |\n");
    printf("+------------------------------------------+\n");

    // Print item locations
    for (int i = 0; i < dungeon->roomCount; i++)
    {
        struct Room *room = &dungeon->rooms[i];
        if (room->Items == ITEM_HEALTH_POTION)
        {
            printf("Room %d: Health Potion\n", room->id);
        }
        else if (room->Items == ITEM_DAMAGE_BOOST)
        {
            printf("Room %d: Damage Boost\n", room->id);
        }
    }

    printf("+------------------------------------------+\n");
}