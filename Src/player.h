#ifndef PLAYER_H
#define PLAYER_H

#include "dungeon.h"

// Structure to represent the player
struct Player {
    char name[50];  // Player's name
    int currentRoom; // Current room ID
    int hp;          // Player's health points
    int damage;      // Player's base damage
};



// Function prototypes
void InitializePlayer(struct Player* player);
void MovePlayer(struct Player* player, struct Dungeon* dungeon);

#endif // PLAYER_H