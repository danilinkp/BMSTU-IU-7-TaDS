#include "hash_table.h"
#include "input_tools.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#define PRIME_NUM 151
#define START_HT_SIZE 13
#define MAX_COLLISIONS 3
#define MAX_LOAD_FACTOR 70

static int is_prime(int x)
{
    if (x < 2)
        return -1;
    if (x < 4)
        return 1;
    if ((x % 2) == 0)
        return 0;
    for (int i = 3; i <= floor(sqrt((double) x)); i += 2)
        if ((x % i) == 0)
            return 0;
    return 1;
}

static int next_prime(int x)
{
    x++;
    while (!is_prime(x))
        x++;
    return x;
}

static key_node_t *list_push_back(key_node_t *head, key_node_t *new_item)
{
    if (!head)
        return new_item;

    key_node_t *curr = head;
    for (; curr->next; curr = curr->next);
    curr->next = new_item;
    new_item->next = NULL;
    return head;
}

static size_t list_len(key_node_t *item)
{
    size_t len = 0;
    for (key_node_t *curr = item; curr; curr = curr->next)
        len++;

    return len;
}

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
        hash += (size_t) str[i];

    return hash % ht_size;
}

static void free_key_node(key_node_t *key_node)
{
    if (key_node->key)
        free(key_node->key);
    free(key_node);
}

static void free_open_ht_item(key_node_t *item)
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
    if (hash_table->keys)
    {
        for (size_t i = 0; i < (size_t) hash_table->size; i++)
            if (hash_table->keys[i])
                free_open_ht_item(hash_table->keys[i]);

        free(hash_table->keys);
        hash_table->keys = NULL;
        hash_table->size = 0;
        hash_table->count = 0;
    }
}

static key_node_t *create_open_ht_item(char *str)
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
    hash_table->keys = calloc(START_HT_SIZE, sizeof(key_node_t *));
    if (!hash_table->keys)
        return HT_DATA_ALLOC_ERROR;
    hash_table->size = START_HT_SIZE;
    hash_table->count = 0;
    return EXIT_SUCCESS;
}

void insert_open_ht(open_hash_table_t *hash_table, char *str)
{
    key_node_t *new_item = create_open_ht_item(str);

    size_t index = simple_hash(str, hash_table->size);

    if (list_len(hash_table->keys[index]) >= MAX_COLLISIONS)
        restruct_open_ht(hash_table);

    hash_table->keys[index] = list_push_back(hash_table->keys[index], new_item);

    hash_table->count++;
}

char *search_open_ht(open_hash_table_t *hash_table, char *str)
{
    size_t index = simple_hash(str, hash_table->size);

    if (hash_table->keys[index] == NULL)
        return NULL;

    for (key_node_t *curr_key = hash_table->keys[index]; curr_key; curr_key = curr_key->next)
        if (strcmp(str, curr_key->key) == 0)
            return curr_key->key;

    return NULL;
}

int cmps_search_open_ht(open_hash_table_t *hash_table, char *str)
{
    size_t index = simple_hash(str, hash_table->size);
    int cmps = 0;
    if (hash_table->keys[index] == 0)
        return 0;

    for (key_node_t *curr_key = hash_table->keys[index]; curr_key; curr_key = curr_key->next)
    {
        cmps++;
        if (strcmp(str, curr_key->key) == 0)
            return cmps;
    }

    return cmps;
}

void delete_open_ht(open_hash_table_t *hash_table, char *str)
{
    size_t index = simple_hash(str, hash_table->size);

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

void del_by_first_letter_open_ht(open_hash_table_t *hash_table, char letter)
{
    for (size_t i = 0; i < (size_t) hash_table->size; i++)
    {
        key_node_t *current = hash_table->keys[i];
        while (current)
        {
            key_node_t *next = current->next;

            if (current->key[0] == letter)
                delete_open_ht(hash_table, current->key);

            current = next;
        }
    }
}

int fread_open_ht(FILE *file, open_hash_table_t *hash_table)
{
    rewind(file);
    size_t len = 0;
    char tmp_str[MAX_WORD_LEN + 1];
    while (read_str(file, tmp_str, MAX_WORD_LEN) == EXIT_SUCCESS)
    {
        insert_open_ht(hash_table, tmp_str);
        len++;
    }
    if (!len)
        return HT_FILE_EMPTY_ERROR;

    if (!feof(file))
        return HT_FILE_READ_ERROR;

    return EXIT_SUCCESS;
}

void print_open_ht(open_hash_table_t *hash_table)
{
    printf("Open Hash Table:\n");
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

int restruct_open_ht(open_hash_table_t *hash_table)
{
    key_node_t **old_keys = hash_table->keys;
    size_t old_size = hash_table->size;
    size_t new_size = next_prime((int) hash_table->size);

    hash_table->keys = calloc(new_size, sizeof(key_node_t *));
    if (!hash_table->keys)
    {
        hash_table->keys = old_keys;
        return HT_DATA_ALLOC_ERROR;
    }
    hash_table->size = (int) new_size;
    hash_table->count = 0;

    for (size_t i = 0; i < old_size; i++)
    {
        key_node_t *curr = old_keys[i];
        for (; curr; curr = curr->next)
            insert_open_ht(hash_table, curr->key);
    }

    free(old_keys);
    return EXIT_SUCCESS;
}

void clear_closed_ht_data(closed_hash_table_t *hash_table)
{
    if (!hash_table->keys)
        return;

    for (size_t i = 0; i < (size_t) hash_table->size; i++)
        if (hash_table->keys[i])
            free(hash_table->keys[i]);

    free(hash_table->keys);
    hash_table->keys = NULL;
    hash_table->size = 0;
    hash_table->count = 0;
}

int create_closed_ht(closed_hash_table_t *hash_table)
{
    hash_table->keys = calloc(START_HT_SIZE, sizeof(ht_key_t));
    if (!hash_table->keys)
        return HT_DATA_ALLOC_ERROR;
    hash_table->size = START_HT_SIZE;
    hash_table->count = 0;
    return EXIT_SUCCESS;
}

void insert_closed_ht(closed_hash_table_t *hash_table, char *str)
{
    const int load = hash_table->count * 100 / hash_table->size;
    if (load > MAX_LOAD_FACTOR)
        restruct_closed_ht(hash_table);

    size_t index = simple_hash(str, hash_table->size);

    for (int i = 0; i < hash_table->size; ++i)
    {
        size_t curr_index = (index + i * i) % hash_table->size;
        if (hash_table->keys[curr_index] == NULL)
        {
            hash_table->keys[curr_index] = strdup(str);
            hash_table->count++;
            return;
        }
    }

    restruct_closed_ht(hash_table);
    insert_closed_ht(hash_table, str);
}

char *search_closed_ht(closed_hash_table_t *hash_table, char *str)
{
    size_t index = simple_hash(str, hash_table->size);

    ht_key_t key = hash_table->keys[index];

    if (key == NULL)
        return NULL;

    for (int i = 0; i < hash_table->size; ++i)
    {
        size_t curr_index = (index + i * i) % hash_table->size;
        if (strcmp(hash_table->keys[curr_index], str) == 0)
            return hash_table->keys[curr_index];
    }

    return NULL;
}

int cmps_search_closed_ht(closed_hash_table_t *hash_table, char *str)
{
    size_t index = simple_hash(str, hash_table->size);

    ht_key_t key = hash_table->keys[index];

    if (key == NULL)
        return 0;

    int compares = 0;
    for (int i = 0; i < hash_table->size; ++i)
    {
        compares++;
        size_t curr_index = (index + i * i) % hash_table->size;
        if (strcmp(hash_table->keys[curr_index], str) == 0)
            return compares;
    }

    return compares;
}

void delete_closed_ht(closed_hash_table_t *hash_table, char *str)
{
    size_t index = simple_hash(str, hash_table->size);
    for (int i = 0; i < hash_table->size; ++i)
    {
        size_t curr_index = (index + i * i) % hash_table->size;
        if (hash_table->keys[curr_index])
            if (strcmp(hash_table->keys[curr_index], str) == 0)
            {
                free(hash_table->keys[curr_index]);
                hash_table->keys[curr_index] = NULL;
                hash_table->count--;
                return;
            }
    }
}

void del_by_first_letter_closed_ht(closed_hash_table_t *hash_table, char letter)
{
    for (size_t i = 0; i < (size_t) hash_table->size; i++)
    {
        ht_key_t key = hash_table->keys[i];
        if (key && key[0] == letter)
            delete_closed_ht(hash_table, key);
    }
}

int fread_closed_ht(FILE *file, closed_hash_table_t *hash_table)
{
    rewind(file);
    size_t len = 0;
    char tmp_str[MAX_WORD_LEN + 1];
    while (read_str(file, tmp_str, MAX_WORD_LEN) == EXIT_SUCCESS)
    {
        insert_closed_ht(hash_table, tmp_str);
        len++;
    }
    if (!len)
        return HT_FILE_EMPTY_ERROR;
    if (!feof(file))
        return HT_FILE_READ_ERROR;

    return EXIT_SUCCESS;
}

void print_closed_ht(closed_hash_table_t *hash_table)
{
    printf("Closed Hash Table:\n");
    for (size_t i = 0; i < (size_t) hash_table->size; i++)
    {
        printf("Index %zu: ", i);
        ht_key_t key = hash_table->keys[i];
        if (!key)
        {
            printf("NULL\n");
            continue;
        }
        printf("%s\n", key);
    }
}

int restruct_closed_ht(closed_hash_table_t *hash_table)
{
    ht_key_t *old_keys = hash_table->keys;
    size_t old_size = hash_table->size;
    size_t new_size = next_prime((int) hash_table->size);

    hash_table->keys = calloc(new_size, sizeof(ht_key_t));
    if (!hash_table->keys)
    {
        hash_table->keys = old_keys;
        return HT_DATA_ALLOC_ERROR;
    }
    hash_table->size = (int) new_size;
    hash_table->count = 0;

    for (size_t i = 0; i < old_size; i++)
        if (old_keys[i])
            insert_closed_ht(hash_table, old_keys[i]);

    free(old_keys);
    return EXIT_SUCCESS;
}
