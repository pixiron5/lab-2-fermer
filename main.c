#include <stdio.h>
#include <string.h>
#include <stdlib.h>   
#include <time.h> 

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

void show_time(int day, int hour) { //ТЕКУЩИЕ ВРЕМЯ
    printf("Текущее время: День %d, %02d:00\n", day, hour);
}

void skip_time(int *day, int *hour) { //ПРОМОТАТЬ ВРЕМЯ
    int hours;
    printf("Сколько часов хотите промотать? ");
    if (scanf("%d", &hours) != 1) {
        printf("Ошибка: нужно ввести число!\n");
        while (getchar() != '\n');
        return;
    }
    if (hours < 0) {
        printf("Часы не могут быть отрицательными!\n");
        return;
    }
    *hour += hours;
    while (*hour >= 24) {
        *hour -= 24;
        (*day)++;
    }
    printf("Прошло %d часов.\n", hours);
}
   
void show_inventory(int inv[], char names[][MAX_NAME_LEN]) { //ПОСМОТРЕТЬ ИНВЕНТАРЬ
    for(int i = 0; i < INVENTORY_SIZE; i++)
        printf("Слот %d: [%d] - (%s)\n", i, inv[i], names[inv[i]]);
}

void put_item(int inv[]) { //ПОМЕСТИТЬ ПРЕДМЕТ В СЛОТ
    int index,id;
    printf("Введите индекс слота (0-%d): ", INVENTORY_SIZE - 1);
    if (scanf("%d", &index) != 1) {
        printf("Ошибка: Нужно ввести число!\n");
        while (getchar() != '\n');
        return;
    }
    if (index < 0 || index >= INVENTORY_SIZE) {
        printf("Ошибка: индекс вне границ массива (0-%d)!\n", INVENTORY_SIZE - 1);
        return;
    }
    printf("Введите индекс предмета (0-%d): ", MAX_ITEM_ID); 
    if (scanf("%d", &id) != 1) {
        printf("Ошибка: Нужно ввести число!\n");
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

void drop_item(int inv[], char names[][MAX_NAME_LEN]) { //ВЫКИНУТЬ ПРЕДМЕТ
    int index;
    printf("Введите индекс слота, из которого вы хотите выбросить предмет (0-%d): ", INVENTORY_SIZE-1);
    if (scanf("%d", &index) !=1) {
        printf("Ошибка: Нужно ввести число!\n");
        while (getchar() != '\n');
        return; 
    }
    if (index <0 || index >= INVENTORY_SIZE) {
        printf("Ошибка: индекс вне границ массива (0-%d)!\n", INVENTORY_SIZE - 1);
        return;  
    }
    printf("Выброшен предмет: %d (%s)\n", inv[index], names[inv[index]]);
    inv[index] = 0;
    return;

}

void find_favorite(int inv[], char names[][MAX_NAME_LEN]) { //ЛЮБИМЫЙ ПРЕДМЕТ
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
        printf("Любимый ресурс: ID %d (%s), количество слотов: %d\n",favorite_id, names[favorite_id], max_count);
    }
    return;
    }

void read_string(char *buffer, int size) { //ВВОД СТРОКИ
    fgets(buffer, size, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
}

void load_items(char names[][MAX_NAME_LEN]) { //ИНВЕНТАРЬ
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

void find_by_name(int inv[], char names[][MAX_NAME_LEN]) { //ПОИСК ПРДМЕТА
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

char **create_map(int height, int width) {
    char **map = malloc(height * sizeof(char*));

    if (map == NULL) {
    printf("Ошибка: не удалось выделить память!\n");
    return NULL;
    }
    for (int i = 0; i < height; i++) {
    map[i] = malloc(width * sizeof(char));
    if (map[i] == NULL) {
        for (int j = 0; j < i; j++) {
            free(map[j]);
        }
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
    for (int i = 0; i < height; i++) {
        free(map[i]);
    }
    free(map);
}

void print_map(char **map, int height, int width) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            printf("%c ", map[i][j]);
        }
        printf("\n");
    }
}

int main(void) {
    int current_day = 1;
    int current_hour = 8;
    int inventory[INVENTORY_SIZE] = {4, 6, 0, 2, 0, 3, 8, 0, 1, 0};
    char farmer_name[MAX_NAME_LEN];

    char item_names[MAX_ITEMS][MAX_NAME_LEN];
    load_items(item_names);

    int choice;

    srand(time(NULL));
    char **map = create_map(10, 10);
    if (map == NULL) return 1;
    fill_map(map, 10, 10);
    print_map(map, 10, 10);
    free_map(map, 10);
    printf("Память освобождена.\n");

    printf("Введите имя фермера: ");
    read_string(farmer_name, MAX_NAME_LEN);
    printf("Добро пожаловать, %s!\n", farmer_name);

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
                show_time(current_day,current_hour);
                break;
            case 2: 
                skip_time(&current_day,&current_hour);
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
                
                break;
            default:
                printf("Неверный пункт меню!\n");
        }
    }

    return 0;
}