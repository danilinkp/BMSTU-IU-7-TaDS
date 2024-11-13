#ifndef LAB_05_03_QUEUE_H
#define LAB_05_03_QUEUE_H

#include "request.h"
#include "array_queue.h"
#include "list_queue.h"

enum errors
{
    QUEUE_OVERFLOW = 1,
    QUEUE_UNDERFLOW,
};

// Перечисление для выбора типа очереди
typedef enum {
    ARRAY_QUEUE,
    LIST_QUEUE
} queue_type_t;

// Универсальная структура очереди
typedef struct {
    queue_type_t type; // Тип очереди
    arr_queue_t arr_queue;  // Очередь на массиве
    list_queue_t list_queue;  // Очередь на списке
} queue_t;

void queue_init(queue_t *queue, queue_type_t queue_type, int max_size);

int is_queue_empty(queue_t *queue);

int is_queue_full(queue_t *queue);

int enqueue(queue_t *queue, request_t elem);

int dequeue(queue_t *queue, request_t *removed_elem);

void print_queue(queue_t *queue);

int queue_length(queue_t *queue);

void queue_print_free_area(queue_t *queue);

void queue_free(queue_t *queue);

#endif //LAB_05_03_QUEUE_H
