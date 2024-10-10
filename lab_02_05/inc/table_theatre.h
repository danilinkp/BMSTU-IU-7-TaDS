#ifndef LAB_02_05_TABLE_THEATRE_H
#define LAB_02_05_TABLE_THEATRE_H

#include <stdlib.h>
#include "theatre.h"
#include "tools.h"

#define MAX_ROW_COUNT 10000
#define NUM_OF_ITERATIONS 1000

typedef int (*compar_t)(const void *, const void *);

typedef void (*sort_t)(void *, size_t, size_t, compar_t);

typedef struct
{
    int price_low;
    size_t theatre_id;
} theatre_key_t;

typedef struct
{
    theatre_t theatres[MAX_ROW_COUNT];
    size_t rows;
} table_theatre_t;

enum table_errors
{
    FILE_READ_ERROR = 9,
    EMPTY_FILE_ERROR,
    NAME_THEATRE_READ_ERROR,
    THEATRE_NOT_FOUND_ERROR,
    THEATRE_READ_ERROR,
    BALLET_NOT_FOUND_ERROR,
    TABLE_OVERFLOW_ERROR
};

// Функция чтения табоицы из файла
int fread_table(table_theatre_t *table_theatre, theatre_key_t table_key[], size_t *keys_size, FILE *file_input);

// Функция вывода таблицы
void fprint_table(table_theatre_t table_theatre, FILE *file_output);

// Функция вывода таблицы ключей
void fprint_key_table(theatre_key_t table_key[], size_t keys_size, FILE *file_output);

// Функция вывода таблицы с использование таблицы ключей
void fprint_table_by_keys(table_theatre_t table_theatre, theatre_key_t table_key[], FILE *file_output);

// Обновление таблицы ключей с основной таблицей
void update_keys_with_table(table_theatre_t table_theatre, theatre_key_t table_key[], size_t *keys_size);

// Функция удаления театра по его имени
int delete_theatre_by_name(table_theatre_t *table_theatre);

// Функция добавления театра
int add_theatre(table_theatre_t *table_theatre);

// Функция поиска баллетов по введенным данным
int find_ballets(table_theatre_t table_theatre, int age, int duration);

// Соритровка вставками
void insertion_sort(void *base, size_t nmemb, size_t size, compar_t cmp);

// Замер времени для таблицы
double time_sort_table(table_theatre_t table_theatre, size_t count, sort_t sort);

// Замер времени для таблицы клбчей
double time_sort_keys(theatre_key_t table_keys[], size_t table_size, size_t count, sort_t sort);


#endif //LAB_02_05_TABLE_THEATRE_H
