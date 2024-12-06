#ifndef MAP_H
#define MAP_H

#define NEW_MAP_SIZE  6
#define MAIN_MAP_SIZE 16

typedef struct {
    char **tiles;
    int width,height;
    int dungX,dungY;
    int id;
} Map;
void generateMap(Map *map,int width, int height, int id); 
void freeMap(Map *map);

#endif