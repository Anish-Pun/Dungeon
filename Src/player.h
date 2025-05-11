#ifdef PLAYER_H
#define PLAYER_H

#include "dungeon.h"

struct Player {
    char name[50];  // Player's name
    int currentRoom; // Current room ID
    int hp;          // Player's health points
    int damage;      // Player's base damage
};

#endif // PLAYER_H