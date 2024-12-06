#include "../map/map.h"
#include "player.h"
#include "../game.h"

void movePlayer(GameState *state, int dx, int dy) {
    Map *currentMap=&state->maps[state->currentMapId];

    int newX = state->player->x + dx;
    int newY = state->player->y + dy;

    // Check boundaries
    if (newX >= 0 && newX < currentMap->height  && newY >= 0 && newY < currentMap->width) {
        // Update player position
        state->player->x = newX;
        state->player->y = newY;
        if(currentMap->tiles[state->player->x][state->player->y]=='D') {
            if(currentMap->id==0){
                state->currentMapId=1;
                state->player->x=state->maps[1].height/2;
                state->player->y=state->maps[1].width/2;
            } else {
                state->currentMapId=0;
                state->player->x=state->maps[0].height/2;
                state->player->y=state->maps[0].width/2;
            }
        }
    }
}