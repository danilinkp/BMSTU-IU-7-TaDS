#ifndef LAB_03_03_VECTOR_H
#define LAB_03_03_VECTOR_H

#include <stdlib.h>

typedef struct
{
    int *data; // вектор В, содержащий значения ненулевых элементов
    size_t *ib; // вектор IB, содержащий индексы ненулевых элементов
    size_t num_non_zeros;
    size_t rows;
} vector_t;

enum vector_errors
{
    DATA_ALLOC_ERROR = 6,
    INDEXES_ALLOC_ERROR,
    ELEM_READ_ERROR,
    INDEX_READ_ERROR,
    VECTOR_LEN_READ_ERROR,
};

void vector_free(vector_t *vector);

int vector_alloc(vector_t *vector);

size_t read_vector_sizes(size_t *rows, size_t *len, int is_matrix_readed, size_t matrix_cols);

void read_vector(vector_t *vector);

void fill_vector_rand(vector_t *vector);

void print_vector_std(vector_t vector);

void print_sparse_vector(vector_t vector);

int get_vector_element(const vector_t *vector, size_t index_j);

void del_zero_elements(vector_t *vector);

#endif //LAB_03_03_VECTOR_H
