#ifndef PLAYER_H
#define PLAYER_H
#pragma once
#include "../game.h"

typedef struct Player {
    int x, y;       // Player's position
} Player;

void movePlayer(GameState *state, int dx, int dy);

#endif