#include "tools.h"
#include <string.h>

int read_int(FILE *input, int *num)
{
    if (fscanf(input, "%d", num) != 1)
        return NOT_NUMBER_ERROR;
    return EXIT_SUCCESS;
}

int read_two_ints(FILE *input, int *num_1, int *num_2)
{
    if (fscanf(input, "%d %d", num_1, num_2) != 2)
        return NOT_NUMBER_ERROR;
    return EXIT_SUCCESS;
}

int read_str(FILE *input, char *str_dst, size_t max_len)
{
    char tmp[max_len + 2];
    if (fgets(tmp, max_len + 2, input) == NULL)
        return EMPTY_INPUT_ERROR;
    tmp[strcspn(tmp, "\n")] = '\0';

    if (strlen(tmp) > max_len || strlen(tmp) == 0)
        return STR_LEN_ERROR;

    size_t len = strlen(tmp);
    strncpy(str_dst, tmp, len);
    return EXIT_SUCCESS;
}
