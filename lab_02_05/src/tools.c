#include "tools.h"
#include <ctype.h>

int read_str(FILE *input, char *str_dst, size_t max_len)
{
    char tmp[max_len + 2];
    if (fgets(tmp, max_len + 2, input) == NULL)
        return EMPTY_INPUT_ERROR;
    tmp[strcspn(tmp, "\n")] = '\0';

    if (strlen(tmp) > max_len || strlen(tmp) == 0)
        return STR_LEN_ERROR;

    strcpy(str_dst, tmp);
    return EXIT_SUCCESS;
}

int is_int(const char *s)
{
    for (size_t i = 0; i < strlen(s); i++)
    {
        if (!isdigit(s[i]))
        {
            if (i == 0 && (s[i] == '-' || s[i] == '+'))
                continue;
            return 0;
        }
    }
    return 1;
}

int read_int(FILE *input, int *num)
{
    char tmp[MAX_STR_LEN + 1];
    int max_len = 1000000;
    int rc = read_str(input, tmp, max_len);
    if (rc != EXIT_SUCCESS)
        return rc;
    if (!is_int(tmp))
        return NOT_NUMBER_ERROR;
    if (sscanf(tmp, "%d", num) != 1)
        return NOT_NUMBER_ERROR;
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
