#include "ui.h"
#include "map/map.h"
#include "../game.h"
#include <wchar.h>
#include <ncursesw/curses.h>
#include <../player/player.h>

void renderMap(GameState *state) {
    int termHeight, termWidth;
    getmaxyx(stdscr, termHeight, termWidth); // Get the terminal dimensions
    Map *currentMap=&state->maps[state->currentMapId];

    // Calculate starting position to center the map
    int startY = (termHeight - currentMap->height) / 2;
    int startX = (termWidth - (currentMap->width * 2)) / 2; // Each tile is spaced by 2 chars
    drawBorder(currentMap->width,currentMap->height,startX,startY);

    for (int i = 0; i < currentMap->height; i++) {
        for (int j = 0; j < currentMap->width; j++) {
                wchar_t toPrint=getBiomeSymbol(currentMap->tiles[i][j]);
                if(i==state->player->x&&j==state->player->y){ 
                    mvaddnwstr(startY+i+1,startX+1+j*2, L"@",1);
                } else {
                    mvaddnwstr(startY+1+i,startX+1+j*2,&toPrint,1);
                }
        }
    }
    const char* currentMonths=generateDate();
    int currentDay=rand() % 30+1;
     // Display biome near the top-left corner of the map
    const char* biomeName = getBiomeName(currentMap->tiles[state->player->x][state->player->y]);
    mvprintw(startY, startX-25, "Current biome: %s", biomeName); // 2 rows above the map
    mvprintw(startY+2, startX-25,"Date: %s, %d",currentMonths,currentDay); 
    // Display instructions at the bottom of the screen
    mvprintw(LINES - 2, 0, "Use W/A/S/D to move, Q to quit.");
    refresh();
}

void drawBorder(int width,int height, int startX, int startY){
    mvaddch(startY,startX,'+');
    mvaddch(startY,startX+2*width+1,'+');
    mvaddch(startY+height+1,startX,'+');
    mvaddch(startY+height+1,startX+2*width+1,'+');

    for(int i=1;i<=2*width;i++){
        mvaddch(startY,startX+i,'-');
        mvaddch(startY+height+1,startX+i,'-');
    }
    for(int i=1;i<=height;i++){
        mvaddch(startY+i,startX,'|');
        mvaddch(startY+i,startX+2*width+1,'|');
    }

}
