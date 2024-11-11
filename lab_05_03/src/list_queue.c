#include "list_queue.h"
#include <stdlib.h>
#include <stdio.h>

void list_queue_init(list_queue_t *queue)
{
    queue->head = NULL;
    queue->tail = NULL;
    queue->size = 0;

    queue->free_list = NULL;
    queue->free_elems_size = 0;
}

int is_list_queue_empty(list_queue_t *queue)
{
    return queue->size == 0;
}

int is_list_queue_full(list_queue_t *queue)
{
    return queue->size == MAX_QUEUE_SIZE;
}

int list_enqueue(list_queue_t *queue, request_t elem)
{
    if (is_list_queue_full(queue))
        return QUEUE_OVERFLOW;

    node_t *new_elem = malloc(sizeof(node_t));
    if (!new_elem)
    {
        printf("Ошибка выделения памяти под узел. Возможно стек переполнен.\n");
        return NODE_ALLOC_ERROR;
    }

    new_elem->data = elem;
    new_elem->next = NULL;

    if (queue->size == 0)
    {
        queue->tail = new_elem;
        queue->head = new_elem;
    }
    else
    {
        queue->tail->next = new_elem;
        queue->tail = new_elem;
    }

    queue->size++;

    return EXIT_SUCCESS;
}

int list_dequeue(list_queue_t *queue, request_t *removed_elem)
{
    if (is_list_queue_empty(queue))
        return QUEUE_UNDERFLOW;

    *removed_elem = queue->head->data;
    node_t *next_elem = queue->head->next;

    free_node_t *free_node = malloc((sizeof(free_node_t)));
    if (!free_node)
    {
        printf("Ошибка выделения памяти под узел списка освобожденной памяти.\n");
        return FREE_NODE_ALLOC_ERROR;
    }

    free_node->address = (void *) queue->head;
    free_node->next = queue->free_list;
    queue->free_list = free_node;
    queue->free_elems_size++;

    free(queue->head);

    queue->size--;
    queue->head = next_elem;

    if (queue->size == 0)
        queue->tail = NULL;

    return EXIT_SUCCESS;
}

void list_queue_print(list_queue_t *queue)
{
    node_t *curr = queue->head;
    for (; curr; curr = curr->next)
    {
        printf("Адресс элемента - %p, ", (void *) curr);
        printf("значение элемента - %d\n", curr->data.id);
    }
}

void free_queue(list_queue_t *queue)
{
    node_t *next;
    for (; queue->head; queue->head = next)
    {
        next = queue->head->next;
        free(queue->head);
    }

    free_node_t *next_free;
    for (; queue->free_list; queue->free_list = next_free)
    {
        next_free = queue->free_list->next;
        free(queue->free_list);
    }
}
