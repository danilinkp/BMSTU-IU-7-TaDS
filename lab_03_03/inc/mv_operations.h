#ifndef LAB_03_03_MV_OPERATIONS_H
#define LAB_03_03_MV_OPERATIONS_H

#include "matrix.h"
#include "vector.h"


void matrix_mul_vector(const matrix_t *matrix, const vector_t *vector, vector_t *result);

void sparse_matrix_mul_vector(const sparse_matrix_t *sparse_matrix, const vector_t *vector, vector_t *result);


#endif //LAB_03_03_MV_OPERATIONS_H
