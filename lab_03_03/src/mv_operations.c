#include "mv_operations.h"

void matrix_mul_vector(const matrix_t *matrix, const vector_t *vector, vector_t *result)
{
    for (size_t i = 0; i < matrix->rows; i++)
        for (size_t j = 0; j < matrix->cols; j++)
        {
            result->data[i] += matrix->data[i][j] * get_vector_element(vector, j);
            result->ib[i] = i;
        }
}

void sparse_matrix_mul_vector(const sparse_matrix_t *sparse_matrix, const vector_t *vector, vector_t *result)
{
    for (size_t row = 0; row < sparse_matrix->rows; row++)
    {
        for (size_t ind = sparse_matrix->ia[row]; ind < sparse_matrix->ia[row + 1]; ind++)
            result->data[row] += sparse_matrix->a[ind] * get_vector_element(vector, sparse_matrix->ja[ind]);
        result->ib[row] = row;
    }
}
