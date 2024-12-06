#include "map.h"
#include <wchar.h>
#include <stdlib.h>
//Generate map
void generateMap(Map *map,int width, int height, int id) {
   map->width=width;
    map->height=height;
    map->id=id;
    map->tiles=malloc(height*sizeof(char*));
    for(int i=0;i<height;i++){
        map->tiles[i]=malloc(width*sizeof(wchar_t*));
        for (int j=0;j<width;j++) {
            map->tiles[i][j]=(rand()%2)?'P':'F';
        }
    }
    map->dungX=rand()%height;
    map->dungY=rand()%width;
    map->tiles[map->dungX][map->dungY]='D';
}
//Free map's malloc
void freeMap(Map *map){
    for(int i=0;i<map->height;i++){
        free(map->tiles[i]);
    }
    free(map->tiles);
}