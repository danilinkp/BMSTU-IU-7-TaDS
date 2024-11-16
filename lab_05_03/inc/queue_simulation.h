#ifndef LAB_05_03_QUEUE_SIMULATION_H
#define LAB_05_03_QUEUE_SIMULATION_H

#include "queue.h"
#include "request.h"

typedef struct
{
    int is_busy; // Занят ли аппарат
    double end_time; // Время конца обслуживания
    request_t current_request; // Текущая заявка в аппарате
} service_unit_t;

void simulation_queue(queue_t *queue1, queue_t *queue2, double t1_start, double t1_end,
                      double t2_start, double t2_end, double probability);

#endif //LAB_05_03_QUEUE_SIMULATION_H
