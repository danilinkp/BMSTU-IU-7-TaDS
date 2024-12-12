#include "tree.h"
#include "hash_table.h"
#include "input_tools.h"
#include "efficiency_performance.h"
#include <stdio.h>
#include <stdlib.h>

void menu_print(void)
{
    printf("\n");
    printf("========================================\n");
    printf("Программа для работы с деревом поиска и хеш-таблицей.\n");
    printf("Пункты меню:\n");
    printf("1) Построить бинарное дерево из файла\n");
    printf("2) Добавить слово в бинарное дерево\n");
    printf("3) Извлечь элемент из бинарного дерева\n");
    printf("4) Найти элемент в дереве.\n");
    printf("5) Удалить все слова, начинающиеся на указанную букву в бинарном дереве\n");
    printf("6) Вывести бинарное дерево на экран\n");
    printf("7) Построить хэш таблицу из файла\n");
    printf("8) Добавить слово в хеш-таблицу\n");
    printf("9) Удалить слово из хеш-таблицы\n");
    printf("10) Найти слово в хеш-таблице\n");
    printf("11) Удалить все слова, начинающиеся на указанную букву в хеш-таблице\n");
    printf("12) Вывести хеш-таблицу\n");
    printf("13) Вывести замеры эффективности\n");
    printf("0) Завершить работу программы\n");
    printf("========================================\n");
    printf("Введите пункт: ");
}

typedef enum
{
    EXIT,
    BST_READ,
    BST_INSERT,
    BST_REMOVE,
    BST_SEARCH,
    BST_REMOVE_BY_LETTER,
    BST_PRINT,
    HT_READ,
    HT_INSERT,
    HT_DEL,
    HT_SEARCH,
    HT_DEL_BY_LET,
    HT_PRINT,
    PRINT_MEASUREMENTS
} menu_t;

int main(void)
{
    tree_t bst_tree = { .root=NULL, .is_avl=0 };
    tree_t avl_tree = { .root=NULL, .is_avl=1 };

    open_hash_table_t open_hash_table;
    create_open_ht(&open_hash_table);
    closed_hash_table_t closed_hash_table;
    create_closed_ht(&closed_hash_table);

    char file_name[MAX_STR_LEN + 1];
    char word[MAX_STR_LEN + 1];
    char *bst_file_name = "bst_tree";
    char *avl_file_name = "avl_tree";
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
                free_tree(bst_tree.root);
                free_tree(avl_tree.root);
                bst_tree.root = NULL;
                avl_tree.root = NULL;
                rc = tree_read(file, &bst_tree.root, &avl_tree.root);
                if (rc != EXIT_SUCCESS)
                {
                    fclose(file);
                    printf("Файл записан не правильно, или он пуст.\n");
                    break;
                }
                printf("Деревья успешно построены.\n");
                fclose(file);
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
                tree_node_t *new_bst_node = create_node(word);
                tree_node_t *new_avl_node = create_node(word);
                if (!new_bst_node || !new_avl_node)
                {
                    printf("Ошибка добавления элемента. Недостаточно памяти.\n");
                    break;
                }
                bst_tree.root = tree_node_insert(bst_tree.root, new_bst_node, bst_tree.is_avl);
                avl_tree.root = tree_node_insert(avl_tree.root, new_avl_node, avl_tree.is_avl);
                printf("Элемент успешно добавлен.\n");
                break;
            case BST_SEARCH:
                if (!bst_tree.root || !avl_tree.root)
                {
                    printf("Дерево пусто, искать нечего.\n");
                    break;
                }
                printf("Введите элемент который хотите найти (регистр влияет): ");
                rc = read_str(stdin, word, MAX_STR_LEN);
                if (rc != EXIT_SUCCESS)
                {
                    printf("Ошибка ввода элемента.\n");
                    break;
                }
                tree_node_t *searched_node = tree_node_search(bst_tree.root, word);
                if (!searched_node)
                {
                    printf("Такой вершины нет.\n");
                    break;
                }
                printf("Вершина найдена.\n");
                break;
            case BST_REMOVE:
                if (!bst_tree.root || !avl_tree.root)
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
                tree_node_t *removed_node_bst = tree_node_search(bst_tree.root, word);
                tree_node_t *removed_node_avl = tree_node_search(avl_tree.root, word);
                if (!removed_node_bst || !removed_node_avl)
                {
                    printf("Ошибка. Такой вершины нет.\n");
                    break;
                }
                bst_tree.root = tree_node_remove(bst_tree.root, removed_node_bst, bst_tree.is_avl);
                avl_tree.root = tree_node_remove(avl_tree.root, removed_node_avl, avl_tree.is_avl);
                break;
            case BST_PRINT:
                if (!bst_tree.root || !avl_tree.root)
                {
                    printf("Дерево пусто. Выводить нечего.\n");
                    break;
                }
                open_dot_img(bst_file_name, bst_tree.root);
                open_dot_img(avl_file_name, avl_tree.root);
                break;
            case BST_REMOVE_BY_LETTER:
                if (!bst_tree.root || !avl_tree.root)
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
                bst_tree.root = tree_remove_by_first_letter(bst_tree.root, letter[0], &removed_elems, bst_tree.is_avl);
                avl_tree.root = tree_remove_by_first_letter(avl_tree.root, letter[0], &removed_elems, avl_tree.is_avl);
                if (!bst_tree.root || !avl_tree.root)
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

            case HT_READ:
                printf("Введите имя файла: ");
                rc = read_str(stdin, file_name, MAX_STR_LEN);
                if (rc != EXIT_SUCCESS)
                {
                    printf("Ошибка ввода имени файла.\n");
                    break;
                }
                sprintf(data_file_path, "../data/%s", file_name);
                file = fopen(data_file_path, "r");
                if (!file)
                {
                    printf("Ошибка открытия файла или его не существует.\n");
                    break;
                }
                clear_open_ht_data(&open_hash_table);
                clear_closed_ht_data(&closed_hash_table);

                create_open_ht(&open_hash_table);
                create_closed_ht(&closed_hash_table);
                rc = fread_open_ht(file, &open_hash_table);
                if (rc != EXIT_SUCCESS)
                {
                    fclose(file);
                    printf("Файл записан не правильно, или он пуст.\n");
                    break;
                }
                rc = fread_closed_ht(file, &closed_hash_table);
                if (rc != EXIT_SUCCESS)
                {
                    fclose(file);
                    printf("Файл записан не правильно, или он пуст.\n");
                    break;
                }
                printf("Хеш таблицы успешно построены.\n");
                break;
            case HT_INSERT:
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

                insert_open_ht(&open_hash_table, word);
                insert_closed_ht(&closed_hash_table, word);
                printf("Элемент успешно добавлен.\n");
                break;
            case HT_DEL:
                if (!open_hash_table.count || !closed_hash_table.count)
                {
                    printf("Хеш таблицы пусты, удалять нечего.\n");
                    break;
                }
                printf("Введите элемент который хотите удалить (регистр влияет): ");
                rc = read_str(stdin, word, MAX_STR_LEN);
                if (rc != EXIT_SUCCESS)
                {
                    printf("Ошибка ввода элемента.\n");
                    break;
                }
                ht_key_t removed_key = search_open_ht(&open_hash_table, word);
                if (!removed_key)
                {
                    printf("Ошибка. Такого слова нет.\n");
                    break;
                }
                delete_open_ht(&open_hash_table, word);
                delete_closed_ht(&closed_hash_table, word);
                break;
            case HT_SEARCH:
                if (!open_hash_table.count || !closed_hash_table.count)
                {
                    printf("Хеш-таблицы пусты, удалять нечего.\n");
                    break;
                }
                printf("Введите элемент который хотите удалить (регистр влияет): ");
                rc = read_str(stdin, word, MAX_STR_LEN);
                if (rc != EXIT_SUCCESS)
                {
                    printf("Ошибка ввода элемента.\n");
                    break;
                }
                ht_key_t searched_key = search_open_ht(&open_hash_table, word);
                if (!searched_key)
                {
                    printf("Такого слова в хеш-таблице нет.\n");
                    break;
                }
                printf("Слово в хеш таблице есть.\n");
                break;
            case HT_DEL_BY_LET:
                if (!open_hash_table.count || !closed_hash_table.count)
                {
                    printf("Хеш-таблицы пусты, удалять нечего.\n");
                    break;
                }
                printf("Введите букву, на которую хотите удалить слова(регистр влияет): ");
                rc = read_str(stdin, letter, MAX_STR_LEN);
                if (rc != EXIT_SUCCESS || strlen(letter) != 1)
                {
                    printf("Ошибка ввода буквы.\n");
                    break;
                }
                del_by_first_letter_open_ht(&open_hash_table, letter[0]);
                del_by_first_letter_closed_ht(&closed_hash_table, letter[0]);
                if (!open_hash_table.count || !closed_hash_table.count)
                {
                    printf("Таблица удалилась полностью.\n");
                    break;
                }
                printf("Элементы удалились.\n");
                break;
            case HT_PRINT:
                if (!open_hash_table.count || !closed_hash_table.count)
                {
                    printf("Хеш-таблицы пусты, выводить нечего.\n");
                    break;
                }
                print_open_ht(&open_hash_table);
                print_closed_ht(&closed_hash_table);
                break;
            case PRINT_MEASUREMENTS:
                print_measurements();
                break;
            default:
                printf("Выберите 1 из пунктов меню.\n");
                break;
        }
    }
    free_tree(bst_tree.root);
    free_tree(avl_tree.root);
    clear_open_ht_data(&open_hash_table);
    clear_closed_ht_data(&closed_hash_table);
    return EXIT_SUCCESS;
}
