#ifndef LAB_07_04_INPUT_TOOLS_H
#define LAB_07_04_INPUT_TOOLS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STR_LEN 256

enum errors_tools
{
    NOT_NUMBER_ERROR = 6,
    EMPTY_INPUT_ERROR,
    STR_LEN_ERROR,
};

int read_str(FILE *file, char *str_dst, size_t max_len);

int read_int(FILE *file, int *num);

#endif //LAB_07_04_INPUT_TOOLS_H
