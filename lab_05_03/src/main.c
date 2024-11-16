#include "queue.h"
#include "queue_simulation.h"
#include "menu.h"
#include "timer.h"
#include "input_tools.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_QUEUE_SIZE 100

int main(void)
{
    int rc;

    arr_queue_t arr_queue;
    arr_queue_init(&arr_queue, MAX_QUEUE_SIZE);

    list_queue_t list_queue;
    list_queue_init(&list_queue, MAX_QUEUE_SIZE);

    queue_t queue1;
    queue_t queue2;

    double t1_start = 0.0, t1_end = 6.0;
    double t2_start = 1.0, t2_end = 8.0;

    double probability = 0.7;

    int action = -1;

    while (action != EXIT)
    {
        menu_print();
        if (read_int(&action) != EXIT_SUCCESS)
        {
            printf("Ошибка: Пожалуйста, введите число.\n");
            continue;
        }

        if (action == EXIT)
            break;

        request_t request;

        switch (action)
        {
            case EXIT:
                break;
            case ARR_QUEUE_PUSH:
                if (is_arr_queue_full(&arr_queue))
                {
                    printf("Ошибка добавления элемента. Очередь переполнена.\n");
                    break;
                }
                rc = EXIT_SUCCESS;
                do
                {
                    printf("Введите номер заявки, которую хотите добавить: ");
                    if (read_int(&request.id) != EXIT_SUCCESS)
                    {
                        rc = WRONG_VALUE_READ_ERROR;
                        printf("Ошибка ввода номера заявки. Попробуйте еще раз.\n");
                        continue;
                    }
                    rc = EXIT_SUCCESS;
                }
                while (rc != EXIT_SUCCESS);
                arr_enqueue(&arr_queue, request);
                break;
            case LIST_QUEUE_PUSH:
                if (is_list_queue_full(&list_queue))
                {
                    printf("Ошибка добавления элемента. Очередь переполнена.\n");
                    break;
                }
                rc = EXIT_SUCCESS;
                do
                {
                    printf("Введите номер заявки, которую хотите добавить: ");
                    if (read_int(&request.id) != EXIT_SUCCESS)
                    {
                        rc = WRONG_VALUE_READ_ERROR;
                        printf("Ошибка ввода номера заявки. Попробуйте еще раз.\n");
                        continue;
                    }
                    rc = EXIT_SUCCESS;
                }
                while (rc != EXIT_SUCCESS);
                list_enqueue(&list_queue, request);
                break;
            case ARR_QUEUE_POP:
                if (is_arr_queue_empty(&arr_queue))
                {
                    printf("Ошибка извлечения элемента. Очередь пуста\n");
                    break;
                }
                request_t elem = arr_dequeque(&arr_queue);
                printf("Номер извлеченной заявки: %d\n", elem.id);
                break;
            case LIST_QUEUE_POP:
                if (is_list_queue_empty(&list_queue))
                {
                    printf("Ошибка извлечения элемента. Очередь пуста\n");
                    break;
                }
                rc = list_dequeue(&list_queue, &elem);
                if (rc == EXIT_SUCCESS)
                    printf("Номер извлеченной заявки: %d\n", elem.id);
                break;
            case PRINT_ARR_QUEUE:
                if (is_arr_queue_empty(&arr_queue))
                {
                    printf("Выводить нечего. Очередь пуста\n");
                    break;
                }
                printf("Текущее состояние очереди:\n");
                print_arr_queue(&arr_queue);
                break;
            case PRINT_LIST_QUEUE:
                if (is_list_queue_empty(&list_queue))
                {
                    printf("Выводить нечего. Очередь пуста\n");
                    break;
                }
                printf("Текущее состояние очереди:\n");
                print_list_queue(&list_queue);
                break;
            case PRINT_FREE_LIST:
                if (list_queue.free_elems_size == 0)
                {
                    printf("Элементы не удалялись.\n");
                    break;
                }
                print_free_area(&list_queue);
                break;
            case MAKE_SIMULATION_ON_ARR:
                queue_init(&queue1, ARRAY_QUEUE, MAX_QUEUE_SIZE);
                queue_init(&queue2, ARRAY_QUEUE, MAX_QUEUE_SIZE);
                simulation_queue(&queue1, &queue2, t1_start, t1_end, t2_start, t2_end, probability);
                break;
            case MAKE_SIMULATION_ON_LIST:
                queue_init(&queue1, LIST_QUEUE, MAX_QUEUE_SIZE);
                queue_init(&queue2, LIST_QUEUE, MAX_QUEUE_SIZE);
                simulation_queue(&queue1, &queue2, t1_start, t1_end, t2_start, t2_end, probability);
                queue_free(&queue1);
                queue_free(&queue2);
                break;
            case CHANGE_PARAMETRS:
                rc = EXIT_SUCCESS;
                printf("Введите интервал обработки ОА1:\n");
                do
                {
                    printf("Введите начальное значение(не меньше нуля): ");
                    if (read_double(&t1_start) != EXIT_SUCCESS || t1_start < 0)
                    {
                        rc = WRONG_VALUE_READ_ERROR;
                        printf("Ошибка ввода времени. Попробуйте еще раз.\n");
                        continue;
                    }
                    rc = EXIT_SUCCESS;
                }
                while (rc != EXIT_SUCCESS);

                do
                {
                    printf("Введите конечное значение (не меньше начального): ");
                    if (read_double(&t1_end) != EXIT_SUCCESS || t1_start > t1_end)
                    {
                        rc = WRONG_VALUE_READ_ERROR;
                        printf("Ошибка ввода времени. Попробуйте еще раз.\n");
                        continue;
                    }
                    rc = EXIT_SUCCESS;
                }
                while (rc != EXIT_SUCCESS);

                printf("Введите интервал обработки ОА2:\n");
                do
                {
                    printf("Введите начальное значение(не меньше нуля): ");
                    if (read_double(&t2_start) != EXIT_SUCCESS || t2_start < 0)
                    {
                        rc = WRONG_VALUE_READ_ERROR;
                        printf("Ошибка ввода времени. Попробуйте еще раз.\n");
                        continue;
                    }
                    rc = EXIT_SUCCESS;
                }
                while (rc != EXIT_SUCCESS);

                do
                {
                    printf("Введите конечное значение(не меньше начального): ");
                    if (read_double(&t2_end) != EXIT_SUCCESS || t2_start > t2_end)
                    {
                        rc = WRONG_VALUE_READ_ERROR;
                        printf("Ошибка ввода времени. Попробуйте еще раз.\n");
                        continue;
                    }
                    rc = EXIT_SUCCESS;
                }
                while (rc != EXIT_SUCCESS);

                do
                {
                    printf("Введите вероятность попадания заявки обратно в первую очередь(от 0 до 1): ");
                    if (read_double(&probability) != EXIT_SUCCESS || probability <= 0 || probability > 1)
                    {
                        rc = WRONG_VALUE_READ_ERROR;
                        printf("Ошибка ввода вероятности. Попробуйте еще раз.\n");
                        continue;
                    }
                    rc = EXIT_SUCCESS;
                }
                while (rc != EXIT_SUCCESS);

                printf("Новые параметры:\n");
                printf("Интервал обработки ОА1: от %.2lf до %.2lf\n", t1_start, t1_end);
                printf("Интервал обработки ОА2: от %.2lf до %.2lf\n", t2_start, t2_end);
                printf("Вероятность попадания заявки вновь в первую очередь: %.2lf\n", probability);
                break;
            case PRINT_MEASUREMENTS:
                print_measurements();
                test_memory_fragmentation(10, 5, 5);
                break;
            default:
                printf("Выберите 1 из пунктов меню.\n");
                break;
        }
    }
    free_list_queue(&list_queue);
    return EXIT_SUCCESS;
}
