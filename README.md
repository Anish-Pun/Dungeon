# Dungeon
 
## Dungeon Generation

**Implemented:**
- Variable number of rooms: The number of rooms in the dungeon is determined by the `roomCount` variable.
- Graph structure with adjacency list: Each room uses an adjacency list (`connectedRooms`) to store connections to other rooms.
- 1-4 connections per room: Each room is connected to 1-4 other rooms.
- Heap allocation: The dungeon and its rooms are dynamically allocated using `malloc`.

**Not Implemented:**
- 4-dimensional doubly linked list (optional): The adjacency list could be replaced with a doubly linked list structure with `north`, `south`, `east`, and `west` pointers.

## Player Mechanics

**Implemented:**
- Player location tracking: The player's current room is tracked using the `currentRoom` field.
- Health points (HP) and damage: The player has `hp` and `damage` 

**Not Implemented:**
- Combat system.
- Item usage: Items are implemented to restore HP and boost player stats.

## Room Content

**Implemented:**
- Doors to other rooms: Each room connects to 1-4 other rooms.
- Unique room IDs: Every room has a distinct `id`.

**Not Implemented:**
- Monsters: Rooms can contain monsters, tracked by the `hasMonster` field. Monsters have different types with varying `hp` and `damage` values.
- Treasure (Crown): A crown is placed randomly; finding it ends the game.
- Item types: Two types of items with different effects are available.
