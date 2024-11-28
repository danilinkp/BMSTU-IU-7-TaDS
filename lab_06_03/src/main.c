#include "tree.h"
#include "input_tools.h"
#include "menu.h"
#include "efficiency_performance.h"
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    tree_node_t *root = NULL;
    char file_name[MAX_STR_LEN + 1];
    char word[MAX_STR_LEN + 1];
    char *img_file_name = "my_tree";
    FILE *file = NULL;
    char letter[MAX_STR_LEN];
    int rc;
    int action = -1;
    while (action != EXIT)
    {
        menu_print();
        if (read_int(stdin, &action) != EXIT_SUCCESS)
        {
            printf("Ошибка: Пожалуйста, введите число.\n");
            continue;
        }
        if (action == EXIT)
            break;

        switch (action)
        {
            case BST_READ:
                printf("Введите имя файла: ");
                rc = read_str(stdin, file_name, MAX_STR_LEN);
                if (rc != EXIT_SUCCESS)
                {
                    printf("Ошибка ввода имени файла.\n");
                    break;
                }
                char data_file_path[2 * MAX_STR_LEN + 1];
                sprintf(data_file_path, "../data/%s", file_name);
                file = fopen(data_file_path, "r");
                if (!file)
                {
                    printf("Ошибка открытия файла или его не существует.\n");
                    break;
                }
                free_tree(root);
                root = NULL;
                rc = tree_read(file, &root);
                if (rc != EXIT_SUCCESS)
                {
                    fclose(file);
                    printf("Файл записан не правильно, или он пуст.\n");
                    break;
                }
                printf("Дерево успешно построено.\n");
                break;
            case BST_INSERT:
                do
                {
                    printf("Введите слово, которое хотите добавить: ");
                    rc = read_str(stdin, word, MAX_STR_LEN);
                    if (rc != EXIT_SUCCESS)
                    {
                        printf("Ошибка ввода. Попробуйте еще раз.\n");
                        continue;
                    }
                    rc = EXIT_SUCCESS;
                }
                while (rc != EXIT_SUCCESS);
                tree_node_t *new_node = create_node(word);
                if (!new_node)
                {
                    printf("Ошибка добавления элемента. Недостаточно памяти.\n");
                    break;
                }
                root = tree_node_insert(root, new_node);
                printf("Элемент успешно добавлен.\n");
                break;

            case BST_REMOVE:
                if (!root)
                {
                    printf("Дерево пусто, удалять нечего.\n");
                    break;
                }
                printf("Введите элемент который хотите удалить (регистр влияет): ");
                rc = read_str(stdin, word, MAX_STR_LEN);
                if (rc != EXIT_SUCCESS)
                {
                    printf("Ошибка ввода элемента.\n");
                    break;
                }
                tree_node_t *removed_node = tree_node_search(root, word);
                if (!removed_node)
                {
                    printf("Ошибка. Такой вершины нет.\n");
                    break;
                }
                root = tree_node_remove(root, removed_node);
                break;
            case BST_PRINT:
                if (!root)
                {
                    printf("Дерево пусто. Выводить нечего.\n");
                    break;
                }
                open_dot_img(img_file_name, root);
                break;
            case BST_REMOVE_BY_LETTER:
                if (!root)
                {
                    printf("Дерево пусто, удалять нечего.\n");
                    break;
                }
                printf("Введите букву, на которую хотите удалить слова(регистр влияет): ");
                rc = read_str(stdin, letter, MAX_STR_LEN);
                if (rc != EXIT_SUCCESS || strlen(letter) != 1)
                {
                    printf("Ошибка ввода буквы.\n");
                    break;
                }
                size_t removed_elems = 0;
                root = tree_remove_by_first_letter(root, letter[0], &removed_elems);
                if (!root)
                {
                    printf("Дерево удалилось полностью!\n");
                    break;
                }
                if (removed_elems == 0)
                {
                    printf("Ничего не удалилось.\n");
                    break;
                }
                printf("Удалено %zu вершин\n", removed_elems);
                break;

            case PRINT_MEASUREMENTS:
                print_measurements();
                break;
            default:
                printf("Выберите 1 из пунктов меню.\n");
                break;
        }
    }
    free_tree(root);
    return EXIT_SUCCESS;
}
