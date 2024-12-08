#ifndef LAB_07_04_HASH_TABLE_H
#define LAB_07_04_HASH_TABLE_H

#define MAX_WORD_LEN 256
#define HASH_TABLE_CAPACITY 257

typedef char key_t[MAX_WORD_LEN];

typedef struct node
{
    key_t key;
    struct node *next;
} node_t;

typedef struct
{
    node_t *keys;
    int size;
    int capacity;
} hash_table_t;

#endif //LAB_07_04_HASH_TABLE_H
