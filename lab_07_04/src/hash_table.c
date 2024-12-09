#include "hash_table.h"
#include "input_tools.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#define PRIME_NUM 151
#define START_HT_SIZE 57

/*
 * Заметка:
 * Реструктуризировать таблицу автоматически
 * изменять размер до следующего простого числа
 * Сделать функцию подсчёта среднего кол-ва колизий
 * спорный момент когда реструктуризовывать (когда коллизий больше 3-4) или среднее кол-во сравнений больше 3-4
 * этот момент буду решать или этим или тем (комбинированный подход)
 */

typedef size_t (*hash_t)(char *, int);

size_t polynom_hash(char *str, int ht_size)
{
    long hash = 0;
    size_t len_s = strlen(str);
    for (size_t i = 0; i < len_s; i++)
    {
        hash += (long) pow(PRIME_NUM, (double) (len_s - (i + 1))) * str[i];
        hash %= ht_size;
    }
    return (size_t) hash;
}

size_t simple_hash(char *str, int ht_size)
{
    size_t hash = 0;
    size_t len_s = strlen(str);
    for (size_t i = 0; i < len_s; i++)
        hash += str[i];

    return hash % ht_size;
}

key_node_t *list_push_back(key_node_t *head, key_node_t *new_item)
{
    if (!head)
        return new_item;

    key_node_t *curr = head;
    for (; curr->next; curr = curr->next);
    curr->next = new_item;
    new_item->next = NULL;
    return head;
}

void free_key_node(key_node_t *key_node)
{
    if (key_node->key)
    {
        free(key_node->key);
        key_node->key = NULL;
    }
    free(key_node);
    key_node = NULL;
}

void free_open_ht_item(key_node_t *item)
{
    key_node_t *next;
    for (; item; item = next)
    {
        next = item->next;
        free_key_node(item);
    }
}

void clear_open_ht_data(open_hash_table_t *hash_table)
{
    for (size_t i = 0; i < (size_t) hash_table->size; i++)
        if (hash_table->keys[i])
            free_open_ht_item(hash_table->keys[i]);
    hash_table->size = 0;
}

key_node_t *create_open_ht_item(char *str)
{
    key_node_t *new_key = calloc(1, sizeof(key_node_t));
    if (!new_key)
        return NULL;

    new_key->key = malloc((strlen(str) + 1) * sizeof(char));
    if (!new_key->key)
    {
        free(new_key);
        return NULL;
    }
    strcpy(new_key->key, str);

    return new_key;
}

int create_open_ht(open_hash_table_t *hash_table)
{
    hash_table->keys = calloc(MAX_ARR_LEN, sizeof(key_node_t *));
    if (!hash_table->keys)
        return OPEN_HT_DATA_ALLOC_ERROR;
    hash_table->size = START_HT_SIZE;
    hash_table->count = 0;
    return EXIT_SUCCESS;
}

void insert_open_ht(open_hash_table_t *hash_table, char *str, hash_t hash)
{
    key_node_t *new_item = create_open_ht_item(str);

    size_t index = hash(str, hash_table->size);
    hash_table->keys[index] = list_push_back(hash_table->keys[index], new_item);

    hash_table->count++;
}

char *search_open_ht(open_hash_table_t *hash_table, char *str, hash_t hash)
{
    hash_table->compares++;

    size_t index = hash(str, hash_table->size);

    if (hash_table->keys[index] == NULL)
        return NULL;

    for (key_node_t *curr_key = hash_table->keys[index]; curr_key; curr_key = curr_key->next)
    {
        hash_table->compares++;
        if (strcmp(str, curr_key->key) == 0)
            return curr_key->key;
    }

    return NULL;
}

void delete_open_ht(open_hash_table_t *hash_table, char *str, hash_t hash)
{
    size_t index = hash(str, hash_table->size);

    key_node_t *curr_key = hash_table->keys[index];
    key_node_t *prev = NULL;
    for (; curr_key; prev = curr_key, curr_key = curr_key->next)
    {
        if (strcmp(str, curr_key->key) == 0)
        {
            if (prev)
                prev->next = curr_key->next;
            else
                hash_table->keys[index] = curr_key->next;
            free_key_node(curr_key);
            hash_table->count--;
            return;
        }
    }
}

int fread_open_ht(FILE *file, open_hash_table_t *hash_table)
{
    size_t len = 0;
    char tmp_str[MAX_WORD_LEN + 1];
    while (read_str(file, tmp_str, MAX_WORD_LEN) == EXIT_SUCCESS)
    {
        insert_open_ht(hash_table, tmp_str, simple_hash);
        len++;
    }
    if (!len)
        return FILE_EMPTY_ERROR;

    if (!feof(file))
        return FILE_READ_ERROR;

    return EXIT_SUCCESS;
}

void print_open_ht(open_hash_table_t *hash_table)
{
    printf("Hash Table:\n");
    for (size_t i = 0; i < (size_t) hash_table->size; i++)
    {
        printf("Index %zu: ", i);
        key_node_t *current = hash_table->keys[i];
        if (!current)
        {
            printf("NULL\n");
            continue;
        }
        while (current)
        {
            printf("%s -> ", current->key);
            current = current->next;
        }
        printf("NULL\n");
    }
}

void restruct_open_ht(open_hash_table_t *hash_table)
{

}