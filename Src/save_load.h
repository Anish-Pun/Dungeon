#ifndef SAVE_LOAD_H
#define SAVE_LOAD_H

#include "player.h"
#include "dungeon.h"

void SaveGame(struct Player* player, struct Dungeon* dungeon, const char* filename) ;
int LoadGame(struct Player* player, struct Dungeon* dungeon, const char* filename);
#endif // SAVE_LOAD_H


