#ifndef LAB_05_03_ARRAY_QUEUE_H
#define LAB_05_03_ARRAY_QUEUE_H

#include "request.h"
#include "queue.h"


typedef struct
{
    request_t data[MAX_QUEUE_SIZE]; // Элементы очереди
    int size; // Размер очереди
    int head; // Индекс начала очереди
    int tail; // Индекс конца очереди
} arr_queue_t;

// Инициализация очереди
void arr_queue_init(arr_queue_t *queue);

// Проверка очереди на пустоту
int is_arr_queue_empty(arr_queue_t *queue);

// Проверка очереди на переолненность
int is_arr_queue_full(arr_queue_t *queue);

// Добавление элемента в очередь
int arr_enqueue(arr_queue_t *queue, request_t elem);

// Удаление элемента из очереди
int arr_dequeque(arr_queue_t *queue, request_t *removed_elem);

// Вывод элементов очереди на экран
void print_arr_queue(arr_queue_t *queue);

#endif //LAB_05_03_ARRAY_QUEUE_H
