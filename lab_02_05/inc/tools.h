#ifndef LAB_02_05_TOOLS_H
#define LAB_02_05_TOOLS_H

#include <stdlib.h>
#include <stdio.h>


enum errors
{
    NOT_NUMBER_ERROR = 6,
    EMPTY_INPUT_ERROR,
    STR_LEN_ERROR,
};

int read_int(FILE *input, int *num);

int read_two_ints(FILE *input, int *num_1, int *num_2);

int read_str(FILE *input, char *str_dst, size_t max_len);

#endif //LAB_02_05_TOOLS_H
