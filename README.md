# Dungeon
 
## Dungeon Generation

**Implemented:**
- Variable number of rooms: The number of rooms in the dungeon is determined by the `roomCount` variable.
- Graph structure with adjacency list: Each room uses an adjacency list (`connectedRooms`) to store connections to other rooms.
- 1-4 connections per room: Each room is connected to 1-4 other rooms.
- Heap allocation: The dungeon and its rooms are dynamically allocated using `malloc`.

**Not Implemented:**
- 4-dimensional doubly linked list (optional): The adjacency list could be replaced with a doubly linked list structure with `north`, `south`, `east`, and `west` pointers.

