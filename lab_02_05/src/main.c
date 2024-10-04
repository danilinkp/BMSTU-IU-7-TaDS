#include "table_theatre.h"
#include "menu.h"
#include <stdio.h>

int main(void)
{
    int rc;
    table_theatre_t table_theatre = {.rows = 0};
    theatre_key_t table_keys[MAX_ROW_COUNT];
    size_t size_keys = 0;
    char file_name[256 + 1];
    FILE *file = NULL;
    int action = -1;
    double avg_table_quick_sort, avg_table_insertion_sort, avg_keys_quick_sort, avg_keys_insertion_sort;
    while (action != EXIT_PROGRAM)
    {
        menu_print();
        if (scanf("%d", &action) != 1)
        {
            printf("Нет такого пункта!");
            continue;
        }

        switch (action)
        {
            case EXIT_PROGRAM:
                break;
            case LOAD_TABLE:
                printf("Введите имя файла: ");
                if (read_str(stdin, file_name, sizeof(file_name)) != EXIT_SUCCESS)
                {
                    printf("Ошибка чтения имени файла.\n");
                    break;
                }
                file = fopen(file_name, "rt");
                if (!file)
                {
                    printf("Ошибка открытия файла или его не существует.\n");
                    break;
                }

                rc = fread_table(&table_theatre, table_keys, &size_keys, file);
                if (rc != EXIT_SUCCESS)
                {
                    fclose(file);
                    printf("Файл записан не правильно, или он пуст.\n");
                    break;
                }
                printf("Таблица успешна загружена.\n");
                break;
            case PRINT_NOT_SORTED_TABLE:
                if (table_theatre.rows == 0)
                    printf("Таблица пуста.\n");
                else
                    fprint_table(table_theatre, stdout);
                break;
            case PRINT_NOT_SORTED_KEYS:
                if (size_keys == 0)
                    printf("Таблица ключей пуста.\n");
                else
                    fprint_key_table(table_keys, size_keys, stdout);
                break;
            case ADD_THEATRE:
                rc = add_theatre(&table_theatre);
                if (rc != EXIT_SUCCESS)
                {
                    printf("Ошибка добавления театра.\n");
                    break;
                }
                update_keys_with_table(table_theatre, table_keys, &size_keys);
                printf("Театр был успешно добавлен.\n");
                break;
            case DELETE_THEATRE:
                rc = delete_theatre_by_name(&table_theatre);
                if (rc != EXIT_SUCCESS)
                {
                    printf("Ничего не удалилось.\n");
                    break;
                }
                update_keys_with_table(table_theatre, table_keys, &size_keys);
                break;
            case PRINT_SORTED_KEYS:
                if (size_keys == 0)
                    printf("Таблица ключей пуста.\n");
                qsort(table_keys, size_keys, sizeof(theatre_key_t), key_comp);
                fprint_key_table(table_keys, size_keys, stdout);
                break;
            case PRINT_SORTED_TABLE:
                if (table_theatre.rows == 0)
                    printf("Таблица пуста.\n");
                qsort(&table_theatre, table_theatre.rows, sizeof(table_theatre), theatre_t_comp);
                fprint_table(table_theatre, stdout);
                update_keys_with_table(table_theatre, table_keys, &size_keys);
                break;
            case PRINT_SORTED_TABLE_BY_KEYS:
                if (size_keys == 0)
                {
                    printf("Таблицы пусты.\n");
                }
                qsort(table_keys, size_keys, sizeof(theatre_key_t), key_comp);
                fprint_table_by_keys(table_theatre, table_keys, stdout);
                break;
            case PRINT_BALLETS:
                printf("Введите возрастное ограничение на балет: ");
                age_limit_t age_limit;
                if (read_age_limit(&age_limit, stdin) != EXIT_SUCCESS)
                {
                    printf("Ошибка ввода возраста.\n");
                    break;
                }
                printf("Введите продолжительность балета: ");
                int duration;
                if (scanf("%d", &duration) != 1)
                {
                    printf("Ошибка ввода продолжительности.\n");
                    break;
                }
                rc = find_ballets(table_theatre, age_limit, duration);
                if (rc != EXIT_SUCCESS)
                {
                    printf("Балеты не найдены.\n");
                    break;
                }
            case COMP_TABLE_WITH_KEYS:
                if (table_theatre.rows == 0)
                {
                    printf("Таблицы пусты. Замерять нечего.\n");
                    break;
                }
                printf("Таблица времени от сортировки и алгоритма работы:\n");
                avg_table_quick_sort = time_sort_table(table_theatre, NUM_OF_ITERATIONS, qsort);
                avg_keys_quick_sort = time_sort_keys(table_keys, size_keys, NUM_OF_ITERATIONS, qsort);

                avg_table_insertion_sort = time_sort_table(table_theatre, NUM_OF_ITERATIONS, insertion_sort);
                avg_keys_insertion_sort = time_sort_keys(table_keys, size_keys, NUM_OF_ITERATIONS, insertion_sort);

                printf("| %-25s | %-25s | %-14s |\n", "Алгоритм сортировки", "Структра данных", "Среднее время");
                printf("|---------------------|-----------------|---------------|\n");
                printf("| %-25s | %-25s | %-14lf |", "Quick Sort", "Таблица", avg_table_quick_sort);
                printf("| %-25s | %-25s | %-14lf |", "Quick Sort", "Ключи", avg_keys_quick_sort);
                printf("|---------------------|-----------------|---------------|\n");
                printf("| %-25s | %-25s | %-14lf |", "Insertion Sort", "Таблица", avg_table_insertion_sort);
                printf("| %-25s | %-25s | %-14lf |", "Insertion Sort", "Ключи", avg_keys_insertion_sort);

                printf(" -Quick sort sort: ключи на %.2lf%% более эффективны, чем сортировка таблицы\n",
                       (avg_table_quick_sort - avg_keys_quick_sort) / avg_table_quick_sort * 100);
                printf(" -Insertion sort: ключи на  %.2lf%% более эффективны, чем сортировка таблицы\n",
                       (avg_table_insertion_sort - avg_keys_insertion_sort) / avg_table_insertion_sort * 100);

                printf("\nРазмеры: \n");
                printf(" -Таблицы: %zu байт\n", sizeof(table_theatre));
                printf(" -Таблицы + ключи: %zu байт\n", sizeof(table_theatre) + sizeof(table_keys) * size_keys);
                printf(" -Ключи: %zu байт\n", sizeof(table_keys) * size_keys);
                printf("Таблица с ключами занимает на %.2lf%% больше места, чем сама таблица\n",
                       ((double) (sizeof(table_theatre) + sizeof(table_keys) * size_keys)) /
                       ((double) (sizeof(table_theatre))) * 100 - 100);
                break;
            case COMPARE_SORTS:
                if (table_theatre.rows == 0)
                {
                    printf("Таблицы пусты. Замерять нечего.\n");
                    break;
                }
                printf("Таблица времени от сортировки и алгоритма работы:\n");
                avg_table_quick_sort = time_sort_table(table_theatre, NUM_OF_ITERATIONS, qsort);
                avg_keys_quick_sort = time_sort_keys(table_keys, size_keys, NUM_OF_ITERATIONS, qsort);

                avg_table_insertion_sort = time_sort_table(table_theatre, NUM_OF_ITERATIONS, insertion_sort);
                avg_keys_insertion_sort = time_sort_keys(table_keys, size_keys, NUM_OF_ITERATIONS, insertion_sort);

                printf("| %-25s | %-25s | %-14s |\n", "Алгоритм сортировки", "Структра данных", "Среднее время");
                printf("|---------------------|-----------------|---------------|\n");
                printf("| %-25s | %-25s | %-14lf |", "Quick Sort", "Таблица", avg_table_quick_sort);
                printf("| %-25s | %-25s | %-14lf |", "Quick Sort", "Ключи", avg_keys_quick_sort);
                printf("|---------------------|-----------------|---------------|\n");
                printf("| %-25s | %-25s | %-14lf |", "Insertion Sort", "Таблица", avg_table_insertion_sort);
                printf("| %-25s | %-25s | %-14lf |", "Insertion Sort", "Ключи", avg_keys_insertion_sort);

                printf(" -Qsort на %.2lf%% более эффективен, чем сортировка вставками для Таблиц\n",
                       (avg_table_insertion_sort - avg_table_quick_sort) / avg_table_insertion_sort * 100);
                printf(" -Qsort на %.2lf%% более эффективен, чем сортировка вставками для Ключей\n",
                       (avg_keys_insertion_sort - avg_keys_quick_sort) / avg_keys_insertion_sort * 100);

                printf("\nРазмеры: \n");
                printf(" -Таблицы: %zu байт\n", sizeof(table_theatre));
                printf(" -Таблицы + ключи: %zu байт\n", sizeof(table_theatre) + sizeof(table_keys) * size_keys);
                printf(" -Ключи: %zu байт\n", sizeof(table_keys) * size_keys);
                printf("Таблица с ключами занимает на %.2lf%% больше места, чем сама таблица\n",
                       ((double) (sizeof(table_theatre) + sizeof(table_keys) * size_keys)) /
                       ((double) (sizeof(table_theatre))) * 100 - 100);
                break;
            default:
                printf("Выберите 1 из пунктов меню.\n");
                break;
        }
    }
    return EXIT_SUCCESS;
}
