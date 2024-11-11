#include "array_queue.h"
#include <stdlib.h>
#include <stdio.h>

void arr_queue_init(arr_queue_t *queue)
{
    queue->size = 0;
    queue->head = 0;
    queue->tail = 0;
}

int is_arr_queue_empty(arr_queue_t *queue)
{
    return queue->size == 0;
}

int is_arr_queue_full(arr_queue_t *queue)
{
    return (queue->head == (queue->tail + 1) % MAX_QUEUE_SIZE);
}

int arr_enqueue(arr_queue_t *queue, request_t elem)
{
    if (is_arr_queue_full(queue))
        return QUEUE_OVERFLOW;

    queue->data[queue->tail] = elem;
    queue->tail = (queue->tail + 1) % MAX_QUEUE_SIZE;
    queue->size++;

    return EXIT_SUCCESS;
}

int arr_dequeque(arr_queue_t *queue, request_t *removed_elem)
{
    if (is_arr_queue_empty(queue))
        return QUEUE_UNDERFLOW;

    *removed_elem = queue->data[queue->head];
    queue->head = (queue->head + 1) % MAX_QUEUE_SIZE;
    queue->size--;
    return EXIT_SUCCESS;
}

void print_arr_queue(arr_queue_t *queue)
{
    for (int i = 0; i < queue->size; i++)
    {
        int index = (queue->head + i) % MAX_QUEUE_SIZE;
        printf("%d ", queue->data[index].id);
    }
}
