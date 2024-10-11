#include "bignum.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_STR_LEN 100
#define BASE 10


static void normalize_num(bignum_t *bignum)
{
    // Нормализуем нули стоящие сразу после точки
    if (bignum->mantissa_len != 0)
    {
        for (size_t i = bignum->mantissa_len - 1; bignum->mantissa[i] == 0; i--)
        {
            bignum->mantissa_len--;
            bignum->order--;
        }
    }
    // Удаление незначащих нулей
    size_t count = 0;
    int is_not_zero = 0;
    for (size_t i = 0; i < bignum->mantissa_len; i++)
    {
        if (bignum->mantissa[i] == 0 && !is_not_zero)
            count++;
        else
        {
            is_not_zero = 1;
            bignum->mantissa[i - count] = bignum->mantissa[i];
        }
    }
    bignum->mantissa_len -= count;
}


void bignum_to_str(char str[], size_t *len, bignum_t *bignum)
{
    char buf[MAX_STR_LEN];
    int next_ind = 0;
    if (bignum->mantissa_len == 0)
    {
        buf[0] = '0';
        next_ind = 1;
    }
    else
    {
        if (bignum->is_negative)
        {
            buf[0] = '-';
            buf[1] = '0';
            buf[2] = '.';
            next_ind = 3;
        }
        else
        {
            buf[0] = '0';
            buf[1] = '.';
            next_ind = 2;
        }
        for (int i = bignum->mantissa_len - 1; i >= 0; i--)
            next_ind += snprintf(str + next_ind, MAX_STR_LEN + 1 - next_ind, "%d", bignum->mantissa[i]);
    }
    snprintf(str + next_ind, MAX_STR_LEN + 1 - next_ind, "E%d", bignum->order);
    strncpy(str, buf, strlen(buf));
    *len = strlen(str);
}

void bignum_print(bignum_t *bignum)
{
    normalize_num(bignum);

    char sbignum[MAX_STR_LEN - 1];
    size_t sbignum_len;

    bignum_to_str(sbignum, &sbignum_len, bignum);
    printf("%s\n", sbignum);
}

int bignum_scan(bignum_t *bignum)
{
    char buf[MAX_STR_LEN + 1];
    if (fgets(buf, MAX_STR_LEN + 1, stdin) == NULL)
        return ERROR_EMPTY_INPUT;

    buf[strcspn(buf, "\n")] = '\0';
    if (strlen(buf) > MAX_STR_LEN)
        return ERROR_NUM_LEN;
    if (strlen(buf) == 0)
        return ERROR_EMPTY_INPUT;

    int rc = str_to_bignum(buf, strlen(buf), bignum);

    return rc;
}

static int is_int(char str[])
{
    int is_int = 1;
    for (size_t i = 0; i < strlen(str); i++)
        if (!isdigit(str[i]) && str[i] != '-')
            is_int = 0;
    return is_int;
}

int bigint_scan(bignum_t *bigint)
{
    char buf[MAX_MANTISSA_LEN + 1];
    if (fgets(buf, MAX_MANTISSA_LEN + 1, stdin) == NULL)
        return ERROR_EMPTY_INPUT;

    buf[strcspn(buf, "\n")] = '\0';
    if (strlen(buf) > MAX_STR_LEN || strlen(buf) == 0)
        return ERROR_EMPTY_INPUT;

    if (!is_int(buf))
        return ERROR_INVALID_SYMBOL;

    int rc = str_to_bignum(buf, strlen(buf), bigint);

    return rc;
}

static int get_order(char str[], size_t len, int *order, int *order_ind)
{
    char *exponent_substr = strstr(str, "E");
    if (exponent_substr != NULL)
    {
        *order_ind = exponent_substr - str;
        int cur_ind = *order_ind;
        if (str[*order_ind + 1] == '+' || str[*order_ind + 1] == '-')
            cur_ind++;
        if (len - cur_ind - 1 > MAX_ORDER_LEN)
            return ORDER_LEN_ERR;
        int tmp = 0;
        for (size_t i = cur_ind + 1; i < len; i++)
        {
            if (!isdigit(str[i]))
                return ERROR_INVALID_SYMBOL;
            tmp *= BASE;
            tmp += (int) (str[i] - '0');
        }
        *order = tmp;
        if (str[*order_ind + 1] == '-')
            *order *= -1;
    }
    return EXIT_SUCCESS;

}

static void get_dot(const char str[], size_t len, int *dot_ind)
{
    for (size_t i = 0; i < len; i++)
        if (str[i] == '.')
        {
            *dot_ind = i;
            break;
        }
}

static void get_no_zero_digit(const char str[], size_t len, int *digit_ind)
{
    for (size_t i = 0; i < len; i++)
        if (isdigit(str[i]) && str[i] != '0')
        {
            *digit_ind = i;
            break;
        }
}

int str_to_bignum(char str[], size_t len, bignum_t *bignum)
{
    bignum->is_negative = 0;
    bignum->mantissa_len = 0;
    bignum->order = 0;

    int order_ind = len, no_0_ind = 0, dot_ind = -1;

    int rc = get_order(str, len, &(bignum->order), &order_ind);

    if (rc != EXIT_SUCCESS)
        return rc;

    get_no_zero_digit(str, len, &no_0_ind);
    get_dot(str, len, &dot_ind);

    if (dot_ind < no_0_ind)
        no_0_ind = dot_ind;

    int is_start_num = 0;
    size_t cur_mantissa_len = 0;
    for (int i = order_ind - 1; i >= 0; i--)
    {
        if (i == 0 && !isdigit(str[i]))
        {
            if (str[i] == '-')
                bignum->is_negative = 1;
            else if (str[i] != '+' && str[i] != '.' && !isdigit(str[i]))
                return ERROR_INVALID_SYMBOL;
        }
        else if (isdigit(str[i]))
        {
            if (i >= no_0_ind)
            {
                int digit = (int) (str[i] - '0');
                if (!is_start_num)
                    if (digit)
                        is_start_num = 1;
                if (is_start_num)
                    bignum->mantissa[cur_mantissa_len++] = digit;
                if ((dot_ind != -1 && i < dot_ind) || dot_ind == -1)
                    bignum->order++;
            }
        }
        else if (str[i] == '.')
        {
            if (dot_ind != -1 && i != dot_ind)
                return ERROR_INVALID_SYMBOL;
            else
                continue;
        }
        else
            return ERROR_INVALID_SYMBOL;
    }
    bignum->mantissa_len = cur_mantissa_len;
    return EXIT_SUCCESS;
}

void control_overflow(int tmp_mantissa[], size_t *len)
{
    int t = 0, cur_num;
    for (size_t i = 0; i < *len; i++)
    {
        cur_num = tmp_mantissa[i];
        tmp_mantissa[i] = cur_num % BASE + t;
        t = cur_num / BASE;
        if (tmp_mantissa[i] > 9)
        {
            cur_num = tmp_mantissa[i];
            tmp_mantissa[i] = cur_num % BASE;
            t = cur_num / BASE;
        }

    }
    if (tmp_mantissa[*len] > 9)
    {
        tmp_mantissa[*len] = tmp_mantissa[*len] % BASE;
        tmp_mantissa[*len + 1] = 1;
        ++(*len);
    }
}


static void rounding(int tmp_mantissa[], size_t start_ind)
{
    for (size_t i = 0; i < start_ind; i++)
    {
        if (tmp_mantissa[i] >= 5)
            tmp_mantissa[i + 1] += 1;
        tmp_mantissa[i] = 0;
    }
}


int bignum_mul(bignum_t *dst, bignum_t *src, bignum_t *result)
{
    if (src->mantissa_len == 0 || dst->mantissa_len == 0)
    {
        result->order = 0;
        result->is_negative = 0;
        result->mantissa_len = 0;
        return EXIT_SUCCESS;
    }
    normalize_num(dst);
    normalize_num(src);
    result->order = src->order + dst->order;
    result->is_negative = (src->is_negative + dst->is_negative) % 2;
    int tmp_mantissa[2 * MAX_MANTISSA_LEN];
    for (size_t i = 0; i < 2 * MAX_MANTISSA_LEN; i++)
        tmp_mantissa[i] = 0;
    for (size_t i = 0; i < src->mantissa_len; i++)
        for (size_t j = 0; j < dst->mantissa_len; j++)
        {
            int cur_num = src->mantissa[i] * dst->mantissa[j];
            tmp_mantissa[i + j] += cur_num % BASE;
            tmp_mantissa[i + j + 1] += cur_num / BASE;
            int control_overflow = tmp_mantissa[i + j];
            tmp_mantissa[i + j] = control_overflow % BASE;
            tmp_mantissa[i + j + 1] += control_overflow / BASE;
        }
    size_t len = src->mantissa_len + dst->mantissa_len;
    // Проверка того, что мантиса будет меньше на единицу от максимально возможной
    if (tmp_mantissa[len - 1] == 0 && len == MAX_RESULT_MANTISSA_LEN + 1)
    {
        len--;
        result->order--;
    }
    control_overflow(tmp_mantissa, &len);
    size_t start_ind = 0;
    if (len > MAX_RESULT_MANTISSA_LEN)
        start_ind = len - MAX_RESULT_MANTISSA_LEN;
    rounding(tmp_mantissa, start_ind);
    control_overflow(tmp_mantissa, &len);
    result->mantissa_len = 0;
    for (size_t i = start_ind; i < len; i++)
        result->mantissa[result->mantissa_len++] = tmp_mantissa[i];
    return EXIT_SUCCESS;
}
