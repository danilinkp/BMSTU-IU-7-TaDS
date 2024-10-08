#ifndef LAB_02_05_TOOLS_H
#define LAB_02_05_TOOLS_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>
#include "theatre.h"
#include "table_theatre.h"


enum errors
{
    NOT_NUMBER_ERROR = 6,
    EMPTY_INPUT_ERROR,
    STR_LEN_ERROR,
};

int read_int(FILE *input, int *num);

int read_str(FILE *input, char *str_dst, size_t max_len);

int theatre_t_comp(const void *l, const void *r);

int key_comp(const void *l, const void *r);

unsigned long long microseconds_now(void);

#endif //LAB_02_05_TOOLS_H
