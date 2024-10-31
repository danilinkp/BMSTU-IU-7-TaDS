#include "stack.h"
#include <stdio.h>


void arr_stack_print_sequence(array_stack_t *stack)
{
    array_stack_t tmp_stack;
    arr_stack_init(&tmp_stack, stack->max_size);
    printf("Убывающие серии последовательности в обратном порядке:\n");

    int pop_elem = arr_stack_pop(stack);

    if (is_arr_stack_empty(stack))
        printf("%d\n", pop_elem);
    arr_stack_push(&tmp_stack, pop_elem);

    array_stack_t result;
    arr_stack_init(&result, stack->max_size);
    int tmp_elem;
    while (!is_arr_stack_empty(stack))
    {
        tmp_elem = arr_stack_pop(stack);
        if (pop_elem > tmp_elem)
        {
            if (is_arr_stack_empty(&result))
            {
                arr_stack_push(&result, pop_elem);
                arr_stack_push(&result, tmp_elem);
            }
            else
                arr_stack_push(&result, tmp_elem);
        }
        else
        {
            if (!is_arr_stack_empty(&result))
            {
                print_arr_stack(&result);
                arr_stack_init(&result);
            }
        }
        arr_stack_push(&tmp_stack, tmp_elem);
        pop_elem = tmp_elem;
    }

    // Возвращаем список в исходное положение
    int tmp_stack_value;
    while (!is_arr_stack_empty(&tmp_stack))
    {
        tmp_stack_value = arr_stack_pop(&tmp_stack);
        arr_stack_push(stack, tmp_stack_value);
    }
}


void list_stack_print_sequence(list_stack_t *stack)
{
    list_stack_t tmp_stack;
    list_stack_init(&tmp_stack, stack->max_size);
    printf("Убывающие серии последовательности в обратном порядке:\n");

    int pop_elem;
    list_stack_pop(stack, &pop_elem);

    if (is_list_stack_empty(stack))
        printf("%d\n", pop_elem);
    list_stack_push(&tmp_stack, pop_elem);

    list_stack_t result;
    list_stack_init(&result, stack->max_size);

    while (!is_list_stack_empty(stack))
    {
        int tmp_elem;
        list_stack_pop(stack, &tmp_elem);
        if (pop_elem > tmp_elem)
        {
            if (is_list_stack_empty(&result))
            {
                list_stack_push(&result, pop_elem);
                list_stack_push(&result, tmp_elem);
            }
            else
                list_stack_push(&result, tmp_elem);
        }
        else
        {
            if (!is_list_stack_empty(&result))
            {
                print_list_stack(&result);
                free_list_stack(&result);
                list_stack_init(&result);
            }
        }
        list_stack_push(&tmp_stack, tmp_elem);
        pop_elem = tmp_elem;
    }

    // Возвращаем список в исходное положение
    int tmp_stack_value;
    while (!is_list_stack_empty(&tmp_stack))
    {
        list_stack_pop(&tmp_stack, &tmp_stack_value);
        list_stack_push(stack, tmp_stack_value);
    }

    free_list_stack(&tmp_stack);
}
