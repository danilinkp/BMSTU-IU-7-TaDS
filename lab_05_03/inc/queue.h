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

#endif //LAB_05_03_QUEUE_H
