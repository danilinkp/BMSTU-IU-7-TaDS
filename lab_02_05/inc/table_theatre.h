#ifndef LAB_02_05_TABLE_THEATRE_H
#define LAB_02_05_TABLE_THEATRE_H

#include <stdlib.h>
#include "theatre.h"

#define MAX_ROW_COUNT 1000

typedef struct
{
    theatre_t theatres[MAX_ROW_COUNT];
    size_t rows;
} table_theatre_t;

typedef struct
{
    size_t theatr_id;
    int price_low;
} table_key_t;

#endif //LAB_02_05_TABLE_THEATRE_H
