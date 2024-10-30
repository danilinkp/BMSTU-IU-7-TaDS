#include "array_stack.h"
#include <stdio.h>
#include <stdlib.h>

void init(array_stack_t *stack)
{
    stack->ps = -1;
}

int is_empty(array_stack_t *stack)
{
    return stack->ps == -1;
}

int is_full(array_stack_t *stack)
{
    return stack->ps == MAX_STACK_SIZE - 1;
}

int push(array_stack_t *stack, const int value)
{
    if (is_full(stack))
    {
        printf("Ошибка добавления элемента. Стек переполен.\n");
        return STACK_OVERFLOW;
    }
    stack->data[++stack->ps] = value;
    return EXIT_SUCCESS;
}

int pop(array_stack_t *stack, int *value)
{
    if (is_empty(stack))
    {
        printf("Ошибка удаления элемента. Стек пуст.\n");
        return STACK_UNDERFLOW;
    }
    *value = stack->data[stack->ps--];
    return EXIT_SUCCESS;
}

int peek(array_stack_t *stack, int *value)
{
    if (is_empty(stack))
    {
        printf("Ошибка получения верхнего элемента стека. Стек пуст.\n");
        return STACK_UNDERFLOW;
    }
    *value = stack->data[stack->ps];
    return EXIT_SUCCESS;
}

void print_stack(array_stack_t *stack)
{
    array_stack_t tmp_stack;
    init(&tmp_stack);
    int tmp_value;

    while (!(is_empty(stack)))
    {
         pop(stack, &tmp_value);
         printf("%d ", tmp_value);
         push(&tmp_stack, tmp_value);
    }
    printf("\n");
    while (!is_empty(&tmp_stack))
    {
        pop(&tmp_stack, &tmp_value);
        push(stack, tmp_value);
    }
}
