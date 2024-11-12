#include "array_queue.h"
#include <stdlib.h>
#include <stdio.h>

void arr_queue_init(arr_queue_t *queue, int max_size)
{
    queue->size = 0;
    queue->head = 0;
    queue->max_size = max_size;
    queue->tail = 0;
}

int is_arr_queue_empty(arr_queue_t *queue)
{
    return queue->size == 0;
}

int is_arr_queue_full(arr_queue_t *queue)
{
    return (queue->head == (queue->tail + 1) % queue->max_size);
}

void arr_enqueue(arr_queue_t *queue, request_t elem)
{
    queue->data[queue->tail] = elem;
    queue->tail = (queue->tail + 1) % queue->max_size;
    queue->size++;
}

request_t arr_dequeque(arr_queue_t *queue)
{
    request_t removed_elem = queue->data[queue->head];
    queue->head = (queue->head + 1) % queue->max_size;
    queue->size--;
    return removed_elem;
}

void print_arr_queue(arr_queue_t *queue)
{
    for (int i = 0; i < queue->size; i++)
    {
        int index = (queue->head + i) % queue->max_size;
        printf("%d ", queue->data[index].id);
    }
}
