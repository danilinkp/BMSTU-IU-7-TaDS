#include "queue_simulation.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define TOTAL_REQUESTS 1000
#define INITIAL_REQUESTS 100
#define EPS 1e-8

/// Определение типов событий
typedef enum
{
    ARRIVAL,
    OA1_COMPLETION,
    OA2_COMPLETION
} event_type_t;

// Функция для генерации случайного времени в заданном диапазоне
static double generate_random_time(double min_time, double max_time)
{
    return (max_time - min_time) * ((double) rand() / RAND_MAX) + min_time;
}

// Основная функция симуляции
void simulation_queue(queue_t *queue1, queue_t *queue2, double t1_start, double t1_end,
                      double t2_start, double t2_end, double probability)
{
    int processed_in_oa2 = 0;     // Количество заявок, вышедших из ОА2
    int processed_in_oa1 = 0;     // Количество заявок, обслуженных ОА1
    double idle_time_oa2 = 0.0;   // Время простоя ОА2
    double total_time = 0.0;      // Общее время моделирования

    double next_arrival_time = generate_random_time(t1_start, t1_end); // Время для следующей новой заявки
    double oa1_end_time = 0.0;    // Время завершения работы в ОА1
    double oa2_end_time = 0.0;    // Время завершения работы в ОА2

    // Средние времена для расчетного времени
    double avg_arrival_time = (t1_start + t1_end) / 2;
    double avg_processing_time_oa2 = (t2_start + t2_end) / 2;

    // Для расчета средней длины очереди
    double cumulative_queue1_length = 0.0;
    double cumulative_queue2_length = 0.0;
    int snapshots = 0;

    srand(time(NULL)); // Инициализация генератора случайных чисел

    // Начальное заполнение первой очереди заявками
    for (int i = 0; i < INITIAL_REQUESTS; i++)
    {
        request_t initial_request = { .id = i + 1, .processing_time = 0 };
        enqueue(queue1, initial_request);
    }

    // Основной цикл симуляции
    while (processed_in_oa2 < TOTAL_REQUESTS)
    {
        // Определяем текущее время как минимальное из всех запланированных событий
        total_time = (next_arrival_time < oa1_end_time && next_arrival_time < oa2_end_time) ? next_arrival_time
                                                                                            : (oa1_end_time
                                                                                               < oa2_end_time
                                                                                               ? oa1_end_time
                                                                                               : oa2_end_time);

        // Обработка событий, происходящих в один и тот же момент времени

        // 1. Если событие завершения обработки заявки в ОА2 происходит в текущее время
        if (fabs(total_time - oa2_end_time) < EPS && !is_queue_empty(queue2))
        {
            request_t request;
            dequeue(queue2, &request);
            request.processing_time = generate_random_time(t2_start, t2_end); // Время обработки в ОА2
            oa2_end_time = total_time + request.processing_time;
            processed_in_oa2++;
        }
        else if (is_queue_empty(queue2))
        {
            idle_time_oa2 += total_time - oa2_end_time;
            oa2_end_time = total_time;
        }

        // 2. Если событие завершения обработки заявки в ОА1 происходит в текущее время
        if (fabs(total_time - oa1_end_time) < EPS && !is_queue_empty(queue1))
        {
            request_t request;
            dequeue(queue1, &request);
            request.processing_time = generate_random_time(t1_start, t1_end); // Время обработки в ОА1
            oa1_end_time = total_time + request.processing_time;
            processed_in_oa1++;

            // Переход в очередь 2 с вероятностью 1 - P
            if (((double) rand() / RAND_MAX) > probability)
                enqueue(queue2, request);
            else
                enqueue(queue1, request); // Повторное добавление в очередь 1
        }

        // 3. Если событие прихода новой заявки происходит в текущее время
        if (fabs(total_time - next_arrival_time) < EPS
            && processed_in_oa2 + processed_in_oa1 < TOTAL_REQUESTS + INITIAL_REQUESTS)
        {
            request_t new_request = { .id = processed_in_oa2 + processed_in_oa1 + 1, .processing_time = 0 };
            enqueue(queue1, new_request);
            next_arrival_time += generate_random_time(t1_start, t1_end); // Время следующего прихода
        }

        // Сбор данных о длине очередей для среднего значения
        cumulative_queue1_length += queue_length(queue1);
        cumulative_queue2_length += queue_length(queue2);
        snapshots++;

        // Периодический вывод информации после каждых 100 заявок в ОА2
        if (processed_in_oa2 % 100 == 0 && processed_in_oa2 > 0)
        {
            printf("Обработано в ОА2: %d заявок\n", processed_in_oa2);
            printf("Текущая длина очереди 1: %d\n", queue_length(queue1));
            printf("Средняя длина очереди 1: %.2f\n", cumulative_queue1_length / snapshots);
            printf("Текущая длина очереди 2: %d\n", queue_length(queue2));
            printf("Средняя длина очереди 2: %.2f\n\n", cumulative_queue2_length / snapshots);
        }
    }

    // Итоговые результаты
    printf("\nИтоговые результаты:\n");
    printf("Общее время моделирования: %.2f\n", total_time);
    printf("Время простоя ОА2: %.2f\n", idle_time_oa2);
    printf("Количество обработанных заявок в ОА1: %d\n", processed_in_oa1);
    printf("Средняя длина очереди 1: %.2f\n", cumulative_queue1_length / snapshots);
    printf("Средняя длина очереди 2: %.2f\n", cumulative_queue2_length / snapshots);

    // Расчетное время моделирования
    double expected_time_arrival = avg_arrival_time * (INITIAL_REQUESTS + processed_in_oa1);
    double expected_time_processing = avg_processing_time_oa2 * processed_in_oa2;

    // Процент погрешности
    double error_percentage_arrival = ((total_time - expected_time_arrival) / expected_time_arrival) * 100;
    double error_percentage_processing = ((total_time - expected_time_processing) / expected_time_processing) * 100;

    printf("\nПроцент погрешности по приходу заявок: %.2f%%\n", error_percentage_arrival);
    printf("Процент погрешности по обработке заявок: %.2f%%\n", error_percentage_processing);
}
