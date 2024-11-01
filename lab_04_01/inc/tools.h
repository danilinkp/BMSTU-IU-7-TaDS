#ifndef LAB_04_01_TOOLS_H
#define LAB_04_01_TOOLS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum errors_tools
{
    NOT_NUMBER_ERROR = 6,
    EMPTY_INPUT_ERROR,
    STR_LEN_ERROR,
};

int read_int(int *num);

#endif //LAB_04_01_TOOLS_H
