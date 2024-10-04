#include "tools.h"

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

int theatre_t_comp(const void *l, const void *r)
{
    const theatre_t *left = (const theatre_t *) l;
    const theatre_t *right = (const theatre_t *) r;

    return (left->price_low - right->price_low);
}

int key_comp(const void *l, const void *r)
{
    const theatre_key_t *left = (const theatre_key_t *) l;
    const theatre_key_t *right = (const theatre_key_t *) r;

    return (left->price_low - right->price_low);
}

unsigned long long microseconds_now(void)
{
    struct timeval value;

    if (gettimeofday(&value, NULL))
        return (unsigned long long) -1;

    return value.tv_sec * 1000ULL * 1000ULL + value.tv_usec;
}
