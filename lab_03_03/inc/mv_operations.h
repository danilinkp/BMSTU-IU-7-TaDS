#ifndef LAB_03_03_MV_OPERATIONS_H
#define LAB_03_03_MV_OPERATIONS_H

#include "matrix.h"
#include "vector.h"


void matrix_mul_vector(matrix_t *matrix, vector_t *vector, vector_t *result);

void sparse_matrix_mul_vector(sparse_matrix_t *sparse_matrix, vector_t *vector, vector_t *result);


#endif //LAB_03_03_MV_OPERATIONS_H
