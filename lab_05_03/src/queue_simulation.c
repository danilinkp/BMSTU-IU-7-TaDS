#include "queue_simulation.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static double calculate_theoretical_time(int oa2_activations, int oa1_requests, double t1_start, double t1_end,
                                         double t2_start, double t2_end)
{
    double avg_t1 = (t1_start + t1_end) / 2.0;
    double avg_t2 = (t2_start + t2_end) / 2.0;

    double time_oa1 = avg_t1 * oa1_requests;
    double time_oa2 = avg_t2 * oa2_activations;

    return (time_oa1 > time_oa2) ? time_oa1 : time_oa2;
}

void simulation_queue(queue_t *queue1, queue_t *queue2,
                      double t1_start, double t1_end,
                      double t2_start, double t2_end,
                      double probability)
{

    service_unit_t oa1 = { .is_busy = 0, .end_time = 0.0 };
    service_unit_t oa2 = { .is_busy = 0, .end_time = 0.0 };

    double current_time = 0.0;
    int processed_requests = 0;
    int oa1_activations = 0;
    double total_oa2_idle_time = 0.0;
    double queue_stats[2] = { 0.0, 0.0 };
    int stat_points = 0;

    // Инициализация первой очереди
    for (int i = 0; i < 100; i++)
    {
        request_t req = { i, current_time };
        enqueue(queue1, req);
    }

    while (processed_requests < 1000)
    {
        // Обработка первого аппарата.
        if (!oa1.is_busy && !is_queue_empty(queue1))
        {
            request_t req;
            dequeue(queue1, &req);
            oa1.is_busy = 1;
            oa1.current_request = req;
            double service_time = t1_start + ((double) rand() / RAND_MAX) * (t1_end - t1_start);
            oa1.end_time = current_time + service_time;
            oa1_activations++;
        }

        // Обработка второго аппрата.
        if (!oa2.is_busy && !is_queue_empty(queue2))
        {
            request_t req;
            dequeue(queue2, &req);
            oa2.is_busy = 1;
            oa2.current_request = req;
            double service_time = t2_start + ((double) rand() / RAND_MAX) * (t2_end - t2_start);
            oa2.end_time = current_time + service_time;
        }

        // Находим ближайшее время завершения обработки
        double next_time = 0.0;
        int next_unit = 0; // 1 - ОА1, 2 - ОА2

        if (oa1.is_busy && oa2.is_busy)
        {
            if (oa1.end_time <= oa2.end_time)
            {
                next_time = oa1.end_time;
                next_unit = 1;
            }
            else
            {
                next_time = oa2.end_time;
                next_unit = 2;
            }
        }
        else if (oa1.is_busy)
        {
            next_time = oa1.end_time;
            next_unit = 1;
        }
        else if (oa2.is_busy)
        {
            next_time = oa2.end_time;
            next_unit = 2;
        }

        if (!oa2.is_busy)
            total_oa2_idle_time += (next_time - current_time);
        current_time = next_time;

        if (next_unit == 1)
        {
            if (((double) rand() / RAND_MAX) < probability)
                enqueue(queue1, oa1.current_request);
            else
                enqueue(queue2, oa1.current_request);
            oa1.is_busy = 0;
        }
        else
        {
            enqueue(queue1, oa2.current_request);
            oa2.is_busy = 0;
            processed_requests++;

            if (processed_requests % 100 == 0)
            {
                queue_stats[0] += queue_length(queue1);
                queue_stats[1] += queue_length(queue2);
                stat_points++;
                printf("Обработано заявок во втором аппарате: %d\n", processed_requests);
                printf("Текущая длины очереди: Q1 = %d, Q2 = %d\n",
                       queue_length(queue1), queue_length(queue2));
                printf("Средние длины очередей: Q1 = %.2f, Q2= %.2f\n",
                       queue_stats[0] / stat_points,
                       queue_stats[1] / stat_points);
            }
        }
    }
    double theoretical_time = calculate_theoretical_time(1000, oa1_activations,
                                                         t1_start, t1_end,
                                                         t2_start, t2_end);

    double time_difference = fabs(current_time - theoretical_time);
    double difference_percent = (time_difference / theoretical_time) * 100.0;

    printf("\nИтоги симуляции:\n");
    printf("Общее время работы: %.2f\n", current_time);
    printf("Теоретический рассчёт: %.2f\n", theoretical_time);
    printf("Процент погрешности: %.2f%%\n", difference_percent);
    printf("Время простоя ОА2: %.2f\n", total_oa2_idle_time);
    printf("Количество срабатываний ОА1: %d\n", oa1_activations);
    printf("Средняя длина первой очереди %.2f\n", queue_stats[0] / stat_points);
    printf("Средняя длина второй очереди: %.2f\n", queue_stats[1] / stat_points);
}
