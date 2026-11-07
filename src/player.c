#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "player.h"

Player *create_player(void) {
    Player *p = malloc(sizeof(Player));
    if (p == NULL) {
        printf("Ошибка: не удалось выделить память!\n");
        return NULL;
    }
    printf("Введите имя фермера: ");
    read_string(p->name, MAX_NAME_LEN);
    p->max_hp = 100;
    p->hp = 100;
    p->max_stamina = 50;
    p->stamina = 50;
    p->gold = 0;
    p->x = 0;
    p->y = 0;
    for (int i = 0; i < INVENTORY_SIZE; i++) p->inventory[i] = 0;
    p->inventory[0] = 4;
    p->inventory[1] = 6;
    p->inventory[3] = 2;
    p->inventory[5] = 3;
    p->inventory[6] = 8;
    p->inventory[8] = 1;
    printf("Добро пожаловать, %s!\n", p->name);
    return p;
}

void free_player(Player **player) {
    if (*player == NULL) return;
    printf("Память структуры по адресу %p освобождена\n", (void*)*player);
    free(*player);
    *player = NULL;
}

void show_player(Player *player, char names[][MAX_NAME_LEN], int day, int hour) {
    if (player == NULL) {
        printf("Персонаж не создан!\n");
        return;
    }
    printf("\n=== Карточка персонажа ===\n");
    printf("Имя: %s\n", player->name);
    printf("HP: %d/%d\n", player->hp, player->max_hp);
    printf("Выносливость: %d/%d\n", player->stamina, player->max_stamina);
    printf("Золото: %d\n", player->gold);
    printf("Время: День %d, %02d:00\n", day, hour);
    printf("Инвентарь:\n");
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        printf("  Слот %d: [%d] - (%s)\n", i, player->inventory[i], names[player->inventory[i]]);
    }
}

void show_hud(Player *player, int floor, int hour) {
    printf("HP: %d/%d | Выносливость: %d/%d | Золото: %d | Этаж: %d | Время: %02d:00\n",
           player->hp, player->max_hp,
           player->stamina, player->max_stamina,
           player->gold, floor, hour);
}

void show_inventory(int inv[], char names[][MAX_NAME_LEN]) {
    for (int i = 0; i < INVENTORY_SIZE; i++)
        printf("Слот %d: [%d] - (%s)\n", i, inv[i], names[inv[i]]);
}

void put_item(int inv[]) {
    int index, id;
    printf("Введите индекс слота (0-%d): ", INVENTORY_SIZE - 1);
    if (scanf("%d", &index) != 1) {
        printf("Ошибка: нужно ввести число!\n");
        while (getchar() != '\n');
        return;
    }
    if (index < 0 || index >= INVENTORY_SIZE) {
        printf("Ошибка: индекс вне границ массива (0-%d)!\n", INVENTORY_SIZE - 1);
        return;
    }
    printf("Введите ID предмета (0-%d): ", MAX_ITEM_ID);
    if (scanf("%d", &id) != 1) {
        printf("Ошибка: нужно ввести число!\n");
        while (getchar() != '\n');
        return;
    }
    if (id < 0 || id > MAX_ITEM_ID) {
        printf("Ошибка: такого ID не существует (0-%d)!\n", MAX_ITEM_ID);
        return;
    }
    inv[index] = id;
    printf("Предмет [%d] положен в слот %d.\n", id, index);
}

void drop_item(int inv[], char names[][MAX_NAME_LEN]) {
    int index;
    printf("Введите индекс слота, из которого вы хотите выбросить предмет (0-%d): ", INVENTORY_SIZE - 1);
    if (scanf("%d", &index) != 1) {
        printf("Ошибка: нужно ввести число!\n");
        while (getchar() != '\n');
        return;
    }
    if (index < 0 || index >= INVENTORY_SIZE) {
        printf("Ошибка: индекс вне границ массива (0-%d)!\n", INVENTORY_SIZE - 1);
        return;
    }
    printf("Выброшен предмет: %d (%s)\n", inv[index], names[inv[index]]);
    inv[index] = 0;
}

void find_favorite(int inv[], char names[][MAX_NAME_LEN]) {
    int max_count = 0;
    int favorite_id = 0;

    for (int i = 0; i < INVENTORY_SIZE; i++) {
        if (inv[i] == 0) continue;

        int count = 0;
        for (int j = 0; j < INVENTORY_SIZE; j++) {
            if (inv[j] == inv[i]) count++;
        }

        if (count > max_count) {
            max_count = count;
            favorite_id = inv[i];
        }
    }

    if (favorite_id == 0) {
        printf("Инвентарь пуст — любимого ресурса нет.\n");
    } else if (max_count == 1) {
        printf("Нет любимого ресурса — все предметы уникальны.\n");
    } else {
        printf("Любимый ресурс: ID %d (%s), количество слотов: %d\n", favorite_id, names[favorite_id], max_count);
    }
}

void find_by_name(int inv[], char names[][MAX_NAME_LEN]) {
    char search[MAX_NAME_LEN];
    printf("Введите название предмета: ");
    read_string(search, MAX_NAME_LEN);

    int found_id = -1;
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (strcmp(names[i], search) == 0) {
            found_id = i;
            break;
        }
    }

    if (found_id == -1) {
        printf("Такого предмета нет в каталоге.\n");
        return;
    }

    int count = 0;
    printf("Предмет '%s' (ID %d) найден в слотах: ", search, found_id);
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        if (inv[i] == found_id) {
            printf("%d ", i);
            count++;
        }
    }

    if (count == 0) {
        printf("нет");
    }
    printf("\n");
}