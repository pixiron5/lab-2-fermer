#ifndef PLAYER_H
#define PLAYER_H

#include "utils.h"

typedef struct Player {
    char name[MAX_NAME_LEN];
    int hp;
    int max_hp;
    int stamina;
    int max_stamina;
    int gold;
    int x, y;
    int inventory[INVENTORY_SIZE];
} Player;

Player *create_player(void);
void free_player(Player **player);
void show_player(Player *player, char names[][MAX_NAME_LEN], int day, int hour);
void show_hud(Player *player, int floor, int hour);

void show_inventory(int inv[], char names[][MAX_NAME_LEN]);
void put_item(int inv[]);
void drop_item(int inv[], char names[][MAX_NAME_LEN]);
void find_favorite(int inv[], char names[][MAX_NAME_LEN]);
void find_by_name(int inv[], char names[][MAX_NAME_LEN]);

#endif