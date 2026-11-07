#ifndef PLAYER_H
#define PLAYER_H

#include "utils.h"

void show_inventory(int inv[], char names[][MAX_NAME_LEN]);
void put_item(int inv[]);
void drop_item(int inv[], char names[][MAX_NAME_LEN]);
void find_favorite(int inv[], char names[][MAX_NAME_LEN]);
void find_by_name(int inv[], char names[][MAX_NAME_LEN]);

#endif