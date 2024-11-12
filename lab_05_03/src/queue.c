#include "queue.h"
#include <stdlib.h>

void queue_init(queue_t *queue, queue_type_t queue_type, int max_size)
{
    queue->type = queue_type;
    if (queue_type == ARRAY_QUEUE)
        arr_queue_init(&(queue->arr_queue), max_size);
    else if (queue_type == LIST_QUEUE)
        list_queue_init(&(queue->list_queue), max_size);
}

int is_queue_empty(queue_t *queue)
{
    if (queue->type == ARRAY_QUEUE)
        return is_arr_queue_empty(&(queue->arr_queue));
    return is_list_queue_empty(&(queue->list_queue));
}

int is_queue_full(queue_t *queue)
{
    if (queue->type == ARRAY_QUEUE)
        return is_arr_queue_full(&(queue->arr_queue));
    return is_list_queue_full(&(queue->list_queue));
}

int enqueue(queue_t *queue, request_t elem)
{
    if (is_queue_full(queue))
        return QUEUE_OVERFLOW;
    if (queue->type == ARRAY_QUEUE)
        arr_enqueue(&(queue->arr_queue), elem);
    else if (queue->type == LIST_QUEUE)
    {
        int rc = list_enqueue(&(queue->list_queue), elem);
        if (rc != EXIT_SUCCESS)
            return rc;
    }

    return EXIT_SUCCESS;
}

int dequeue(queue_t *queue, request_t *removed_elem)
{
    if (is_queue_empty(queue))
        return QUEUE_UNDERFLOW;

    if (queue->type == ARRAY_QUEUE)
        *removed_elem = arr_dequeque(&(queue->arr_queue));
    else if (queue->type == LIST_QUEUE)
    {
        int rc = list_dequeue(&(queue->list_queue), removed_elem);
        if (rc != EXIT_SUCCESS)
            return rc;
    }
    return EXIT_SUCCESS;
}

void print_queue(queue_t *queue)
{
    if (queue->type == ARRAY_QUEUE)
        print_arr_queue(&(queue->arr_queue));
    else if (queue->type == LIST_QUEUE)
        print_list_queue(&(queue->list_queue));
}

void queue_print_free_area(queue_t *queue)
{
    if (queue->type == LIST_QUEUE)
        print_free_area(&(queue->list_queue));
}

void queue_free(queue_t *queue)
{
    if (queue->type == LIST_QUEUE)
        free_queue(&(queue->list_queue));
}

