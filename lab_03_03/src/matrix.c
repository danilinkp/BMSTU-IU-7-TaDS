#include "matrix.h"
#include <stdio.h>


// Функции выделения и освобождения памяти
void matrix_free(matrix_t *matrix)
{
    for (size_t i = 0; i < matrix->rows; i++)
        free(matrix->data[i]);
    free(matrix->data);
}

int matrix_alloc(matrix_t *matrix)
{
    matrix->data = calloc(matrix->rows, sizeof(int *));
    if (!matrix->data)
        return DATA_ALLOC_ERROR;
    for (size_t i = 0; i < matrix->rows; i++)
    {
        matrix->data[i] = calloc(matrix->cols, sizeof(int));
        if (!matrix->data[i])
        {
            matrix_free(matrix);
            return DATA_ALLOC_ERROR;
        }
    }
    return EXIT_SUCCESS;
}

void sparse_matrix_free(sparse_matrix_t *sparse_matrix)
{
    free(sparse_matrix->a);
    free(sparse_matrix->ia);
    free(sparse_matrix->ja);
}

int sparse_matrix_alloc(sparse_matrix_t *sparse_matrix)
{
    sparse_matrix->a = calloc(sparse_matrix->num_non_zeros, sizeof(int));
    if (!sparse_matrix->a)
        return DATA_ALLOC_ERROR;

    sparse_matrix->ja = calloc(sparse_matrix->num_non_zeros, sizeof(size_t));
    if (!sparse_matrix->ja)
        return DATA_ALLOC_ERROR;

    sparse_matrix->ia = calloc(sparse_matrix->rows + 1, sizeof(size_t));
    if (!sparse_matrix->ia)
        return DATA_ALLOC_ERROR;

    return EXIT_SUCCESS;
}

// Функии чтения данных
void read_matrix_size(size_t *rows, size_t *cols, size_t *num_non_zeros)
{
    int rc = EXIT_SUCCESS;

    size_t tmp_cols, tmp_rows, tmp_non_zeros;
    do
    {
        printf("Введите количество строк матрицы: ");
        if ((scanf("%zu", &tmp_rows)) != 1 || tmp_rows <= 0)
        {
            printf("Количество строк введено неверно.\n");
            rc = READ_SIZE_ERROR;
            continue;
        }
        rc = EXIT_SUCCESS;
    } while (rc != EXIT_SUCCESS);
    *rows = tmp_rows;

    do
    {
        printf("Введите количество строк матрицы: ");
        if ((scanf("%zu", &tmp_cols)) != 1 || tmp_cols <= 0)
        {
            printf("Количество строк введено неверно.\n");
            rc = READ_SIZE_ERROR;
            continue;
        }
        rc = EXIT_SUCCESS;
    } while (rc != EXIT_SUCCESS);
    *cols = tmp_cols;

    do
    {
        printf("Введите количество ненулевых элементов матрицы: ");
        if ((scanf("%zu", &tmp_non_zeros)) != 1 || tmp_non_zeros <= 0)
        {
            printf("Количество ненулевых элементов матрицы введено неверно.\n");
            rc = READ_SIZE_ERROR;
            continue;
        }
        rc = EXIT_SUCCESS;
    } while (rc != EXIT_SUCCESS);
    *num_non_zeros = tmp_non_zeros;
}

int read_matrix()
{
    return 0;
}

int fill_matrix()
{
    return 0;
}

void sparse_matrix_to_std()
{
    return;
}

void std_matrix_to_sparse()
{
    return;
}

void print_matrix()
{
}

void print_sparse_matrix()
{
    return;
}
