#ifndef LAB_05_03_REQUEST_H
#define LAB_05_03_REQUEST_H

typedef struct
{
    int id;
    double processing_time;
} request_t;

double generateTime(double t1, double t2);

#endif //LAB_05_03_REQUEST_H
