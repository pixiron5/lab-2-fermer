#include <stdio.h>
#include <stdlib.h>
#include "map.h"

Direction char_to_direction(char c) {
    if (c == 'W' || c == 'w') return DIR_UP;
    if (c == 'S' || c == 's') return DIR_DOWN;
    if (c == 'A' || c == 'a') return DIR_LEFT;
    if (c == 'D' || c == 'd') return DIR_RIGHT;
    if (c == 'Q' || c == 'q') return DIR_QUIT;
    return DIR_NONE;
}

Map *create_map(int height, int width) {
    Map *map = malloc(sizeof(Map));
    if (map == NULL) {
        printf("Ошибка: не удалось выделить память!\n");
        return NULL;
    }
    map->height = height;
    map->width = width;
    map->floor = 1;

    map->cells = malloc(height * sizeof(char*));
    if (map->cells == NULL) {
        free(map);
        printf("Ошибка: не удалось выделить память!\n");
        return NULL;
    }
    for (int i = 0; i < height; i++) {
        map->cells[i] = malloc(width * sizeof(char));
        if (map->cells[i] == NULL) {
            for (int j = 0; j < i; j++) free(map->cells[j]);
            free(map->cells);
            free(map);
            printf("Ошибка: не удалось выделить память!\n");
            return NULL;
        }
    }
    return map;
}

void free_map(Map *map) {
    if (map == NULL) return;
    for (int i = 0; i < map->height; i++) {
        free(map->cells[i]);
    }
    free(map->cells);
    free(map);
}

void fill_map(Map *map) {
    for (int i = 0; i < map->height; i++) {
        for (int j = 0; j < map->width; j++) {
            if (i == 0 || i == map->height - 1 || j == 0 || j == map->width - 1) {
                map->cells[i][j] = WALL;
            } else {
                int r = rand() % 100;
                if (r < 15) map->cells[i][j] = TREE;
                else if (r < 30) map->cells[i][j] = STONE;
                else map->cells[i][j] = FLOOR;
            }
        }
    }
}

void print_map(Map *map, int hero_x, int hero_y) {
    for (int i = 0; i < map->height; i++) {
        for (int j = 0; j < map->width; j++) {
            if (i == hero_y && j == hero_x) printf("%c ", HERO);
            else printf("%c ", map->cells[i][j]);
        }
        printf("\n");
    }
}

void place_hero(Map *map, int *hero_x, int *hero_y) {
    while (1) {
        int x = 1 + rand() % (map->width - 2);
        int y = 1 + rand() % (map->height - 2);
        if (map->cells[y][x] == FLOOR) {
            *hero_x = x;
            *hero_y = y;
            return;
        }
    }
}

int move_hero(Map *map, int *hero_x, int *hero_y, char direction) {
    int new_x = *hero_x, new_y = *hero_y;
    if (direction == 'W' || direction == 'w') new_y--;
    else if (direction == 'S' || direction == 's') new_y++;
    else if (direction == 'A' || direction == 'a') new_x--;
    else if (direction == 'D' || direction == 'd') new_x++;

    if (new_x < 0 || new_x >= map->width || new_y < 0 || new_y >= map->height) return 0;

    char target = map->cells[new_y][new_x];
    if (target != FLOOR && target != STAIRS_DOWN) return 0;

    *hero_x = new_x;
    *hero_y = new_y;
    return 1;
}

void place_stairs(Map *map, char symbol) {
    int attempts = 0;
    while (attempts < 1000) {
        int x = 2 + rand() % (map->width - 4);
        int y = 2 + rand() % (map->height - 4);
        if (map->cells[y][x] == FLOOR) {
            map->cells[y][x] = symbol;
            return;
        }
        attempts++;
    }
    for (int i = 1; i < map->height - 1; i++) {
        for (int j = 1; j < map->width - 1; j++) {
            if (map->cells[i][j] == FLOOR) {
                map->cells[i][j] = symbol;
                return;
            }
        }
    }
}

void collapse_floor(Map *map, int hero_x, int hero_y) {
    Map *copy = create_map(map->height, map->width);
    if (copy == NULL) return;

    for (int i = 0; i < map->height; i++)
        for (int j = 0; j < map->width; j++)
            copy->cells[i][j] = map->cells[i][j];

    for (int i = 1; i < map->height - 1; i++) {
        for (int j = 1; j < map->width - 1; j++) {
            if (copy->cells[i][j] == WALL || copy->cells[i][j] == ABYSS) continue;

            int edge = 0;
            if (copy->cells[i-1][j] == WALL || copy->cells[i-1][j] == ABYSS) edge = 1;
            if (copy->cells[i+1][j] == WALL || copy->cells[i+1][j] == ABYSS) edge = 1;
            if (copy->cells[i][j-1] == WALL || copy->cells[i][j-1] == ABYSS) edge = 1;
            if (copy->cells[i][j+1] == WALL || copy->cells[i][j+1] == ABYSS) edge = 1;

            if (edge && (i != hero_y || j != hero_x)) {
                map->cells[i][j] = ABYSS;
            }
        }
    }
    free_map(copy);
}

int is_stairs_up_present(Map *map) {
    for (int i = 0; i < map->height; i++)
        for (int j = 0; j < map->width; j++)
            if (map->cells[i][j] == STAIRS_UP) return 1;
    return 0;
}

int is_stairs_down_present(Map *map) {
    for (int i = 0; i < map->height; i++)
        for (int j = 0; j < map->width; j++)
            if (map->cells[i][j] == STAIRS_DOWN) return 1;
    return 0;
}

void adventure(Player *player, int *day, int *hour) {
    if (player == NULL) {
        printf("Сначала создайте персонажа!\n");
        return;
    }
    if (player->stamina <= 0) {
        printf("У вас нет выносливости! Отдохните.\n");
        return;
    }
    if (*hour >= 2 && *hour < 8) {
        printf("Сейчас ночь (02:00–08:00). Фермер должен спать.\n");
        return;
    }

    int floor = 1;
    while (1) {
        int height = MIN_MAP_SIZE + rand() % (MAX_MAP_SIZE - MIN_MAP_SIZE + 1);
        int width = MIN_MAP_SIZE + rand() % (MAX_MAP_SIZE - MIN_MAP_SIZE + 1);
        int steps = 0;

        Map *map = create_map(height, width);
        if (map == NULL) return;
        map->floor = floor;
        fill_map(map);

        int hero_x, hero_y;
        place_hero(map, &hero_x, &hero_y);
        map->cells[hero_y][hero_x] = STAIRS_DOWN;
        place_stairs(map, STAIRS_UP);

        int new_floor = 0;
        char direction_char;

        while (1) {
            printf("\n--- Этаж %d ---\n", map->floor);
            show_hud(player, map->floor, *hour);
            print_map(map, hero_x, hero_y);
            printf("W/A/S/D — движение, Q — выход: ");
            scanf(" %c", &direction_char);

            Direction dir = char_to_direction(direction_char);

            if (dir == DIR_QUIT) {
                if (map->cells[hero_y][hero_x] == STAIRS_DOWN ||
                    !is_stairs_up_present(map) ||
                    !is_stairs_down_present(map)) {
                    free_map(map);
                    printf("Вы вышли из башни. Золото сохранено.\n");
                    return;
                } else {
                    printf("Выход работает только на лестнице вниз (D)!\n");
                }
                continue;
            }

            if (dir == DIR_NONE) {
                printf("Неизвестная команда!\n");
                while (getchar() != '\n');
                continue;
            }

            int nx = hero_x, ny = hero_y;
            switch (dir) {
                case DIR_UP:    ny--; break;
                case DIR_DOWN:  ny++; break;
                case DIR_LEFT:  nx--; break;
                case DIR_RIGHT: nx++; break;
                default: break;
            }

            if (nx < 0 || nx >= map->width || ny < 0 || ny >= map->height) continue;

            if (map->cells[ny][nx] == STAIRS_UP) {
                player->stamina -= 5;
                (*hour)++;
                if (*hour >= 24) { *hour = 0; (*day)++; }

                if (player->stamina <= 0) {
                    printf("Вы обессилели! Спасательная служба доставляет вас домой.\n");
                    player->gold /= 2;
                    printf("За услуги списана половина золота. Осталось: %d\n", player->gold);
                    free_map(map);
                    return;
                }
                if (*hour >= 2 && *hour < 8) {
                    printf("Наступила ночь! Вы засыпаете прямо в башне.\n");
                    player->gold /= 2;
                    printf("Вас доставили домой. Осталось золота: %d\n", player->gold);
                    free_map(map);
                    return;
                }

                int reward = GOLD_PER_FLOOR * floor;
                player->gold += reward;
                printf("Вы поднимаетесь на этаж %d! Получено %d золота.\n", floor + 1, reward);
                new_floor = 1;
                break;
            }

            if (move_hero(map, &hero_x, &hero_y, direction_char)) {
                player->stamina--;
                if (player->stamina <= 0) {
                    printf("Вы обессилели! Спасательная служба доставляет вас домой.\n");
                    player->gold /= 2;
                    printf("За услуги списана половина золота. Осталось: %d\n", player->gold);
                    free_map(map);
                    return;
                }

                steps++;
                if (steps % COLLAPSE_INTERVAL == 0) {
                    printf("Пол начинает рушиться!\n");
                    collapse_floor(map, hero_x, hero_y);
                    if (map->cells[hero_y][hero_x] == ABYSS) {
                        printf("Вы провалились в бездну! Игра окончена.\n");
                        free_map(map);
                        return;
                    }
                    if (!is_stairs_up_present(map)) {
                        printf("Лестница вверх провалилась! Нужно срочно эвакуироваться.\n");
                    }
                }
            }
        }
        free_map(map);
        if (!new_floor) return;
        floor++;
    }
}