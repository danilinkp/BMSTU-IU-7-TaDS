#include "list_stack.h"
#include <stdlib.h>
#include <stdio.h>

void init(list_stack_t *stack, int max_size)
{
    stack->ps = NULL;
    stack->size = 0;
    stack->max_size = max_size;

    stack->free_list = NULL;
    stack->free_elems_size = 0;
}

int is_empty(list_stack_t *stack)
{
    return stack->ps == NULL;
}

int is_full(list_stack_t *stack)
{
    return stack->size >= stack->max_size;
}

int push(list_stack_t *stack, int value)
{
    if (is_full(stack))
    {
        printf("Ошибка добавления элемента. Стек переполнен\n");
        return STACK_OVERFLOW;
    }

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

int pop(list_stack_t *stack, int *value)
{
    if (is_empty(stack))
    {
        printf("Ошибка извлечения элемента. Стек пуст.\n");
        return STACK_UNDERFLOW;
    }

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

int peak(list_stack_t *stack, int *value)
{
    if (is_empty(stack))
    {
        printf("Ошибка извлечения элемента. Стек пуст.\n");
        return STACK_UNDERFLOW;
    }

    *value = stack->ps->data;

    return EXIT_SUCCESS;
}

void print_list_stack(list_stack_t *stack)
{
    list_stack_t tmp_stack;
    init(&tmp_stack, stack->max_size);

    int tmp_value;
    while (!(is_empty(stack)))
    {
        pop(stack, &tmp_value);
        printf("Адресс элемента - %p, значение элемента - %d\n", (void *) stack->ps, tmp_value);
        push(&tmp_stack, tmp_value);
    }
    printf("\n");
    while (!is_empty(&tmp_stack))
    {
        pop(&tmp_stack, &tmp_value);
        push(stack, tmp_value);
    }
}

void print_free_list(list_stack_t *stack)
{
    printf("Список свободных областей:\n");
    free_node_t *current = stack->free_list;
    for (; current; current = current->next)
        printf("Освобожденная область памяти: %p\n", stack->free_list->address);
}
