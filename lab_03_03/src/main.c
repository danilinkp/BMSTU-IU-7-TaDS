#include "matrix.h"
#include "vector.h"
#include "mv_operations.h"
#include "menu.h"
#include <stdio.h>

int main(void)
{
    int rc;
    matrix_t matrix = {.rows = 0, .cols = 0};
    sparse_matrix_t sparse_matrix;
    vector_t vector = {.rows = 0, .num_non_zeros=0}, vector_result;

    size_t num_non_zeros;
    int action = -1;
    int choice, is_matrix_readed = 0, is_vector_readed = 0;

    while (action != EXIT)
    {
        menu_print();
        if (scanf("%d", &action) != 1)
        {
            printf("Ошибка: Пожалуйста, введите число.\\n");
            continue;
        }

        if (action == EXIT)
            break;
        else if (action == READ_MATRIX)
        {

        }
        switch (action)
        {
            case EXIT:
                break;
            case READ_MATRIX:
                read_matrix_size(&matrix.rows, &matrix.cols, &num_non_zeros, is_vector_readed, vector.rows);
                if (matrix_alloc(&matrix) != EXIT_SUCCESS)
                {
                    printf("Ошибка выделения памяти под матрицу.\n");
                    break;
                }
                choice = -1;
                rc = EXIT_SUCCESS;
                do
                {
                    printf("Выберите каким способом вы хотите заполнить матрицу:\n"
                           "1) Ввод с консоли\n"
                           "2) Рандомное заполнение\n");
                    if (scanf("%d", &choice) != 1 || choice < 1 || choice > 2)
                    {
                        printf("Выбран неверный пункт\n");
                        rc = WRONG_CHOICE_ERROR;
                        continue;
                    }
                    rc = EXIT_SUCCESS;
                } while (rc != EXIT_SUCCESS);
                if (choice == 1)
                    fill_matrix_with_coords(&matrix, &num_non_zeros);
                else if (choice == 2)
                    fill_matrix_with_rand_elems(&matrix, &num_non_zeros);
                std_matrix_to_sparse(matrix, &sparse_matrix);
                is_matrix_readed = 1;
                break;
            case READ_VECTOR:
                choice = -1;
                read_vector_sizes(&vector.rows, &vector.num_non_zeros, is_matrix_readed, matrix.cols);
                if (vector_alloc(&vector) != EXIT_SUCCESS)
                {
                    printf("Ошибка выделения памяти под вектор-столбец.\n");
                    break;
                }
                do
                {
                    printf("Выберите каким способом вы хотите заполнить вектор-столбец:\n"
                           "1) Ввод с консоли\n"
                           "2) Рандомное заполнение\n");
                    if (scanf("%d", &choice) != 1 || choice < 1 || choice > 2)
                    {
                        printf("Выбран неверный пункт.\n");
                        rc = WRONG_CHOICE_ERROR;
                        continue;
                    }
                    rc = EXIT_SUCCESS;
                } while (rc != EXIT_SUCCESS);
                if (choice == 1)
                    read_vector(&vector);
                else if (choice == 2)
                    fill_vector_rand(&vector);
                is_vector_readed = 1;
                break;
            case PRINT_MATRIX:
                break;
            case PRINT_VECTOR:
                break;
            case COMPARING:
                break;
            default:
                printf("Выберите 1 из пунктов меню.\n");
                break;
        }
    }
    if (is_matrix_readed)
    {
        matrix_free(&matrix);
        sparse_matrix_free(&sparse_matrix);
    }
    if (is_vector_readed)
    {
        vector_free(&vector);
    }
    return EXIT_SUCCESS;
}
