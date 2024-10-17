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
    VECTOR_DATA_ALLOC_ERROR = 6,
    INDEXES_ALLOC_ERROR,
    VECTOR_ELEM_READ_ERROR,
    INDEX_READ_ERROR,
    VECTOR_LEN_READ_ERROR,
};

// Выделение памяти для вектора
void vector_free(vector_t *vector);

// Очищение памяти из под вектора
int vector_alloc(vector_t *vector);

// Чтение размеров для вектора
void read_vector_sizes(size_t *rows, size_t *len, int is_matrix_readed, size_t matrix_cols);

// Чтение данных вектора
void read_vector(vector_t *vector);

// Заполнение вектора рандомными элементами
void fill_vector_rand(vector_t *vector);

// Вывод вектора в стандартном виде
void print_vector_std(vector_t vector);

// Вывод вектора в разреженном виде
void print_sparse_vector(vector_t vector);

// Получение элемента вектора по индексу
int get_vector_element(const vector_t *vector, size_t index_j);

// Удаление нулевых элементов вектора
void del_zero_elements(vector_t *vector);

#endif //LAB_03_03_VECTOR_H
