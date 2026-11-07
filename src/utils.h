// src/utils.h
#ifndef UTILS_H
#define UTILS_H

#define INVENTORY_SIZE 10
#define MAX_ITEM_ID 9
#define MAX_NAME_LEN 32
#define MAX_ITEMS 10
#define MAX_LINE_LEN 256
#define MIN_MAP_SIZE 5
#define MAX_MAP_SIZE 30
#define WALL '#'
#define FLOOR '.'
#define TREE 'T'
#define STONE 'R'
#define HERO '@'
#define STAIRS_UP 'U'
#define STAIRS_DOWN 'D'
#define ABYSS '!'
#define COLLAPSE_INTERVAL 5

typedef enum {
    DIR_NONE = 0,
    DIR_UP,
    DIR_DOWN,
    DIR_LEFT,
    DIR_RIGHT,
    DIR_QUIT
} Direction;

void show_time(int day, int hour);
void skip_time(int *day, int *hour);
void read_string(char *buffer, int size);
void load_items(char names[][MAX_NAME_LEN]);
void replace_tag(const char *src, const char *tag, const char *value, char *dest);
void process_input(char farmer_name[], int current_day, int inv[], char item_names[][MAX_NAME_LEN]);
void write_diary(char farmer_name[], int day, int hour, int inv[], char names[][MAX_NAME_LEN]);

#endif