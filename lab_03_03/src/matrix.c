#include "matrix.h"
#include <stdio.h>
#include <time.h>

// Функции выделения и освобождения памяти
void matrix_free(matrix_t *matrix)
{
    free(matrix->data);
    matrix->data = NULL;
}


int matrix_alloc(matrix_t *matrix)
{
    matrix->data = malloc(matrix->rows * sizeof(int*) +
                                  matrix->rows * matrix->cols * sizeof(int));
    if (!matrix->data)
        return DATA_ALLOC_ERROR;
    for (size_t i = 0; i < matrix->rows; i++)
        matrix->data[i] = (int*)((char*) matrix->data +
                matrix->rows * sizeof(int*) +
                            i * matrix->cols * sizeof(int));
    for (size_t i = 0; i < matrix->rows; i++)
    {
        for (size_t j = 0; j < matrix->cols; j++)
        {
            matrix->data[i][j] = 0;
        }
    }
    return EXIT_SUCCESS;
}

void sparse_matrix_free(sparse_matrix_t *sparse_matrix)
{
    free(sparse_matrix->a);
    free(sparse_matrix->ia);
    free(sparse_matrix->ja);
    sparse_matrix->a = NULL;
    sparse_matrix->ja = NULL;
    sparse_matrix->ia = NULL;
}

int sparse_matrix_alloc(sparse_matrix_t *sparse_matrix)
{
    sparse_matrix->a = calloc(sparse_matrix->num_non_zeros, sizeof(size_t));
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
void read_matrix_size(size_t *rows, size_t *cols, size_t *num_non_zeros, int is_vector_readed, size_t vector_rows)
{
    int rc = EXIT_SUCCESS;

    int tmp_cols, tmp_rows, tmp_non_zeros;
    do
    {
        printf("Введите количество строк матрицы: ");
        if ((scanf("%d", &tmp_rows)) != 1 || tmp_rows <= 0)
        {
            printf("Количество строк введено неверно. Введите еще раз\n");
            rc = READ_SIZE_ERROR;
            continue;
        }
        rc = EXIT_SUCCESS;
    } while (rc != EXIT_SUCCESS);
    *rows = (size_t) tmp_rows;

    if (!is_vector_readed)
    {
        do
        {
            printf("Введите количество столбцов матрицы: ");
            if ((scanf("%d", &tmp_cols)) != 1 || tmp_cols <= 0)
            {
                printf("Количество столбцов введено неверно. Введите еще раз\n");
                rc = READ_SIZE_ERROR;
                continue;
            }
            rc = EXIT_SUCCESS;
        } while (rc != EXIT_SUCCESS);
        *cols = (size_t) tmp_cols;
    }
    else
    {
        printf("Количество столбцов матрицы равно кол-ву строк вектора столбца = %zu.\n", vector_rows);
        *cols = vector_rows;
    }

    do
    {
        printf("Введите количество ненулевых элементов матрицы (от 1 до %zu): ", *rows * *cols);
        if ((scanf("%d", &tmp_non_zeros)) != 1 || tmp_non_zeros <= 0 || (size_t) tmp_non_zeros > *rows * *cols)
        {
            printf("Количество ненулевых элементов матрицы введено неверно. Введите еще раз\n");
            rc = READ_SIZE_ERROR;
            continue;
        }
        rc = EXIT_SUCCESS;
    } while (rc != EXIT_SUCCESS);
    *num_non_zeros = (size_t) tmp_non_zeros;
}

static void read_matrix_element(size_t *row, size_t *col, int *value, size_t rows, size_t cols)
{
    int tmp_row, tmp_col;
    int tmp_value;
    int rc = EXIT_SUCCESS;
    do
    {
        printf("Введите индекс строки матрицы (от 0 до %zu): ", rows - 1);
        if (scanf("%d", &tmp_row) != 1 || tmp_row < 0 || (size_t) tmp_row > rows - 1)
        {
            printf("Ошибка ввода индекса строки. Введите еще раз\n");
            rc = ROW_READ_ERROR;
            continue;
        }
        rc = EXIT_SUCCESS;
    } while (rc != EXIT_SUCCESS);
    *row = (size_t) tmp_row;

    do
    {
        printf("Введите индекс столбца матрицы (от 0 до %zu): ", cols - 1);
        if (scanf("%d", &tmp_col) != 1 || tmp_col < 0 || (size_t) tmp_col > cols - 1)
        {
            printf("Ошибка ввода индекса столбца.\n");
            rc = COL_READ_ERROR;
            continue;
        }
        rc = EXIT_SUCCESS;
    } while (rc != EXIT_SUCCESS);
    *col = (size_t) tmp_col;

    do
    {
        printf("Введите ненулевое значение элемента матрицы: ");
        if (scanf("%d", &tmp_value) != 1 || tmp_value == 0)
        {
            printf("Ошибка ввода значения элемента. Введите еще раз\n");
            rc = ELEM_READ_ERROR;
            continue;
        }
        rc = EXIT_SUCCESS;
    } while (rc != EXIT_SUCCESS);
    *value = tmp_value;
}

void fill_matrix_with_coords(matrix_t *matrix, size_t const *num_non_zeros)
{
    size_t row, col;
    int value;
    for (size_t i = 0; i < *num_non_zeros; i++)
    {
        read_matrix_element(&row, &col, &value, matrix->rows, matrix->cols);
        if (matrix->data[row][col] == 0)
            matrix->data[row][col] = value;
        else
        {
            i--;
            matrix->data[row][col] = value;
            printf("Вы перезаписали ненулевой элемент\n");
        }
    }
}

void fill_matrix_with_rand_elems(matrix_t *matrix, size_t const *num_non_zeros)
{
    srand(time(NULL));
    for (size_t k = 0; k < *num_non_zeros; k++)
    {
        size_t i = rand() % matrix->rows;
        size_t j = rand() % matrix->cols;
        if (matrix->data[i][j] == 0)
            matrix->data[i][j] = rand() % 10 + 1;
        else
            k--;
    }
}

// Функции перевода из одной формы в другую
void std_matrix_to_sparse(matrix_t matrix, sparse_matrix_t *sparse_matrix)
{
    size_t non_zero_el_ind = 0;
    for (size_t i = 0; i < matrix.rows; i++)
    {
        sparse_matrix->ia[i] = non_zero_el_ind;
        for (size_t j = 0; j < matrix.cols; j++)
            if (matrix.data[i][j] != 0)
            {
                sparse_matrix->a[non_zero_el_ind] = matrix.data[i][j];
                sparse_matrix->ja[non_zero_el_ind] = j;
                non_zero_el_ind++;
            }
    }
    sparse_matrix->ia[sparse_matrix->rows] = sparse_matrix->num_non_zeros;
}

void sparse_matrix_to_std(sparse_matrix_t sparse_matrix, matrix_t *matrix)
{
    for (size_t i = 0; i < sparse_matrix.rows; i++)
        for (size_t j = 0; j < sparse_matrix.cols; j++)
            matrix->data[i][j] = 0;

    for (size_t row = 0; row < sparse_matrix.rows; row++)
        for (size_t ind = sparse_matrix.ia[row]; ind < sparse_matrix.ia[row + 1]; ind++)
            matrix->data[row][sparse_matrix.ja[ind]] = sparse_matrix.a[ind];
}


// Функции вывода матриц
void print_matrix(matrix_t matrix)
{
    printf("Матрица размером %zu на %zu:\n", matrix.rows, matrix.cols);
    for (size_t i = 0; i < matrix.rows; i++)
        for (size_t j = 0; j < matrix.cols; j++)
        {
            if (j == matrix.cols - 1)
                printf("%d\n", matrix.data[i][j]);
            else
                printf("%d ", matrix.data[i][j]);
        }
}

void print_sparse_matrix(sparse_matrix_t sparse_matrix)
{
    printf("Вектор А, который содержит значения ненулевых элементов:\n");
    for (size_t i = 0; i < sparse_matrix.num_non_zeros; i++)
    {
        if (i == sparse_matrix.num_non_zeros - 1)
            printf("%d\n", sparse_matrix.a[i]);
        else
            printf("%d ", sparse_matrix.a[i]);
    }

    printf("Вектор JA, который содержит номера столбцов, для элеметнов вектора А:\n");
    for (size_t i = 0; i < sparse_matrix.num_non_zeros; i++)
    {
        if (i == sparse_matrix.num_non_zeros - 1)
            printf("%zu\n", sparse_matrix.ja[i]);
        else
            printf("%zu ", sparse_matrix.ja[i]);
    }

    printf("Вектор IA, который содержит индекс вектора JA, с которой начинается строка матрицы A:\n");
    for (size_t i = 0; i < sparse_matrix.rows + 1; i++)
    {
        if (i == sparse_matrix.rows)
            printf("%zu\n", sparse_matrix.ia[i]);
        else
            printf("%zu ", sparse_matrix.ia[i]);
    }
}

//1 2   2   12
//0 4 x 5 = 20
//2 3       19
//
//3 * 2 x 2 * 1 = 3 * 1
