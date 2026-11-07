#ifndef MAP_H
#define MAP_H

#include "utils.h"

char **create_map(int height, int width);
void fill_map(char **map, int height, int width);
void free_map(char **map, int height);
void print_map(char **map, int height, int width, int hero_x, int hero_y);
void place_hero(char **map, int height, int width, int *hero_x, int *hero_y);
int move_hero(char **map, int height, int width, int *hero_x, int *hero_y, char direction);
void place_stairs(char **map, int height, int width, char symbol);
void collapse_floor(char **map, int height, int width, int hero_x, int hero_y);
int is_stairs_up_present(char **map, int height, int width);
int is_stairs_down_present(char **map, int height, int width);
void adventure(void);

#endif