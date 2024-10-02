#include <stdio.h>
#include <stdlib.h>
#include "bignum.h"

#define MAX_RESULT_ORDER_SIZE 99999

#define ERROR_FIRST_MANTISSA_LEN 5
#define ERROR_SECOND_MANTISSA_LEN 6
#define ERROR_RESULT_MANTISSA_LEN 7
#define ERROR_RESULT_ORDER_SIZE 8

static char *error_names[] = {"Ошибка: введенная строка пустая",
                              "Ошибка длины вводимой строки",
                              "Ошибка длины порядка числа",
                              "Введен недопустимый символ"};

int main(void)
{

    printf("Программа выполняет действие умножения действительного числа на целое.\n");

    bignum_t a;
    printf("Введите первое число в формате +-m.nЕ+-K, "
           "где суммарная длина мантиссы первого\nсомножителя (m+n) - до 40 значащих цифры, "
           "порядок до 5 цифр."
           "\n----1----2----3----4----5----6----7----8\n");
    int rc = bignum_scan(&a);
    if (rc != EXIT_SUCCESS)
    {
        printf("%s\n", error_names[rc - 1]);
        return rc;
    }
    if (a.mantissa_len > MAX_MANTISSA_LEN)
    {
        printf("ОШИБКА: мантисса первого числа слишком длинная\n");
        return ERROR_FIRST_MANTISSA_LEN;
    }

    bignum_t b;
    printf("Введите второе целое число длиной до 30 десятичных цифр"
           "\n----1----2----3----4----5----6\n");
    rc = bigint_scan(&b);
    if (rc != EXIT_SUCCESS)
    {
        printf("%s\n", error_names[rc - 1]);
        return rc;
    }
    if (b.mantissa_len > MAX_INT_LEN)
    {
        printf("ОШИБКА: мантисса второго числа слишком длинная\n");
        return ERROR_SECOND_MANTISSA_LEN;
    }

    bignum_t res;
    bignum_mul(&a, &b, &res);
    if (res.mantissa_len > MAX_RESULT_MANTISSA_LEN)
    {
        printf("ОШИБКА: мантисса результата слишком длинная\n");
        return ERROR_RESULT_MANTISSA_LEN;
    }
    if (abs(res.order) > MAX_RESULT_ORDER_SIZE)
    {
        printf("ОШИБКА: порядок результата слишком большой\n");
        return ERROR_RESULT_ORDER_SIZE;
    }
    printf("Результат: ");
    bignum_print(&res);

    printf("---------\n");

    return EXIT_SUCCESS;
}
