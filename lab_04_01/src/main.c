#include "stack.h"
#include "menu.h"
#include "timer.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_STACK_SIZE 20


int main(void)
{
    int rc;

    array_stack_t arr_stack;
    arr_stack_init(&arr_stack, MAX_STACK_SIZE);

    list_stack_t list_stack;
    list_stack_init(&list_stack, MAX_STACK_SIZE);

    int action = -1;

    while (action != EXIT)
    {
        menu_print();
        if (scanf("%d", &action) != 1)
        {
            printf("Ошибка: Пожалуйста, введите число.\n");
            continue;
        }

        if (action == EXIT)
            break;

        int value;

        switch (action)
        {
            case EXIT:
                break;
            case ARR_STACK_PUSH:
                if (is_arr_stack_full(&arr_stack))
                {
                    printf("Ошибка добавления элемента. Стек переполнен\n");
                    break;
                }
                rc = EXIT_SUCCESS;
                do
                {
                    printf("Введите число, которое хотите добавить в стек: ");
                    if (scanf("%d", &value) != 1)
                    {
                        rc = WRONG_VALUE_READ_ERROR;
                        printf("Ошибка ввода элемента. Попробуйте еще раз.\n");
                        continue;
                    }
                    rc = EXIT_SUCCESS;
                }
                while (rc != EXIT_SUCCESS);
                arr_stack_push(&arr_stack, value);
                break;
            case LIST_STACK_PUSH:
                if (is_list_stack_full(&list_stack))
                {
                    printf("Ошибка добавления элемента. Стек переполнен\n");
                    break;
                }
                rc = EXIT_SUCCESS;
                do
                {
                    printf("Введите число, которое хотите добавить в стек: ");
                    if (scanf("%d", &value) != 1)
                    {
                        rc = WRONG_VALUE_READ_ERROR;
                        printf("Ошибка ввода элемента. Попробуйте еще раз.\n");
                        continue;
                    }
                    rc = EXIT_SUCCESS;
                }
                while (rc != EXIT_SUCCESS);
                list_stack_push(&list_stack, value);
                break;
            case ARR_STACK_POP:
                if (is_arr_stack_empty(&arr_stack))
                {
                    printf("Ошибка удаления элемента. Стек пуст\n");
                    break;
                }
                int elem = arr_stack_pop(&arr_stack);
                printf("Извлеченный элемент: %d", elem);
                break;
            case LIST_STACK_POP:
                if (is_list_stack_empty(&list_stack))
                {
                    printf("Ошибка удаления элемента. Стек пуст\n");
                    break;
                }
                rc = list_stack_pop(&list_stack, &elem);
                if (rc == EXIT_SUCCESS)
                    printf("Извлеченный элемент: %d", elem);
                break;
            case PRINT_ARR_STACK:
                if (is_arr_stack_empty(&arr_stack))
                {
                    printf("Выводить нечего. Стек пуст.\n");
                    break;
                }
                printf("Текущее состояние стека:\n");
                print_arr_stack(&arr_stack);
                break;
            case PRINT_LIST_STACK:
                if (is_list_stack_empty(&list_stack))
                {
                    printf("Выводить нечего. Стек пуст.\n");
                    break;
                }
                printf("Текущее состояние стека:\n");
                print_list_stack(&list_stack);
                break;
            case PRINT_FREE_LIST:
                if (list_stack.free_elems_size == 0)
                {
                    printf("Выводить нечего. Элементы не удалялись\n");
                    break;
                }
                print_free_list(&list_stack);
                break;
            case PRINT_SEQUENCE_ON_ARR:
                if (is_arr_stack_empty(&arr_stack))
                {
                    printf("Выводить нечего. Стек пуст.\n");
                    break;
                }
                arr_stack_print_sequence(&arr_stack);
                break;
            case PRINT_SEQUENCE_ON_LIST:
                if (is_list_stack_empty(&list_stack))
                {
                    printf("Выводить нечего. Стек пуст.\n");
                    break;
                }
                list_stack_print_sequence(&list_stack);
                break;
            case PRINT_MEASUREMENTS:
                print_measurements();
                break;
            default:
                printf("Выберите 1 из пунктов меню.\n");
                break;
        }
    }

    return EXIT_SUCCESS;
}
