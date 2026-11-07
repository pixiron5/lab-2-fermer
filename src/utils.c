#include <stdio.h>
#include <string.h>
#include "utils.h"
#include "player.h"

void show_time(int day, int hour) {
    printf("Текущее время: День %d, %02d:00\n", day, hour);
}

void wait_time(int *day, int *hour) {
    int hours;
    printf("Сколько часов ждать? (нельзя пересечь 02:00): ");
    if (scanf("%d", &hours) != 1) {
        printf("Ошибка: нужно ввести число!\n");
        while (getchar() != '\n');
        return;
    }
    if (hours <= 0) {
        printf("Нужно ждать хотя бы 1 час!\n");
        return;
    }

    int total = *hour + hours;
    int new_day = *day + total / 24;
    int new_hour = total % 24;

    if (new_day > *day && new_hour >= 2) {
        printf("Нельзя ждать до 02:00! Фермер должен спать.\n");
        return;
    }

    *day = new_day;
    *hour = new_hour;
    printf("Прошло %d часов. Сейчас: День %d, %02d:00\n", hours, *day, *hour);
}

void time_menu(int *day, int *hour, Player *player) {
    int choice;
    while (1) {
        printf("\n=== Управление временем ===\n");
        printf("[0] Назад\n");
        printf("[1] Посмотреть на часы\n");
        printf("[2] Ждать\n");
        printf("[3] Лечь спать\n");
        printf("Ваш выбор: ");

        if (scanf("%d", &choice) != 1) {
            printf("Ошибка: нужно ввести число!\n");
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');

        switch (choice) {
            case 0: return;
            case 1: show_time(*day, *hour); break;
            case 2: wait_time(day, hour); break;
            case 3:
                if (player == NULL) {
                    printf("Сначала создайте персонажа!\n");
                } else {
                    (*day)++;
                    *hour = 8;
                    player->hp = player->max_hp;
                    player->stamina = player->max_stamina;
                    printf("Вы поспали. День %d, 08:00. HP и выносливость восстановлены.\n", *day);
                }
                break;
            default: printf("Неверный пункт меню!\n");
        }
    }
}

void read_string(char *buffer, int size) {
    fgets(buffer, size, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
}

void load_items(char names[][MAX_NAME_LEN]) {
    const char *defaults[MAX_ITEMS] = {
        "Пусто", "Дерево", "Камень", "Семена", "Лопата",
        "Грабли", "Тяпка", "Телега", "Лейка", "Корзина"
    };
    FILE *file = fopen("items.txt", "r");
    if (file == NULL) {
        printf("Предупреждение: items.txt не найден, использую стандартные названия.\n");
        for (int i = 0; i < MAX_ITEMS; i++) {
            strcpy(names[i], defaults[i]);
        }
        return;
    }
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (fgets(names[i], MAX_NAME_LEN, file) == NULL) {
            break;
        }
        names[i][strcspn(names[i], "\n")] = '\0';
    }
    fclose(file);
}

void replace_tag(const char *src, const char *tag, const char *value, char *dest) {
    int i = 0;
    int j = 0;

    while (src[i] != '\0') {
        if (strstr(&src[i], tag) == &src[i]) {
            for (int k = 0; value[k] != '\0'; k++) {
                dest[j] = value[k];
                j++;
            }
            i += strlen(tag);
        } else {
            dest[j] = src[i];
            i++;
            j++;
        }
    }
    dest[j] = '\0';
}

void process_input(char farmer_name[], int current_day, int inv[], char item_names[][MAX_NAME_LEN]) {
    FILE *in = fopen("input.txt", "r");
    if (in == NULL) {
        printf("Ошибка: input.txt не найден!\n");
        return;
    }

    FILE *out = fopen("output.txt", "w");
    if (out == NULL) {
        printf("Ошибка: не удалось открыть output.txt!\n");
        fclose(in);
        return;
    }

    char day_str[16];
    sprintf(day_str, "%d", current_day);

    const char *item_name = item_names[inv[0]];

    char line[MAX_LINE_LEN];
    while (fgets(line, MAX_LINE_LEN, in) != NULL) {
        line[strcspn(line, "\n")] = '\0';

        char temp1[MAX_LINE_LEN * 2];
        char temp2[MAX_LINE_LEN * 2];
        char result[MAX_LINE_LEN * 2];

        replace_tag(line, "<NAME>", farmer_name, temp1);
        replace_tag(temp1, "<DAY>", day_str, temp2);
        replace_tag(temp2, "<ITEM>", item_name, result);

        printf("%s\n", result);
        fprintf(out, "%s\n", result);
    }

    fclose(in);
    fclose(out);
    printf("\nРезультат сохранён в output.txt.\n");
}

void write_diary(char farmer_name[], int day, int hour, int inv[], char names[][MAX_NAME_LEN]) {
    FILE *file = fopen("diary.txt", "a");
    if (file == NULL) {
        printf("Ошибка: не удалось открыть diary.txt для записи!\n");
        return;
    }

    fprintf(file, "=== Фермер %s, День %d, %02d:00 ===\n", farmer_name, day, hour);
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        fprintf(file, "Слот %d: %s\n", i, names[inv[i]]);
    }
    fprintf(file, "\n");

    fclose(file);
    printf("Дневник записан в diary.txt.\n");
}