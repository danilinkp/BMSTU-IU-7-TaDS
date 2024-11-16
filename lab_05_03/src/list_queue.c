#include "list_queue.h"
#include <stdlib.h>
#include <stdio.h>

enum mem_errors
{
    NODE_ALLOC_ERROR = 3,
    FREE_NODE_ALLOC_ERROR,
};

void list_queue_init(list_queue_t *queue, int max_size)
{
    queue->head = NULL;
    queue->tail = NULL;
    queue->size = 0;
    queue->max_size = max_size;

    queue->free_list = NULL;
    queue->free_elems_size = 0;
}

int is_list_queue_empty(list_queue_t *queue)
{
    return queue->size == 0;
}

int is_list_queue_full(list_queue_t *queue)
{
    return queue->size == queue->max_size;
}

int list_enqueue(list_queue_t *queue, request_t elem)
{
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

void print_list_queue(list_queue_t *queue)
{
    node_t *curr = queue->head;
    for (; curr; curr = curr->next)
    {
        printf("Адресс элемента - %p, ", (void *) curr);
        printf("значение элемента - %d\n", curr->data.id);
    }
}

void print_free_area(list_queue_t *queue)
{
    printf("Список свободных областей:\n");
    free_node_t *current = queue->free_list;
    for (; current; current = current->next)
        printf("Освобожденная область памяти: %p\n", current->address);
}

void free_list_queue(list_queue_t *queue)
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

void test_memory_fragmentation(int initial_fill, int remove_count, int additional_fill)
{
    list_queue_t queue;
    list_queue_init(&queue, initial_fill + additional_fill);

    // Шаг 1: Заполняем очередь начальными элементами
    for (int i = 0; i < initial_fill; i++)
    {
        request_t req;
        req.id = i;
        list_enqueue(&queue, req);
    }

    printf("Очередь после начального заполнения:\n");
    print_list_queue(&queue);
    printf("\n");

    request_t removed;
    for (int i = 0; i < remove_count; i++)
    {
        list_dequeue(&queue, &removed);
    }

    printf("Очередь после удаления %d элементов:\n", remove_count);
    print_list_queue(&queue);
    printf("Список освобожденных областей после удаления:\n");
    print_free_area(&queue);
    printf("\n");

    for (int i = initial_fill; i < initial_fill + additional_fill; i++)
    {
        request_t req;
        req.id = i;
        list_enqueue(&queue, req);
    }

    printf("Очередь после добавления %d новых элементов:\n", additional_fill);
    print_list_queue(&queue);
    printf("Список освобожденных областей после добавления:\n");
    print_free_area(&queue);
    printf("\n");

    free_list_queue(&queue);
}
