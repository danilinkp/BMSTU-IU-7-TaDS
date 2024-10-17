#ifndef LAB_03_03_SPARSE_MATRIX_H
#define LAB_03_03_SPARSE_MATRIX_H

#include <stdlib.h>

typedef struct
{
    size_t rows; // Кол-во строк
    size_t cols; // Кол-во столбцов
    size_t num_non_zeros; // Кoл-во ненулевых элементов

    int *a; // Вектор значения ненулевых элементов
    size_t *ja; // Номера столбцов вектора а
    size_t *ia; // Вектор индексов начала строк
} sparse_matrix_t;

typedef struct
{
    int **data;
    size_t rows;
    size_t cols;
} matrix_t;

typedef enum
{
    READ_SIZE_ERROR = 1,
    DATA_ALLOC_ERROR,
    ELEM_READ_ERROR,
    ROW_READ_ERROR,
    COL_READ_ERROR
} matrix_errors_t;

// Выделение памяти под стандартную матрицу
int matrix_alloc(matrix_t *matrix);

// Очищение памяти для стандартной матрицы
void matrix_free(matrix_t *matrix);

//Выделение памяти под разрееженную матрицу
int sparse_matrix_alloc(sparse_matrix_t *sparse_matrix);

// Очищение памяти для разреженной матрицы
void sparse_matrix_free(sparse_matrix_t *sparse_matrix);

// Чтение размера матриц
void read_matrix_size(size_t *rows, size_t *cols, size_t *num_non_zeros, int is_vector_readed, size_t vector_rows);

// Заполнение матрицы, путем ввода координатным способом
void fill_matrix_with_coords(matrix_t *matrix, const size_t *num_non_zeros);

// Заполнение матрицы рандомными элементами в рандомные позиции, соответствующий кол-ву элементов
void fill_matrix_with_rand_elems(matrix_t *matrix, size_t const *num_non_zeros);

// Перевод из стандартной матрицы в разреженную
void std_matrix_to_sparse(matrix_t matrix, sparse_matrix_t *sparse_matrix);

// Перевод из разреженной в стандартную:
void sparse_matrix_to_std(sparse_matrix_t sparse_matrix, matrix_t *matrix);

// Вывод матрицы в стандартном виде
void print_matrix(matrix_t matrix);

// Вывод матрицы в разреженном виде
void print_sparse_matrix(sparse_matrix_t sparse_matrix);

#endif //LAB_03_03_SPARSE_MATRIX_H
