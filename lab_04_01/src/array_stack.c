#include "array_stack.h"
#include <stdio.h>
#include <stdlib.h>

void arr_stack_init(array_stack_t *stack, int max_size)
{
    stack->ps = -1;
    stack->max_size = max_size;
}

int is_arr_stack_empty(array_stack_t *stack)
{
    return stack->ps == -1;
}

int is_arr_stack_full(array_stack_t *stack)
{
    return stack->ps == MAX_STACK_SIZE - 1;
}

void arr_stack_push(array_stack_t *stack, const int value)
{
    stack->data[++stack->ps] = value;
}

int arr_stack_pop(array_stack_t *stack)
{
    return stack->data[stack->ps--];
}

int arr_stack_peek(array_stack_t *stack)
{
    return stack->data[stack->ps];
}

void print_arr_stack(array_stack_t *stack)
{
    array_stack_t tmp_stack;
    arr_stack_init(&tmp_stack, stack->max_size);
    int tmp_value;

    while (!(is_arr_stack_empty(stack)))
    {
        tmp_value = arr_stack_pop(stack);
        printf("%d ", tmp_value);
        arr_stack_push(&tmp_stack, tmp_value);
    }
    printf("\n");
    while (!is_arr_stack_empty(&tmp_stack))
    {
        tmp_value = arr_stack_pop(&tmp_stack);
        arr_stack_push(stack, tmp_value);
    }
}
