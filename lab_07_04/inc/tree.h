#ifndef LAB_07_04_TREE_H
#define LAB_07_04_TREE_H

#include <stdio.h>

#define MAX_WORD_LEN 256

typedef struct tree_node
{
    char *str; //Данные
    struct tree_node *parent; // Указатель на родителя
    struct tree_node *left; // Указатель на левого потомка
    struct tree_node *right; // Указатель на правого потомка
    unsigned char height; // Высота поддерева
} tree_node_t;

typedef struct {
    tree_node_t *root;
    int is_avl; // 1 для AVL, 0 для обычного дерева
} tree_t;

typedef enum
{
    TREE_NODE_ALLOC_ERROR = 1,
    TREE_NODE_READ_ERROR,
    FILE_EMPTY_ERROR,
    FILE_READ_ERROR,
    FILE_OPEN_ERROR,
} tree_errors_t;

// Очищение памяти, ввыделенной под дерево
void free_tree(tree_node_t *tree_node);

// Создание вершины дерева
tree_node_t *create_node(char *str);

// Вставка вершины в дерево
tree_node_t *tree_node_insert(tree_node_t *root, tree_node_t *node, int is_balanced);

// Чтение дерева из файла
int tree_read(FILE *file, tree_node_t **root_bst, tree_node_t **root_avl);

// Поиск вершины в дереве
tree_node_t *tree_node_search(tree_node_t *root, const char *str);

// Удаление вершины из дерева
tree_node_t *tree_node_remove(tree_node_t *root, tree_node_t *removed_node, int is_avl);

// Удаление всех вершин, начинающихся на задааную букву
tree_node_t *tree_remove_by_first_letter(tree_node_t *root, char letter, size_t *removed_elems, int is_avl);

// Вывод дерева на экран
int open_dot_img(const char *file_name, tree_node_t *root);

#endif //LAB_07_04_TREE_H
