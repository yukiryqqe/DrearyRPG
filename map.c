#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>

#define MAP_SIZE 25

typedef struct {
    int x, y;       // Player's position
} Player;

// Function to generate the map
void generateMap(char map[MAP_SIZE][MAP_SIZE]) {
    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
            map[i][j] = (rand() % 2) ? 'P' : 'F'; // Randomly assign 'P' or 'F'
        }
    }
}

// Function to render the map with a border and the player
void renderMap(char map[MAP_SIZE][MAP_SIZE], Player *player) {
    int termHeight, termWidth;
    getmaxyx(stdscr, termHeight, termWidth); // Get the terminal dimensions

    // Calculate starting position to center the map
    int startY = (termHeight - MAP_SIZE) / 2;
    int startX = (termWidth - (MAP_SIZE * 2)) / 2; // Each tile is spaced by 2 chars

    // Draw the border around the map
    for (int i = -1; i <= MAP_SIZE; i++) {
        for (int j = -1; j <= MAP_SIZE; j++) {
            if (i == -1 || i == MAP_SIZE) {
                // Top and bottom borders
                mvaddch(startY + i, startX + j * 2, (j == -1 || j == MAP_SIZE) ? '+' : '-');
            } else if (j == -1 || j == MAP_SIZE) {
                // Left and right borders
                mvaddch(startY + i, startX + j * 2, '|');
            } else {
                // Draw the map content
                char toPrint = map[i][j];
                if (i == player->x && j == player->y) {
                    toPrint = '@';
                }
                mvaddch(startY + i, startX + j * 2, toPrint);
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

// Function to get the current biome name
const char* getBiomeName(char tile) {
    switch (tile) {
        case 'P': return "Plains";
        case 'F': return "Forest";
        default:  return "Unknown";
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
    generateMap(map);
    player.x = MAP_SIZE / 2;  // Start the player in the middle
    player.y = MAP_SIZE / 2;
    previousTile = map[player.x][player.y]; // Save the initial tile under the player

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
        mvprintw(startY - 2, startX, "Current biome: %s", biomeName); // 2 rows above the map

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

