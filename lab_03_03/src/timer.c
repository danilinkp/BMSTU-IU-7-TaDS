#include "timer.h"
#include <time.h>
#include <sys/time.h>
#include <stdio.h>

static unsigned long long microseconds_now(void)
{
    struct timeval value;

    if (gettimeofday(&value, NULL))
        return (unsigned long long) -1;

    return value.tv_sec * 1000ULL * 1000ULL + value.tv_usec;
}
static double std_multiplication_time(const matrix_t *matrix, const vector_t *vector, vector_t *result, size_t count)
{
    unsigned long long beg, end;
    double sum = 0;
    vector_t std = {.rows = vector->rows, .num_non_zeros = vector->rows};
    sparse_vector_to_std(*vector, &std);
    for (size_t i = 0; i < count; i++)
    {
        beg = microseconds_now();
        matrix_mul_vector(matrix, &std, result);
        end = microseconds_now();
        sum += end - beg;
    }
    vector_free(&std);
    return sum / count;
}

static double
sparse_multiplication_time(const sparse_matrix_t *sparse_matrix, const vector_t *vector, size_t count)
{
    unsigned long long beg, end;
    double sum = 0;
    vector_t result = {.rows = sparse_matrix->rows, .num_non_zeros = sparse_matrix->rows};
    for (size_t i = 0; i < count; i++)
    {
        beg = microseconds_now();
        sparse_matrix_mul_vector(sparse_matrix, vector, &result);
        end = microseconds_now();
        sum += end - beg;
    }
    vector_free(&result);
    return sum / count;
}

static void print_table_header(void)
{
    printf("| %-15s| %-22s| %-12s| %-15s| %-15s| %-18s|\n", "Matrix size", "Percentage of filling", "Std time",
           "Sparse time", "Memory of std", "Memory of sparse");
    printf("|----------------|-----------------------|-------------|----------------|----------------|-------------------|\n");
}

static int print_time(size_t rows, size_t percentage)
{
    size_t len = (rows * rows * percentage) / 100;
    size_t vec_len = (rows * percentage) / 100;
    vector_t vector = {.rows = rows, .num_non_zeros = vec_len}, result_std = {.rows = rows, .num_non_zeros = rows};
    matrix_t matrix = {.rows = rows, .cols = rows};
    sparse_matrix_t sparse_matrix = {.rows = rows, .cols = rows, .num_non_zeros = len};
    int rc = vector_alloc(&vector);
    if (rc != EXIT_SUCCESS)
    {
        printf("Ошибка выделения памяти.\n");
        return rc;
    }
    rc = vector_alloc(&result_std);
    if (rc != EXIT_SUCCESS)
    {
        printf("Ошибка выделения памяти.\n");
        return rc;
    }
    rc = matrix_alloc(&matrix);
    if (rc != EXIT_SUCCESS)
    {
        printf("Ошибка выделения памяти.\n");
        return rc;
    }
    rc = sparse_matrix_alloc(&sparse_matrix);
    if (rc != EXIT_SUCCESS)
    {
        printf("Ошибка выделения памяти.\n");
        return rc;
    }
    fill_vector_rand(&vector);
    fill_matrix_with_rand_elems(&matrix, &len);
    std_matrix_to_sparse(matrix, &sparse_matrix);
    double time_std = std_multiplication_time(&matrix, &vector, &result_std, NUM_OF_ITERATIONS);
    double time_sparse = sparse_multiplication_time(&sparse_matrix, &vector, NUM_OF_ITERATIONS);

    size_t std_memory = matrix.rows * matrix.cols * sizeof(int);
    size_t sparse_memory =
            sparse_matrix.num_non_zeros * (sizeof(int) + sizeof(size_t)) + (sparse_matrix.rows + 1) * sizeof(size_t);

    printf("| %-15zu| %-22zu| %-12.2lf| %-15.2lf| %-15zu| %-18zu|\n", rows, percentage, time_std,
           time_sparse, std_memory, sparse_memory);
    printf("|----------------|-----------------------|-------------|----------------|----------------|-------------------|\n");

    matrix_free(&matrix);
    sparse_matrix_free(&sparse_matrix);
    vector_free(&vector);
    vector_free(&result_std);
    return EXIT_SUCCESS;
}

void print_measurements(void)
{
    size_t sizes[] = {10, 50, 100, 500};
    size_t sizes_len = sizeof(sizes) / sizeof(sizes[0]);

    size_t percents[] = {10, 25, 35, 50, 75, 100};
    size_t percents_len = sizeof(percents) / sizeof(percents[0]);

    for (size_t i = 0; i < sizes_len; i++)
    {
        print_table_header();
        for (size_t j = 0; j < percents_len; j++)
            print_time(sizes[i], percents[j]);
        printf("\n");
    }
}
