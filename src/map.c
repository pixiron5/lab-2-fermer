#include <stdio.h>
#include <stdlib.h>
#include "map.h"

char **create_map(int height, int width) {
    char **map = malloc(height * sizeof(char*));
    if (map == NULL) {
        printf("Ошибка: не удалось выделить память!\n");
        return NULL;
    }
    for (int i = 0; i < height; i++) {
        map[i] = malloc(width * sizeof(char));
        if (map[i] == NULL) {
            for (int j = 0; j < i; j++) free(map[j]);
            free(map);
            printf("Ошибка: не удалось выделить память!\n");
            return NULL;
        }
    }
    return map;
}

void fill_map(char **map, int height, int width) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (i == 0 || i == height - 1 || j == 0 || j == width - 1) {
                map[i][j] = WALL;
            } else {
                int r = rand() % 100;
                if (r < 15) map[i][j] = TREE;
                else if (r < 30) map[i][j] = STONE;
                else map[i][j] = FLOOR;
            }
        }
    }
}

void free_map(char **map, int height) {
    if (map == NULL) return;
    for (int i = 0; i < height; i++) free(map[i]);
    free(map);
}

void print_map(char **map, int height, int width, int hero_x, int hero_y) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (i == hero_y && j == hero_x) printf("%c ", HERO);
            else printf("%c ", map[i][j]);
        }
        printf("\n");
    }
}

void place_hero(char **map, int height, int width, int *hero_x, int *hero_y) {
    while (1) {
        int x = 1 + rand() % (width - 2);
        int y = 1 + rand() % (height - 2);
        if (map[y][x] == FLOOR) {
            *hero_x = x;
            *hero_y = y;
            return;
        }
    }
}

int move_hero(char **map, int height, int width, int *hero_x, int *hero_y, char direction) {
    int new_x = *hero_x, new_y = *hero_y;
    if (direction == 'W' || direction == 'w') new_y--;
    else if (direction == 'S' || direction == 's') new_y++;
    else if (direction == 'A' || direction == 'a') new_x--;
    else if (direction == 'D' || direction == 'd') new_x++;

    if (new_x < 0 || new_x >= width || new_y < 0 || new_y >= height) return 0;

    char target = map[new_y][new_x];
    if (target != FLOOR && target != STAIRS_DOWN) return 0;

    *hero_x = new_x;
    *hero_y = new_y;
    return 1;
}

void place_stairs(char **map, int height, int width, char symbol) {
    int attempts = 0;
    while (attempts < 1000) {
        int x = 2 + rand() % (width - 4);
        int y = 2 + rand() % (height - 4);
        if (map[y][x] == FLOOR) {
            map[y][x] = symbol;
            return;
        }
        attempts++;
    }
    for (int i = 1; i < height - 1; i++) {
        for (int j = 1; j < width - 1; j++) {
            if (map[i][j] == FLOOR) {
                map[i][j] = symbol;
                return;
            }
        }
    }
}

void collapse_floor(char **map, int height, int width, int hero_x, int hero_y) {
    char **copy = create_map(height, width);
    if (copy == NULL) return;

    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            copy[i][j] = map[i][j];

    for (int i = 1; i < height - 1; i++) {
        for (int j = 1; j < width - 1; j++) {
            if (copy[i][j] == WALL || copy[i][j] == ABYSS) continue;

            int edge = 0;
            if (copy[i-1][j] == WALL || copy[i-1][j] == ABYSS) edge = 1;
            if (copy[i+1][j] == WALL || copy[i+1][j] == ABYSS) edge = 1;
            if (copy[i][j-1] == WALL || copy[i][j-1] == ABYSS) edge = 1;
            if (copy[i][j+1] == WALL || copy[i][j+1] == ABYSS) edge = 1;

            if (edge && (i != hero_y || j != hero_x)) {
                map[i][j] = ABYSS;
            }
        }
    }
    free_map(copy, height);
}

int is_stairs_up_present(char **map, int height, int width) {
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            if (map[i][j] == STAIRS_UP) return 1;
    return 0;
}

int is_stairs_down_present(char **map, int height, int width) {
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            if (map[i][j] == STAIRS_DOWN) return 1;
    return 0;
}

void adventure(void) {
    int floor = 1;
    while (1) {
        int height = MIN_MAP_SIZE + rand() % (MAX_MAP_SIZE - MIN_MAP_SIZE + 1);
        int width = MIN_MAP_SIZE + rand() % (MAX_MAP_SIZE - MIN_MAP_SIZE + 1);
        int steps = 0;

        char **map = create_map(height, width);
        if (map == NULL) return;
        fill_map(map, height, width);

        int hero_x, hero_y;
        place_hero(map, height, width, &hero_x, &hero_y);
        map[hero_y][hero_x] = STAIRS_DOWN;
        place_stairs(map, height, width, STAIRS_UP);

        int new_floor = 0;
        char direction;

        while (1) {
            printf("\n--- Этаж %d ---\n", floor);
            print_map(map, height, width, hero_x, hero_y);
            printf("W/A/S/D — движение, Q — выход: ");
            scanf(" %c", &direction);

            if (direction == 'Q' || direction == 'q') {
                if (map[hero_y][hero_x] == STAIRS_DOWN ||
                    !is_stairs_up_present(map, height, width) ||
                    !is_stairs_down_present(map, height, width)) {
                    free_map(map, height);
                    printf("Вы вышли из башни.\n");
                    return;
                } else {
                    printf("Выход работает только на лестнице вниз (D)!\n");
                }
                continue;
            }

            int nx = hero_x, ny = hero_y;
            if (direction == 'W' || direction == 'w') ny--;
            else if (direction == 'S' || direction == 's') ny++;
            else if (direction == 'A' || direction == 'a') nx--;
            else if (direction == 'D' || direction == 'd') nx++;
            else {
                printf("Неизвестная команда!\n");
                while (getchar() != '\n');
                continue;
            }

            if (nx < 0 || nx >= width || ny < 0 || ny >= height) continue;

            if (map[ny][nx] == STAIRS_UP) {
                printf("Вы поднимаетесь на этаж %d!\n", floor + 1);
                new_floor = 1;
                break;
            }

            if (move_hero(map, height, width, &hero_x, &hero_y, direction)) {
                steps++;
                if (steps % COLLAPSE_INTERVAL == 0) {
                    printf("Пол начинает рушиться!\n");
                    collapse_floor(map, height, width, hero_x, hero_y);
                    if (map[hero_y][hero_x] == ABYSS) {
                        printf("Вы провалились в бездну! Игра окончена.\n");
                        free_map(map, height);
                        return;
                    }
                    if (!is_stairs_up_present(map, height, width)) {
                        printf("Лестница вверх провалилась! Нужно срочно эвакуироваться.\n");
                    }
                }
            }
        }
        free_map(map, height);
        if (!new_floor) return;
        floor++;
    }
}