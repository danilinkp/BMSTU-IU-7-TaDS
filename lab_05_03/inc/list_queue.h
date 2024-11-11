#ifndef LAB_05_03_LIST_QUEUE_H
#define LAB_05_03_LIST_QUEUE_H

#include "request.h"
#include "queue.h"

// Узел списка для стека
typedef struct node
{
    request_t data;
    struct node *next;
} node_t;

// Узел списка для освобожденных адрессов
typedef struct free_node
{
    void *address;
    struct free_node *next;
} free_node_t;

typedef struct
{
    node_t *head; // Указатель на начало очереди
    node_t *tail; // Указатель на конец очереди
    int size; // Размер очереди

    free_node_t *free_list; // Cписок освобжденных элементов
    int free_elems_size; // Кол-во освобожденных элементов
} list_queue_t;



#endif //LAB_05_03_LIST_QUEUE_H
