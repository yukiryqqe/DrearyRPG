#include <complex.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>

#define MAP_SIZE 25
#define NEW_MAP_SIZE 10
typedef struct {
    int x, y;       // Player's position
} Player;

// Function to generate the map
void generateMap(char map[MAP_SIZE][MAP_SIZE], int mapSize) {
    for (int i = 0; i < mapSize; i++) {
        for (int j = 0; j < mapSize; j++) {
            map[i][j] = (rand() % 2) ? 'P' : 'F'; // Randomly assign 'P' or 'F'
        }
    }
}

void placeGate(char map[MAP_SIZE][MAP_SIZE]){
    int gateX=rand()%MAP_SIZE;
    int gateY=rand()%MAP_SIZE;
    map[gateX][gateY]='D';
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

// Function to render the map with the player
void renderMap(char map[MAP_SIZE][MAP_SIZE], Player *player) {
    int termHeight, termWidth;
    getmaxyx(stdscr, termHeight, termWidth); // Get the terminal dimensions

    // Calculate starting position to center the map
    int startY = (termHeight - MAP_SIZE) / 2;
    int startX = (termWidth - (MAP_SIZE * 2)) / 2; // Each tile is spaced by 2 chars

    for (int i = -1; i <= MAP_SIZE; i++) {
        for (int j = -1; j <= MAP_SIZE; j++) {
            if(i==-1||i==MAP_SIZE){
                mvaddch(startY+i, startX+j*2, (j==-1||j==MAP_SIZE)?'+':'-');
            } else if(j==-1||j==MAP_SIZE){
                mvaddch(startY+i, startX+j*2, '|');
            } else {
                char toPrint=map[i][j];
                if(i==player->x&&j==player->y){
                    toPrint='@';
                }
                mvaddch(startY+i, startX+j*2, toPrint);
            }
        }
    }
    refresh();
}

// Function to handle player movement
void movePlayer(char map[MAP_SIZE][MAP_SIZE], Player *player, char *previousTile, int dx, int dy) {
    int newX = player->x + dx;
    int newY = player->y + dy;

    // Check boundaries
    if (newX >= 0 && newX < MAP_SIZE && newY >= 0 && newY < MAP_SIZE) {
        // Restore the biome of the previous tile
        map[player->x][player->y] = *previousTile;

        // Save the biome of the new tile
        *previousTile = map[newX][newY];

        // Update player position
        player->x = newX;
        player->y = newY;
    }
}

const char* getBiomeName(char tile) {
    switch (tile) {
        case 'P': return "Plains";
        case 'F': return "Forest";
        case 'D': return "Dungeon";
        default: return "Unknown";
    }
}

int isAtGate(char map[MAP_SIZE][MAP_SIZE],Player *player){
    return map[player->x][player->y]=='A';
}

void teleportToNewMap(char map[MAP_SIZE][MAP_SIZE], Player *player) {
    // Initialize the new 5x5 map
    char newMap[NEW_MAP_SIZE][NEW_MAP_SIZE];
    generateMap(newMap, NEW_MAP_SIZE);  // Generate a new map
    placeGate(newMap);  // Place the gate on the new map

    // Adjust player's position to the new map (centered)
    player->x = NEW_MAP_SIZE / 2;
    player->y = NEW_MAP_SIZE / 2;

    // Render the new map
    int termHeight, termWidth;
    getmaxyx(stdscr, termHeight, termWidth); // Get terminal dimensions

    int startY = (termHeight - NEW_MAP_SIZE) / 2;
    int startX = (termWidth - (NEW_MAP_SIZE * 2)) / 2;

    clear();
    renderMap(newMap, player);

    // Display biome near the top-left corner of the new map
    const char* biomeName = getBiomeName(newMap[player->x][player->y]);
    mvprintw(startY - 2, startX, "Current biome: %s", biomeName); // 2 rows above the map
    mvprintw(LINES - 2, 0, "Use W/A/S/D to move, Q to quit, E to go back to the original map.");
    refresh();

    while (1) {
        int input = getch();
        if (input == 'q') break;

        // Move player on the new map
        switch (input) {
            case 'w': movePlayer(newMap, player, &newMap[player->x][player->y], -1, 0); break;
            case 's': movePlayer(newMap, player, &newMap[player->x][player->y], 1, 0); break;
            case 'a': movePlayer(newMap, player, &newMap[player->x][player->y], 0, -1); break;
            case 'd': movePlayer(newMap, player, &newMap[player->x][player->y], 0, 1); break;
            case 'e':
                if (isAtGate(newMap, player)) {
                    player->x = MAP_SIZE / 2;  // Move player back to the original map center
                    player->y = MAP_SIZE / 2;
                    return;  // Exit the new map and return to the main map
                }
                break;
        }

        clear();
        renderMap(newMap, player);
        biomeName = getBiomeName(newMap[player->x][player->y]);
        mvprintw(startY - 2, startX, "Current biome: %s", biomeName);
        mvprintw(LINES - 2, 0, "Use W/A/S/D to move, Q to quit, E to go back to the original map.");
        refresh();
    }
}


int main() {
    char map[MAP_SIZE][MAP_SIZE];
    Player player;
    char previousTile;  // Tracks the biome of the last tile the player stepped on

    srand(time(NULL)); // Seed random number generator

    // Initialize ncurses
    initscr();
    noecho();           // Disable automatic echoing of typed characters
    cbreak();           // Disable line buffering
    curs_set(0);        // Hide the cursor
    keypad(stdscr, TRUE); // Enable arrow keys

    // Generate the map and place the player
    generateMap(map,MAP_SIZE);
    placeGate(map);
    player.x = MAP_SIZE / 2;  // Start the player in the middle
    player.y = MAP_SIZE / 2;
    previousTile = map[player.x][player.y]; // Save the initial tile under the player
    const char* currentMonths=generateDate();
    int currentDay=rand() % 30+1;
    while (1) {
        clear();

        // Get terminal dimensions and map position
        int termHeight, termWidth;
        getmaxyx(stdscr, termHeight, termWidth);
        int startY = (termHeight - MAP_SIZE) / 2;
        int startX = (termWidth - (MAP_SIZE * 2)) / 2;

        renderMap(map, &player);

        // Display biome near the top-left corner of the map
        const char* biomeName = getBiomeName(map[player.x][player.y]);
        mvprintw(startY, startX-25, "Current biome: %s", biomeName); // 2 rows above the map
        mvprintw(startY+2, startX-25,"Date: %s, %d",currentMonths,currentDay); 
        // Display instructions at the bottom of the screen
        mvprintw(LINES - 2, 0, "Use W/A/S/D to move, Q to quit.");
        refresh();

        // Get user input
        int input = getch();
        if (input == 'q') break;

        // Handle movement
        switch (input) {
            case 'w': movePlayer(map, &player, &previousTile, -1,  0); break; // Up
            case 's': movePlayer(map, &player, &previousTile,  1,  0); break; // Down
            case 'a': movePlayer(map, &player, &previousTile,  0, -1); break; // Left
            case 'd': movePlayer(map, &player, &previousTile,  0,  1); break; // Right
        }
    }

    // End ncurses mode
    endwin();

    return 0;
}