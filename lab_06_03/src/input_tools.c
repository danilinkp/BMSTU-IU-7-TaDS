#include "input_tools.h"
#include <ctype.h>


static void clean_stdin(void)
{
    char c;
    while ((c = getchar()) != '\n');
}

int read_str(FILE *file, char *str_dst, size_t max_len)
{
    char tmp[max_len + 2];
    if (fgets(tmp, max_len + 2, file) == NULL)
        return EMPTY_INPUT_ERROR;

    if (tmp[strlen(tmp) - 1] == '\n')
        tmp[strlen(tmp) - 1] = '\0';
    else if (file == stdin)
    {
        clean_stdin();
        return STR_LEN_ERROR;
    }

    if (strlen(tmp) == 0)
        return STR_LEN_ERROR;

    if (strlen(tmp) > MAX_STR_LEN)
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

int read_int(FILE *file, int *num)
{
    char tmp[MAX_STR_LEN + 1];
    int rc = read_str(file, tmp, MAX_STR_LEN);
    if (rc != EXIT_SUCCESS)
        return rc;
    if (!is_int(tmp))
        return NOT_NUMBER_ERROR;
    if (sscanf(tmp, "%d", num) != 1)
        return NOT_NUMBER_ERROR;
    return EXIT_SUCCESS;
}
