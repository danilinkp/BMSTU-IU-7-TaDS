#include "matrix.h"
#include "vector.h"
#include "mv_operations.h"
#include "menu.h"
#include "timer.h"
#include <stdio.h>

int main(void)
{
    int rc;
    matrix_t matrix = {.rows = 0, .cols = 0};
    sparse_matrix_t sparse_matrix;
    vector_t vector = {.rows = 0, .num_non_zeros=0}, vector_result;

    size_t num_non_zeros;
    int action = -1;
    int choice, is_matrix_readen = 0, is_vector_readen = 0, is_mult = 0;

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
                read_matrix_size(&matrix.rows, &matrix.cols, &num_non_zeros, is_vector_readen, vector.rows);
                if (matrix_alloc(&matrix) != EXIT_SUCCESS)
                {
                    printf("Ошибка выделения памяти под матрицу.\n");
                    break;
                }
                sparse_matrix.rows = matrix.rows;
                sparse_matrix.cols = matrix.cols;
                sparse_matrix.num_non_zeros = num_non_zeros;
                if (sparse_matrix_alloc(&sparse_matrix) != EXIT_SUCCESS)
                {
                    printf("Ошибка выделения памяти под разреженную матрицу.\n");
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
                        printf("Выбран неверный пункт. Попробуйте еще раз.\n");
                        rc = WRONG_CHOICE_ERROR;
                        continue;
                    }
                    rc = EXIT_SUCCESS;
                } while (rc != EXIT_SUCCESS);
                if (choice == 1)
                    fill_matrix_with_coords(&matrix, &num_non_zeros);
                else if (choice == 2)
                    fill_matrix_with_rand_elems(&matrix, &num_non_zeros);
                printf("Матрица успешно введена\n");
                std_matrix_to_sparse(matrix, &sparse_matrix);
                is_matrix_readen = 1;
                break;
            case READ_VECTOR:
                choice = -1;
                read_vector_sizes(&vector.rows, &vector.num_non_zeros, is_matrix_readen, matrix.cols);
                if (vector_alloc(&vector) != EXIT_SUCCESS)
                {
                    printf("Ошибка выделения памяти под вектор-столбец.\n");
                    break;
                }
                rc = EXIT_SUCCESS;
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
                is_vector_readen = 1;
                break;
            case PRINT_MATRIX:
                if (!is_matrix_readen)
                {
                    printf("Матрица не введена, выводить нечего.\n");
                    break;
                }
                choice = -1;
                rc = EXIT_SUCCESS;
                do
                {
                    printf("Выберите в каком формате хотите вывести матрицу:\n"
                           "1) В стандартном виде\n"
                           "2) В форме хранения CSR\n");
                    if (scanf("%d", &choice) != 1 || choice < 1 || choice > 2)
                    {
                        printf("Выбран неверный пункт.\n");
                        rc = WRONG_CHOICE_ERROR;
                        continue;
                    }
                    rc = EXIT_SUCCESS;
                } while (rc != EXIT_SUCCESS);
                if (choice == 1)
                    print_matrix(matrix);
                else if (choice == 2)
                    print_sparse_matrix(sparse_matrix);
                break;
            case PRINT_VECTOR:
                if (!is_vector_readen)
                {
                    printf("Векор пуст. Выводить нечего.\n");
                    break;
                }
                choice = -1;
                rc = EXIT_SUCCESS;
                do
                {
                    printf("Выберите в каком формате хотите вывести вектор-столбец:\n"
                           "1) В стандартном виде\n"
                           "2) В виде разреженного вектора\n");
                    if (scanf("%d", &choice) != 1 || choice < 1 || choice > 2)
                    {
                        printf("Выбран неверный пункт.\n");
                        rc = WRONG_CHOICE_ERROR;
                        continue;
                    }
                    rc = EXIT_SUCCESS;
                } while (rc != EXIT_SUCCESS);

                if (choice == 1)
                    print_vector_std(vector);
                else if (choice == 2)
                    print_sparse_vector(vector);
                break;

            case MULTIPLICATION:
                if (!is_vector_readen || !is_matrix_readen)
                {
                    printf("Матрица или вектор не введены. Умножать нечего.\n");
                    break;
                }
                vector_result.rows = matrix.rows;
                vector_result.num_non_zeros = matrix.rows;
                choice = -1;
                rc = EXIT_SUCCESS;
                do
                {
                    printf("Выберите каким способом вы хотите произвести умножение:\n"
                           "1) Стандартное умножение матриц\n"
                           "2) Умножение разреженных матриц\n");
                    if (scanf("%d", &choice) != 1 || choice < 1 || choice > 2)
                    {
                        printf("Выбран неверный пункт.\n");
                        rc = WRONG_CHOICE_ERROR;
                        continue;
                    }
                    rc = EXIT_SUCCESS;
                } while (rc != EXIT_SUCCESS);
                if (vector_alloc(&vector_result) != EXIT_SUCCESS)
                {
                    printf("Ошибка выделения памяти под результат.\n");
                    break;
                }
                if (choice == 1)
                    matrix_mul_vector(&matrix, &vector, &vector_result);
                else if (choice == 2)
                    sparse_matrix_mul_vector(&sparse_matrix, &vector, &vector_result);
                is_mult = 1;
                del_zero_elements(&vector_result);
                printf("Результат умножения:\n");
                print_sparse_vector(vector_result);
                break;

            case COMPARING:
                print_measurements();
                break;

            default:
                printf("Выберите 1 из пунктов меню.\n");
                break;
        }
    }

    if (is_matrix_readen)
    {
        matrix_free(&matrix);
        sparse_matrix_free(&sparse_matrix);
    }
    if (is_vector_readen)
        vector_free(&vector);
    if (is_mult)
        vector_free(&vector_result);
    return EXIT_SUCCESS;
}
