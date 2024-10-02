#ifndef LAB_01_BIG_FLOAT_H
#define LAB_01_BIG_FLOAT_H

#include <stdlib.h>

#define MAX_INT_LEN 30
#define MAX_MANTISSA_LEN 40
#define MAX_RESULT_MANTISSA_LEN 30
#define MAX_ORDER_LEN 5

typedef struct
{
    int is_negative;
    int mantissa[MAX_MANTISSA_LEN];
    size_t mantissa_len;
    int order;
} bignum_t;

enum errors
{
    ERROR_EMPTY_INPUT = 1,
    ERROR_NUM_LEN,
    ORDER_LEN_ERR,
    ERROR_INVALID_SYMBOL
};


int bignum_mul(bignum_t *dst, bignum_t *src, bignum_t *result);

int bigint_scan(bignum_t *bigint);

int bignum_scan(bignum_t *bignum);

void bignum_print(bignum_t *bignum);

int str_to_bignum(char str[], size_t len, bignum_t *bignum);

void bignum_to_str(char str[], size_t *len, bignum_t *bignum);

#endif //LAB_01_BIG_FLOAT_H
