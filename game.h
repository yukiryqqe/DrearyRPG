#ifndef GAME_H
#define GAME_H

#include "map/map.h"

struct Player; 

typedef struct {
    Map *maps;
    int mapCount;
    int currentMapId;
    struct Player *player;
} GameState;

void gameLoop(GameState *state); 

#endif