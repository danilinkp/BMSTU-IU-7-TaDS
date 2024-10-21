#include "vector.h"
#include <stdio.h>
#include <time.h>


void vector_free(vector_t *vector)
{
    free(vector->data);
    vector->data = NULL;
    free(vector->ib);
    vector->ib = NULL;
    vector->num_non_zeros = 0;
}

int vector_alloc(vector_t *vector)
{
    vector->data = calloc(vector->num_non_zeros, sizeof(int));
    if (!vector->data)
        return VECTOR_DATA_ALLOC_ERROR;

    vector->ib = calloc(vector->num_non_zeros, sizeof(size_t));
    if (!vector->ib)
        return INDEXES_ALLOC_ERROR;
    return EXIT_SUCCESS;
}

static void read_vector_element(size_t *ind, int *value, size_t rows)
{
    int rc = EXIT_SUCCESS;
    int tmp_ind;
    int tmp_value;
    do
    {
        printf("Введите индекс вектора-столбца: ");
        if (scanf("%d", &tmp_ind) != 1 || tmp_ind < 0 || tmp_ind > (int) rows)
        {
            printf("Ошибка ввода индекса вектора-столбца\n");
            rc = INDEX_READ_ERROR;
            continue;
        }
        rc = EXIT_SUCCESS;
    } while (rc != EXIT_SUCCESS);
    *ind = (size_t) tmp_ind;

    do
    {
        printf("Введите ненулевое значение вектора-столбца с индексом %zu: ", *ind);
        if (scanf("%d", &tmp_value) != 1 || tmp_value == 0)
        {
            printf("Ошибка ввода значения вектора-столбца\n");
            rc = VECTOR_ELEM_READ_ERROR;
            continue;
        }
        rc = EXIT_SUCCESS;
    } while (rc != EXIT_SUCCESS);
    *value = tmp_value;
}

void read_vector_sizes(size_t *rows, size_t *len, int is_matrix_readed, size_t matrix_cols)
{
    int tmp_len, tmp_rows;
    int rc = EXIT_SUCCESS;
    if (!is_matrix_readed)
    {
        do
        {
            printf("Введите максимальное количество элементов вектора-столбца: ");
            if ((scanf("%d", &tmp_rows)) != 1 || tmp_rows <= 0)
            {
                printf("Количество элементов введено неверно.\n");
                rc = VECTOR_LEN_READ_ERROR;
                continue;
            }
            rc = EXIT_SUCCESS;
        } while (rc != EXIT_SUCCESS);
        *rows = (size_t) tmp_rows;
    }
    else
    {
        printf("Кол-во строк вектора-столбца равно кол-ву столбцов матрицы = %zu.\n", matrix_cols);
        *rows = matrix_cols;
    }
    do
    {
        printf("Введите количество ненулевых элементов вектора: ");
        if (scanf("%d", &tmp_len) != 1 || tmp_len < 0 || (size_t) tmp_len > *rows)
        {
            printf("Введено неверное количество ненулевых элементов.\n");
            rc = VECTOR_LEN_READ_ERROR;
            continue;
        }
        rc = EXIT_SUCCESS;
    } while (rc != EXIT_SUCCESS);
    *len = (size_t) tmp_len;
}

void read_vector(vector_t *vector)
{
    size_t ind;
    int value;
    for (size_t k = 0; k < vector->num_non_zeros; k++)
    {
        read_vector_element(&ind, &value, vector->rows);
        vector->data[k] = value;
        vector->ib[k] = ind;
    }
}

static size_t generate_random_index(vector_t vector, size_t num_filled)
{
    size_t index;
    int unique;
    do
    {
        unique = 1;
        index = rand() % vector.rows;
        for (size_t i = 0; i < num_filled; i++)
        {
            if (vector.ib[i] == index)
            {
                unique = 0;
                break;
            }
        }
    } while (!unique);

    return index;
}

void fill_vector_rand(vector_t *vector)
{
//    vector->num_non_zeros = read_vector_len(vector->rows);
    srand(time(NULL));
    for (size_t i = 0; i < vector->num_non_zeros; i++)
    {
        vector->data[i] = rand() % 50 + 1;
        vector->ib[i] = generate_random_index(*vector, i);
    }
}

void print_vector_std(vector_t vector)
{
    int *full_vector = calloc(vector.rows, sizeof(int));

    for (size_t i = 0; i < vector.num_non_zeros; i++)
        full_vector[vector.ib[i]] = vector.data[i];
    printf("Полный вектор:\n");
    for (size_t i = 0; i < vector.rows; i++)
        printf("%d\n", full_vector[i]);
    free(full_vector);
}

void sparse_vector_to_std(vector_t vector, vector_t *std)
{
    std->data = calloc(vector.rows, sizeof(int));
    for (size_t i = 0; i < vector.num_non_zeros; i++)
        std->data[vector.ib[i]] = vector.data[i];
}


void print_sparse_vector(vector_t vector)
{
    printf("Ненулевые значения вектора:\n");
    for (size_t i = 0; i < vector.num_non_zeros; i++)
        printf("%d ", vector.data[i]);

    printf("\nИндексы ненулевых значений:\n");
    for (size_t i = 0; i < vector.num_non_zeros; i++)
        printf("%zu ", vector.ib[i]);
}


int get_vector_element(const vector_t *vector, size_t index_j)
{
    for (size_t i = 0; i < vector->num_non_zeros; i++)
        if (vector->ib[i] == index_j)
            return vector->data[i];
    return 0;
}

void del_zero_elements(vector_t *vector)
{
    size_t count_zeros = 0;
    for (size_t i = 0; i < vector->num_non_zeros; i++)
    {
        if (vector->data[i] == 0)
            count_zeros++;
        else
        {
            vector->data[i - count_zeros] = vector->data[i];
            vector->ib[i - count_zeros] = vector->ib[i];
        }
    }
    vector->num_non_zeros -= count_zeros;
}
