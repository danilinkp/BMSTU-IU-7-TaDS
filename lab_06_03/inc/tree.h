#ifndef LAB_06_03_TREE_H
#define LAB_06_03_TREE_H

#include <stdio.h>

#define MAX_WORD_LEN 256

typedef struct tree_node
{
    char *str;
    struct tree_node *parent;
    struct tree_node *left;
    struct tree_node *right;
} tree_node_t;

typedef enum
{
    TREE_NODE_ALLOC_ERROR = 1,
    TREE_NODE_READ_ERROR,
    FILE_EMPTY_ERROR,
    FILE_READ_ERROR,
    FILE_OPEN_ERROR,
} tree_errors_t;

typedef enum
{
    PREORDER = 1,
    INORDER,
    POSTORDER
} order_t;

// Очищение памяти, ввыделенной под дерево
void free_tree(tree_node_t *tree_node);

// Создание вершины дерева
tree_node_t *create_node(char *str);

// Вставка вершины в дерево
tree_node_t *tree_node_insert(tree_node_t *root, tree_node_t *node);

// Чтение дерева из файла
int tree_read(FILE *file, tree_node_t **root);

// Поиск вершины в дереве
tree_node_t *tree_node_search(tree_node_t *root, const char *str);

// Удаление вершины из дерева
tree_node_t *tree_node_remove(tree_node_t *root, tree_node_t *removed_node);

// Удаление всех вершин, начинающихся на задааную букву
tree_node_t *tree_remove_by_first_letter(tree_node_t *root, char letter, size_t *removed_elems);

// Вывод дерева на экран
int open_dot_img(const char *file_name, tree_node_t *root);


#endif //LAB_06_03_TREE_H
