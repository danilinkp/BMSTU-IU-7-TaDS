#include "stack.h"

void arr_stack_print_sequence(FILE *file, array_stack_t *stack)
{
    array_stack_t tmp_stack;
    arr_stack_init(&tmp_stack, stack->max_size);
    fprintf(file, "Убывающие серии последовательности в обратном порядке:\n");

    int pop_elem = arr_stack_pop(stack);

    if (is_arr_stack_empty(stack))
        fprintf(file, "%d\n", pop_elem);
    arr_stack_push(&tmp_stack, pop_elem);

//    array_stack_t result;
//    arr_stack_init(&result, stack->max_size);
    int tmp_elem;

    int cur_len = 0;
    while (!is_arr_stack_empty(stack))
    {
        tmp_elem = arr_stack_pop(stack);
        if (pop_elem < tmp_elem)
        {
            if (cur_len == 0)
            {
                fprintf(file, "%d %d ", pop_elem, tmp_elem);
                cur_len += 2;
            }
            else
            {
                fprintf(file, "%d ", tmp_elem);
                cur_len++;
            }
        }
        else
        {
            if (cur_len != 0)
            {
                fprintf(file, "\n");
                cur_len = 0;
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

void list_stack_print_sequence(FILE *file, list_stack_t *stack)
{
    list_stack_t tmp_stack;
    list_stack_init(&tmp_stack, stack->max_size);
    fprintf(file, "Убывающие серии последовательности в обратном порядке:\n");

    int pop_elem;
    list_stack_pop(stack, &pop_elem);

    if (is_list_stack_empty(stack))
        fprintf(file, "%d\n", pop_elem);
    list_stack_push(&tmp_stack, pop_elem);

//    list_stack_t result;
//    list_stack_init(&result, stack->max_size);

    int cur_len = 0;
    while (!is_list_stack_empty(stack))
    {
        int tmp_elem;
        list_stack_pop(stack, &tmp_elem);
        if (pop_elem < tmp_elem)
        {
            if (cur_len == 0)
            {
                fprintf(file, "%d %d ", pop_elem, tmp_elem);
                cur_len += 2;
            }
            else
            {
                fprintf(file, "%d ", tmp_elem);
                cur_len++;
            }
        }
        else
        {
            if (cur_len != 0)
            {
                fprintf(file, "\n");
                cur_len = 0;
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
