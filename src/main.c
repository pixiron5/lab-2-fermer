// src/main.c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utils.h"
#include "player.h"
#include "map.h"

int main(void) {
    int current_day = 1;
    int current_hour = 8;
    int inventory[INVENTORY_SIZE] = {4, 6, 0, 2, 0, 3, 8, 0, 1, 0};
    char farmer_name[MAX_NAME_LEN];

    char item_names[MAX_ITEMS][MAX_NAME_LEN];
    load_items(item_names);

    int choice;

    printf("Введите имя фермера: ");
    read_string(farmer_name, MAX_NAME_LEN);
    printf("Добро пожаловать, %s!\n", farmer_name);

    srand(time(NULL));

    while (1) {
        printf("\n=== Фермер %s, День %d, %02d:00 ===\n", farmer_name, current_day, current_hour);
        printf("[0] Выход\n");
        printf("[1] Посмотреть на часы\n");
        printf("[2] Промотать время\n");
        printf("[3] Посмотреть инвентарь\n");
        printf("[4] Положить предмет\n");
        printf("[5] Выбросить предмет\n");
        printf("[6] Любимый ресурс\n");
        printf("[7] Поиск предмета по названию\n");
        printf("[8] Записать состояние в дневник\n");
        printf("[9] Расшифровать старые записи\n");
        printf("[10] Отправиться в приключение\n");
        printf("Ваш выбор: ");

        if (scanf("%d", &choice) != 1) {
            printf("Ошибка: нужно ввести число!\n");
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');

        switch (choice) {
            case 0:
                printf("Выход.\n");
                return 0;
            case 1:
                show_time(current_day, current_hour);
                break;
            case 2:
                skip_time(&current_day, &current_hour);
                break;
            case 3:
                show_inventory(inventory, item_names);
                break;
            case 4:
                put_item(inventory);
                break;
            case 5:
                drop_item(inventory, item_names);
                break;
            case 6:
                find_favorite(inventory, item_names);
                break;
            case 7:
                find_by_name(inventory, item_names);
                break;
            case 8:
                write_diary(farmer_name, current_day, current_hour, inventory, item_names);
                break;
            case 9:
                process_input(farmer_name, current_day, inventory, item_names);
                break;
            case 10:
                adventure();
                break;
            default:
                printf("Неверный пункт меню!\n");
        }
    }
    return 0;
}