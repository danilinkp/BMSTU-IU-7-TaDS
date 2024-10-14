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

} matrix_errors_t;

#endif //LAB_03_03_SPARSE_MATRIX_H
