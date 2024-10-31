#include "list_stack.h"
#include <stdlib.h>
#include <stdio.h>

void list_stack_init(list_stack_t *stack, int max_size)
{
    stack->ps = NULL;
    stack->size = 0;
    stack->max_size = max_size;

    stack->free_list = NULL;
    stack->free_elems_size = 0;
}

int is_list_stack_empty(list_stack_t *stack)
{
    return stack->ps == NULL;
}

int is_list_stack_full(list_stack_t *stack)
{
    return stack->size >= stack->max_size;
}

int list_stack_push(list_stack_t *stack, int value)
{
    node_t *new_elem = malloc(sizeof(node_t));
    if (!new_elem)
    {
        printf("Ошибка выделения памяти под узел. Возможно стек переполнен.\n");
        return NODE_ALLOC_ERROR;
    }

    new_elem->data = value;
    new_elem->next = stack->ps;
    stack->size++;

    stack->ps = new_elem;

    return EXIT_SUCCESS;
}

int list_stack_pop(list_stack_t *stack, int *value)
{
    *value = stack->ps->data;
    node_t *next_node = stack->ps->next;
    stack->size--;

    free_node_t *free_node = malloc((sizeof(free_node_t)));
    if (!free_node)
    {
        printf("Ошибка выделения памяти под узел списка освобожденной памяти.\n");
        return FREE_NODE_ALLOC_ERROR;
    }
    free_node->address = stack->ps;
    free_node->next = stack->free_list;
    stack->free_list = free_node;
    stack->free_elems_size++;

    free(stack->ps);

    stack->ps = next_node;

    return EXIT_SUCCESS;
}

int list_stack_peek(list_stack_t *stack)
{
    return stack->ps->data;
}

void print_list_stack(list_stack_t *stack)
{
    list_stack_t tmp_stack;
    list_stack_init(&tmp_stack, stack->max_size);

    int tmp_value;
    while (!(is_list_stack_empty(stack)))
    {
        list_stack_pop(stack, &tmp_value);
        printf("Адресс элемента - %p, значение элемента - %d\n", (void *) stack->ps, tmp_value);
        list_stack_push(&tmp_stack, tmp_value);
    }
    printf("\n");
    while (!is_list_stack_empty(&tmp_stack))
    {
        list_stack_pop(&tmp_stack, &tmp_value);
        list_stack_push(stack, tmp_value);
    }
    free_list_stack(&tmp_stack);
}

void print_free_list(list_stack_t *stack)
{
    printf("Список свободных областей:\n");
    free_node_t *current = stack->free_list;
    for (; current; current = current->next)
        printf("Освобожденная область памяти: %p\n", stack->free_list->address);
}

void free_list_stack(list_stack_t *stack)
{
    node_t *next;
    for (; stack->ps; stack->ps = next)
    {
        next = stack->ps->next;
        free(stack->ps);
    }

    free_node_t *next_free;
    for (; stack->free_list; stack->free_list = next_free)
    {
        next_free = stack->free_list->next;
        free(stack->free_list);
    }
}
