#include "tools.h"
#include <ctype.h>

#define MAX_STR_LEN 30

static int read_str(char *str_dst, size_t max_len)
{
    char tmp[max_len + 2];
    if (fgets(tmp, max_len + 2, stdin) == NULL)
        return EMPTY_INPUT_ERROR;
    tmp[strcspn(tmp, "\n")] = '\0';
    if (strlen(tmp) == 0)
        return STR_LEN_ERROR;
    if (strlen(tmp) > max_len)
        return STR_LEN_ERROR;

    strcpy(str_dst, tmp);
    return EXIT_SUCCESS;
}

static int is_int(const char *s)
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

int read_int(int *num)
{
    char tmp[MAX_STR_LEN + 1];
    int max_len = MAX_STR_LEN;
    int rc = read_str(tmp, max_len);
    if (rc != EXIT_SUCCESS)
        return rc;
    if (!is_int(tmp))
        return NOT_NUMBER_ERROR;
    if (sscanf(tmp, "%d", num) != 1)
        return NOT_NUMBER_ERROR;
    return EXIT_SUCCESS;
}
