#include "mv_operations.h"
#include <stdio.h>

void matrix_mul_vector(const matrix_t *m1, const vector_t *vec, vector_t *dst)
{
    size_t el_count = 0;
    for (size_t i = 0; i < m1->rows; ++i)
    {
        int sum = 0;
        for (size_t j = 0; j < m1->cols; j++)
            sum += m1->data[i][j] * vec->data[j];
        dst->data[el_count] = sum;
        dst->ib[el_count] = i;
        ++el_count;
    }
}

void sparse_matrix_mul_vector(const sparse_matrix_t *matrix, const vector_t *vector, vector_t *result)
{
//    size_t *IP = calloc(vector->rows, sizeof(size_t));
    size_t *IP = malloc(vector->rows * sizeof(size_t));
    for (size_t i = 0; i < vector->rows; ++i)
        IP[i] = 0;
    for (size_t i = 0; i < vector->num_non_zeros; ++i)
        IP[vector->ib[i]] = i + 1;
    vector_alloc(result);
    size_t el_count = 0;
    for (size_t i = 0; i < matrix->rows; ++i)
    {
        int sum = 0;

        for (size_t ind = matrix->ia[i]; ind < matrix->ia[i + 1]; ++ind)
        {
            size_t col = matrix->ja[ind];
            if (IP[col] > 0)
            {
                size_t vector_index = IP[col] - 1;
                sum += matrix->a[ind] * vector->data[vector_index];
            }
        }

        if (sum != 0)
        {
            result->data[el_count] = sum;
            result->ib[el_count] = i;
            el_count++;
        }
    }
    free(IP);
}
