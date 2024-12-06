#include <complex.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncursesw/curses.h>
#include <locale.h>
#include <wchar.h>
#include "map/map.h"
#include "player/player.h"
#include "game.h"
#include "UI/ui.h"

const char* getBiomeName(char tile) {
    switch (tile) {
        case 'P': return "Plains";
        case 'F': return "Forest";
        case 'D': return "Dungeon";
        default: return "Unknown";
    }
}

wchar_t getBiomeSymbol(char biome){
    switch (biome) {
        case 'P': return L'˷';
        case 'F': return L'^';
        case 'D': return L'Δ';
        default: return L' ';
    }
}

const char* generateDate() {
    int seasonNum=rand()%9+1;
    switch(seasonNum){
        case 1: return "Kexahm";
        case 2: return "Kreasis";
        case 3: return "Butuhm";
        case 4: return "Trikarf";
        case 5: return "Waaph";
        case 6: return "Browis";
        case 7: return "Naurq";
        case 8: return "Glywill";
        case 9: return "Zadilm";
        default: return "Error";
    }
}

void gameLoop(GameState *state) {
    while(1) {
        clear();
        renderMap(state);

          // Get user input
        int input = getch();
        if (input == 'q') break;

        // Handle movement
        switch (input) {
            case 'w': movePlayer(state,-1,  0); break; // Up
            case 's': movePlayer(state, 1,  0); break; // Down
            case 'a': movePlayer(state, 0, -1); break; // Left
            case 'd': movePlayer(state, 0,  1); break; // Right
        }
    }
}

int main() {
    srand(time(NULL)); // Seed random number generator

    // Initialize ncurses
    setlocale(LC_ALL,"");
    initscr();
    noecho();           // Disable automatic echoing of typed characters
    cbreak();           // Disable line buffering
    curs_set(0);        // Hide the cursor
    keypad(stdscr, TRUE); // Enable arrow keys

    // Generate the map and place the player
    GameState state;
    state.mapCount=2;
    state.maps=malloc(state.mapCount*sizeof(Map));
    generateMap(&state.maps[0],20,10,0);
    generateMap(&state.maps[1],5,5,1);
    
    state.currentMapId=0;
    state.player->x=state.maps[0].height/2;
    state.player->y=state.maps[0].width/2;

    gameLoop(&state);
    //cleanup
    for(int i=0;i<state.mapCount;i++){
        freeMap(&state.maps[i]);
    }
    free(state.maps);
    // End ncurses mode
    endwin();

    return 0;
}