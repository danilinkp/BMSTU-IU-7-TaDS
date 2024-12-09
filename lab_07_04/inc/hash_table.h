#ifndef LAB_07_04_HASH_TABLE_H
#define LAB_07_04_HASH_TABLE_H

#define MAX_WORD_LEN 256
#define MAX_ARR_LEN 1000

typedef char *key_t;

typedef struct node
{
    key_t key;
    struct node *next;
} key_node_t;

typedef struct
{
    key_node_t **keys;
    int size;
    int count;
    int compares;
} open_hash_table_t;

typedef struct
{
    key_t keys[MAX_ARR_LEN];
    int size;
} close_hash_table_t;

enum ht_errors
{
    OPEN_HT_DATA_ALLOC_ERROR = 6,
    FILE_EMPTY_ERROR,
    FILE_READ_ERROR,
};


#endif //LAB_07_04_HASH_TABLE_H
