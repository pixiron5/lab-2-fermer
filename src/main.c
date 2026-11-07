#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utils.h"
#include "player.h"
#include "map.h"

int main(void) {
    int current_day = 1;
    int current_hour = 8;
    Player *player = NULL;

    char item_names[MAX_ITEMS][MAX_NAME_LEN];
    load_items(item_names);

    int choice;

    srand(time(NULL));

    while (1) {
        printf("\n=== День %d, %02d:00 ===\n", current_day, current_hour);
        printf("[0] Выход\n");
        printf("[1] Создать / пересоздать персонажа\n");
        printf("[2] Карточка персонажа\n");
        printf("[3] Посмотреть инвентарь\n");
        printf("[4] Положить предмет\n");
        printf("[5] Выбросить предмет\n");
        printf("[6] Любимый ресурс\n");
        printf("[7] Поиск предмета по названию\n");
        printf("[8] Записать состояние в дневник\n");
        printf("[9] Расшифровать старые записи\n");
        printf("[10] Отправиться в приключение\n");
        printf("[11] Управление временем\n");
        printf("Ваш выбор: ");

        if (scanf("%d", &choice) != 1) {
            printf("Ошибка: нужно ввести число!\n");
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');

        switch (choice) {
            case 0:
                free_player(&player);
                printf("Выход.\n");
                return 0;

            case 1: {
                if (player != NULL) {
                    char answer;
                    printf("Персонаж уже создан. Удалить и создать нового? (y/n): ");
                    scanf(" %c", &answer);
                    while (getchar() != '\n');
                    if (answer != 'y' && answer != 'Y') {
                        printf("Отмена.\n");
                        break;
                    }
                    free_player(&player);
                }
                player = create_player();
                break;
            }

            case 2:
                if (player == NULL) { printf("Сначала создайте персонажа!\n"); break; }
                show_player(player, item_names, current_day, current_hour);
                break;

            case 3:
                if (player == NULL) { printf("Сначала создайте персонажа!\n"); break; }
                show_inventory(player->inventory, item_names);
                break;

            case 4:
                if (player == NULL) { printf("Сначала создайте персонажа!\n"); break; }
                put_item(player->inventory);
                break;

            case 5:
                if (player == NULL) { printf("Сначала создайте персонажа!\n"); break; }
                drop_item(player->inventory, item_names);
                break;

            case 6:
                if (player == NULL) { printf("Сначала создайте персонажа!\n"); break; }
                find_favorite(player->inventory, item_names);
                break;

            case 7:
                if (player == NULL) { printf("Сначала создайте персонажа!\n"); break; }
                find_by_name(player->inventory, item_names);
                break;

            case 8:
                if (player == NULL) { printf("Сначала создайте персонажа!\n"); break; }
                write_diary(player->name, current_day, current_hour, player->inventory, item_names);
                break;

            case 9:
                if (player == NULL) { printf("Сначала создайте персонажа!\n"); break; }
                process_input(player->name, current_day, player->inventory, item_names);
                break;

            case 10:
                if (player == NULL) { printf("Сначала создайте персонажа!\n"); break; }
                adventure(player, &current_day, &current_hour);
                break;

            case 11:
                time_menu(&current_day, &current_hour, player);
                break;

            default:
                printf("Неверный пункт меню!\n");
        }
    }
    return 0;
}