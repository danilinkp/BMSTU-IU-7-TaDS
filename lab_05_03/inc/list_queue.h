#ifndef LAB_05_03_LIST_QUEUE_H
#define LAB_05_03_LIST_QUEUE_H

#include "request.h"

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
    int max_size; // Максимальный размер очереди

    free_node_t *free_list; // Cписок освобжденных элементов
    int free_elems_size; // Кол-во освобожденных элементов
} list_queue_t;

// Инициализация очереди
void list_queue_init(list_queue_t *queue, int max_size);

// Проверка очереди на пустоту
int is_list_queue_empty(list_queue_t *queue);

// Проверка очереди на переполненность
int is_list_queue_full(list_queue_t *queue);

// Добавление элемента в очередь
int list_enqueue(list_queue_t *queue, request_t elem);

// Удаление элемента из очереди
int list_dequeue(list_queue_t *queue, request_t *removed_elem);

// Вывод элементов очереди на экран
void print_list_queue(list_queue_t *queue);

// Очищение очереди
void free_list_queue(list_queue_t *queue);

// Вывод освобжденной области
void print_free_area(list_queue_t *queue);

void test_memory_fragmentation(int initial_fill, int remove_count, int additional_fill);

#endif //LAB_05_03_LIST_QUEUE_H
