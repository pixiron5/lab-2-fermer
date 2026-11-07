#ifndef MAP_H
#define MAP_H

#include "utils.h"

typedef struct {
    char **cells;
    int width;
    int height;
    int floor;
} Map;

Map *create_map(int height, int width);
void free_map(Map *map);
void fill_map(Map *map);
void print_map(Map *map, int hero_x, int hero_y);
void place_hero(Map *map, int *hero_x, int *hero_y);
int move_hero(Map *map, int *hero_x, int *hero_y, char direction);
void place_stairs(Map *map, char symbol);
void collapse_floor(Map *map, int hero_x, int hero_y);
int is_stairs_up_present(Map *map);
int is_stairs_down_present(Map *map);
void adventure(void);

#endif