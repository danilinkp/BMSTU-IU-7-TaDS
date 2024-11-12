#include "queue_simulation.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_REQUESTS 1000
#define P 0.7 // Вероятность возврата заявки в первую очередь после ОА1

// Функция для генерации случайного времени в диапазоне [min, max]
static double generate_time(double min, double max)
{
    return min + (max - min) * ((double) rand() / RAND_MAX);
}

void
simulation_queue(queue_t *queue1, queue_t *queue2, double t1_start, double t1_end, double t2_start, double t2_end)
{
    int serviced_requests_in_OA2 = 0;
    int OA1_activations = 0;
    double total_simulation_time = 0.0;
    double OA2_idle_time = 0.0;
    double last_OA2_service_time = 0.0;

    srand(time(NULL));

    // Заполнение первой очереди начальными заявками
    for (int i = 0; i < 100; i++)
    {
        request_t req = { .id = i, .processing_time = 0.0 };
        enqueue(queue1, req);
    }

    while (serviced_requests_in_OA2 < NUM_REQUESTS)
    {
        // Проверяем очередь 1 и обрабатываем в ОА1
        if (!is_queue_empty(queue1))
        {
            request_t req;
            dequeue(queue1, &req);
            double service_time_OA1 = generate_time(t1_start, t1_end);
            total_simulation_time += service_time_OA1;
            OA1_activations++;

            // Определяем, куда отправить заявку после ОА1
            if ((double) rand() / RAND_MAX < P)
            {
                enqueue(queue1, req); // Возврат в первую очередь
            }
            else
            {
                enqueue(queue2, req); // Переход во вторую очередь
            }
        }

        // Проверяем очередь 2 и обрабатываем в ОА2
        if (!is_queue_empty(queue2))
        {
            request_t req;
            dequeue(queue2, &req);
            double service_time_OA2 = generate_time(t2_start, t2_end);
            total_simulation_time += service_time_OA2;

            // Считаем время простоя ОА2
            if (total_simulation_time > last_OA2_service_time)
            {
                OA2_idle_time += total_simulation_time - last_OA2_service_time;
            }
            last_OA2_service_time = total_simulation_time;

            serviced_requests_in_OA2++;

            // Печать статистики каждые 100 заявок
            if (serviced_requests_in_OA2 % 100 == 0)
            {
                printf("Processed %d requests in OA2\n", serviced_requests_in_OA2);
                printf("Current length of queue1: %d\n", queue_length(queue1));
                printf("Current length of queue2: %d\n", queue_length(queue2));
            }

            // Возможность возврата заявки в первую очередь после ОА2
            if ((double) rand() / RAND_MAX < P)
            {
                enqueue(queue1, req);
            }
        }
    }

    // Итоговая статистика
    printf("\nSimulation complete!\n");
    printf("Total simulation time: %.2f units\n", total_simulation_time);
    printf("Total idle time for OA2: %.2f units\n", OA2_idle_time);
    printf("Total activations of OA1: %d\n", OA1_activations);
    printf("Final length of queue1: %d\n", queue_length(queue1));
    printf("Final length of queue2: %d\n", queue_length(queue2));

}
