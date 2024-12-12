#ifndef LAB_07_04_HASH_TABLE_H
#define LAB_07_04_HASH_TABLE_H

#include <stdlib.h>
#include <stdio.h>

#define MAX_WORD_LEN 256
#define MAX_ARR_LEN 1000

typedef char *ht_key_t;

typedef struct node
{
    ht_key_t key;
    struct node *next;
} key_node_t;

typedef struct
{
    key_node_t **keys;
    int size;
    int count;
} open_hash_table_t;

typedef struct
{
    ht_key_t *keys;
    int size;
    int count;
} closed_hash_table_t;

enum ht_errors
{
    HT_DATA_ALLOC_ERROR = 6,
    HT_FILE_EMPTY_ERROR,
    HT_FILE_READ_ERROR,
};

size_t simple_hash(char *str, int ht_size);

//open hash table

// Очищение данных хеш-таблицы
void clear_open_ht_data(open_hash_table_t *hash_table);

// Инициализация хеш-таблицы
int create_open_ht(open_hash_table_t *hash_table);

// Вставка в хеш-таблицу
void insert_open_ht(open_hash_table_t *hash_table, char *str);

// Поиск в хеш-таблице
char *search_open_ht(open_hash_table_t *hash_table, char *str);

// Кол-во сравнений для поиска
int cmps_search_open_ht(open_hash_table_t *hash_table, char *str);

// Удлаение элемента из хеш-таблицы
void delete_open_ht(open_hash_table_t *hash_table, char *str);

// Удаление всех элементов, начинающихся на заданную букву
void del_by_first_letter_open_ht(open_hash_table_t *hash_table, char letter);

// Чтение хеш-таблицы из файла
int fread_open_ht(FILE *file, open_hash_table_t *hash_table);

// Вывод хеш-таблицы
void print_open_ht(open_hash_table_t *hash_table);

// Реструктуризация хеш-таблицы
int restruct_open_ht(open_hash_table_t *hash_table);

// closed hash table

// Очищение данных хеш-таблицы
void clear_closed_ht_data(closed_hash_table_t *hash_table);

// Инициализация хеш-таблицы
int create_closed_ht(closed_hash_table_t *hash_table);

// Вставка в хеш-таблицу
void insert_closed_ht(closed_hash_table_t *hash_table, char *str);

// Поиск в хеш-таблице
char *search_closed_ht(closed_hash_table_t *hash_table, char *str);

// Кол-во сравнений для поиска
int cmps_search_closed_ht(closed_hash_table_t *hash_table, char *str);

// Удлаение элемента из хеш-таблицы
void delete_closed_ht(closed_hash_table_t *hash_table, char *str);

// Удаление всех элементов, начинающихся на заданную букву
void del_by_first_letter_closed_ht(closed_hash_table_t *hash_table, char letter);

// Чтение хеш-таблицы из файла
int fread_closed_ht(FILE *file, closed_hash_table_t *hash_table);

// Вывод хеш-таблицы
void print_closed_ht(closed_hash_table_t *hash_table);

// Реструктуризация хеш-таблицы
int restruct_closed_ht(closed_hash_table_t *hash_table);

#endif //LAB_07_04_HASH_TABLE_H


