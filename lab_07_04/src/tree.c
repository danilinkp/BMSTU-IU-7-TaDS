#include "tree.h"
#include "input_tools.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>

tree_node_t *create_node(char *str)
{
    tree_node_t *new_node = malloc(sizeof(tree_node_t));
    if (!new_node)
        return NULL;

    new_node->str = malloc((strlen(str) + 1) * sizeof(char));
    if (!new_node->str)
        return NULL;
    strcpy(new_node->str, str);
    new_node->parent = NULL;
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->height = 1;
    return new_node;
}

void free_node(tree_node_t *tree_node)
{
    if (tree_node->str)
    {
        free(tree_node->str);
        tree_node->str = NULL;
    }
    free(tree_node);
    tree_node = NULL;
}

void free_tree(tree_node_t *tree_node)
{
    if (!tree_node)
        return;

    free_tree(tree_node->left);
    free_tree(tree_node->right);
    free_node(tree_node);
}

static unsigned char height(tree_node_t *node)
{
    return node ? node->height : 0;
}

static int balance_factor(tree_node_t *node)
{
    if (!node)
        return 0;
    return height(node->left) - height(node->right);
}

static void update_height(tree_node_t *node)
{
    if (!node)
        return;
    unsigned char left_height = height(node->left);
    unsigned char right_height = height(node->right);
    node->height = (left_height > right_height ? left_height : right_height) + 1;
}


static tree_node_t *rotate_left(tree_node_t *node)
{
    tree_node_t *new_root = node->right;
    node->right = new_root->left;
    if (new_root->left)
        new_root->left->parent = node;
    new_root->left = node;
    node->parent = new_root;

    update_height(node);
    update_height(new_root);

    return new_root;
}

static tree_node_t *rotate_right(tree_node_t *node)
{
    tree_node_t *new_root = node->left;
    node->left = new_root->right;
    if (new_root->right)
        new_root->right->parent = node;
    new_root->right = node;
    node->parent = new_root;

    update_height(node);
    update_height(new_root);

    return new_root;
}

static tree_node_t *balance(tree_node_t *root)
{
    update_height(root);

    int balance = balance_factor(root);

    if (balance > 1)
    {
        if (balance_factor(root->left) < 0)
        {
            root->left = rotate_left(root->left);
            root->left->parent = root;
        }
        tree_node_t *parent = root->parent;
        root = rotate_right(root);
        root->parent = parent;
    }
    if (balance < -1)
    {
        if (balance_factor(root->right) > 0)
        {
            root->right = rotate_right(root->right);
            root->right->parent = root;
        }
        tree_node_t *parent = root->parent;
        root = rotate_left(root);
        root->parent = parent;
    }
    return root;
}

tree_node_t *tree_node_insert(tree_node_t *root, tree_node_t *node, int is_avl)
{
    if (!root)
        return node;

    if (strcmp(node->str, root->str) < 0)
    {
        root->left = tree_node_insert(root->left, node, is_avl);
        root->left->parent = root;
    }
    else if (strcmp(node->str, root->str) > 0)
    {
        root->right = tree_node_insert(root->right, node, is_avl);
        root->right->parent = root;
    }

    if (is_avl)
        return balance(root);

    return root;
}

int tree_read(FILE *file, tree_node_t **root_bst, tree_node_t **root_avl)
{
    size_t len = 0;
    tree_node_t *bst_node = NULL;
    tree_node_t *avl_node = NULL;
    char tmp_str[MAX_WORD_LEN + 1];
    while (read_str(file, tmp_str, MAX_WORD_LEN) == EXIT_SUCCESS)
    {
        bst_node = create_node(tmp_str);
        avl_node = create_node(tmp_str);
        if (!bst_node || !avl_node)
            return TREE_NODE_ALLOC_ERROR;
        *root_bst = tree_node_insert(*root_bst, bst_node, 0);
        *root_avl = tree_node_insert(*root_avl, avl_node, 1);
        len++;
        bst_node = NULL;
        avl_node = NULL;
    }
    if (!len)
        return FILE_EMPTY_ERROR;

    if (!feof(file))
        return FILE_READ_ERROR;

    return EXIT_SUCCESS;
}

tree_node_t *tree_node_search(tree_node_t *root, const char *str)
{
    tree_node_t *curr = root;
    while (curr)
    {
        if (strcmp(curr->str, str) == 0)
            return curr;
        else if (strcmp(curr->str, str) < 0)
            curr = curr->right;
        else
            curr = curr->left;
    }

    return NULL;
}

static tree_node_t *next(tree_node_t *removed_node)
{
    tree_node_t *next = removed_node->right;
    while (next->left)
        next = next->left;
    return next;
}

tree_node_t *tree_node_remove(tree_node_t *root, tree_node_t *removed_node, int is_avl)
{
    tree_node_t *parent = removed_node->parent;
    tree_node_t *balance_start = parent;

    // Удаляемая вершина - лист
    if (!removed_node->left && !removed_node->right)
    {
        if (!parent)
        {
            free_node(removed_node);
            return NULL;
        }
        else
        {
            if (parent->left == removed_node)
                parent->left = removed_node->left;
            else if (parent->right == removed_node)
                parent->right = NULL;
            if (removed_node == root)
                root = NULL;
            free_node(removed_node);
        }
    }

    // Удаляемая вершина имеет одного потомока
    else if (!removed_node->left || !removed_node->right)
    {
        if (!parent)
        {
            if (!removed_node->right)
            {
                root = removed_node->left;
                root->parent = NULL;
            }
            else
            {
                root = removed_node->right;
                removed_node->parent = NULL;
            }
        }
        else
        {
            if (!removed_node->right)
            {
                if (parent->right == removed_node)
                    parent->right = removed_node->left;
                else if (parent->left == removed_node)
                    parent->left = removed_node->left;
                removed_node->left->parent = parent;
                if (root == removed_node)
                    root = removed_node->left;
            }
            else
            {
                if (parent->right == removed_node)
                    parent->right = removed_node->right;
                else if (parent->left == removed_node)
                    parent->left = removed_node->right;
                removed_node->right->parent = parent;
                if (root == removed_node)
                    root = removed_node->right;
            }
        }
        free_node(removed_node);
    }

    // Удаляемая вершина имеет два потомка
    else
    {
        tree_node_t *successor = next(removed_node);
        strcpy(removed_node->str, successor->str);
        if (successor->parent == removed_node)
        {
            successor->parent->right = successor->right;
            if (successor->right)
                successor->right->parent = successor->parent;
        }
        else
        {
            successor->parent->left = successor->right;
            if (successor->right)
                successor->right->parent = successor->parent;
        }
        free_node(successor);
    }

    if (is_avl)
    {
        tree_node_t *current = balance_start;
        while (current)
        {
            update_height(current);
            current = balance(current);
            if (!current->parent)
                root = current;
            current = current->parent;
        }
    }
    return root;
}

tree_node_t *tree_remove_by_first_letter(tree_node_t *root, char letter, size_t *removed_elems, int is_avl)
{
    if (!root)
        return NULL;

    root->left = tree_remove_by_first_letter(root->left, letter, removed_elems, is_avl);
    root->right = tree_remove_by_first_letter(root->right, letter, removed_elems, is_avl);

    if (root->str[0] == letter)
    {
        root = tree_node_remove(root, root, is_avl);
        (*removed_elems)++;
    }
    return root;
}

void tree_preorder(tree_node_t *tree, void (*action)(tree_node_t *, void *), void *arg)
{
    if (tree == NULL)
        return;

    action(tree, arg);
    tree_preorder(tree->left, action, arg);
    tree_preorder(tree->right, action, arg);
}

static void to_dot(struct tree_node *tree, void *param)
{
    FILE *f = param;
    if (tree->left)
        fprintf(f, "\t\"%s\" -> \"%s\";\n", tree->str, tree->left->str);
    else
    {
        fprintf(f, "\t\"%s_left_null\" [shape=point, style=invis];\n", tree->str);
        fprintf(f, "\t\"%s\" -> \"%s_left_null\";\n", tree->str, tree->str);
    }
    if (tree->right)
        fprintf(f, "\t\"%s\" -> \"%s\";\n", tree->str, tree->right->str);
    else
    {
        fprintf(f, "\t\"%s_right_null\" [shape=point, style=invis];\n", tree->str);
        fprintf(f, "\t\"%s\" -> \"%s_right_null\";\n", tree->str, tree->str);
    }
}

void export_to_dot(FILE *f, char *tree_name, tree_node_t *root)
{
    fprintf(f, "digraph %s {\n"
               "center=true;\n"
               "\tnode [shape=oval, fontname=\"Cantarell\", fontsize=14];\n"
               "\tedge [fontsize=14];\n", tree_name);
    tree_preorder(root, to_dot, f);
    fprintf(f, "}\n");
}

int open_dot_img(const char *file_name, tree_node_t *root)
{
    const char *img_dir = "../img";
    const char *dot_dir = "../out";

    struct stat img_st = { 0 };
    if (stat(img_dir, &img_st) == -1)
        mkdir(img_dir, 0777);

    struct stat dot_st = { 0 };
    if (stat(dot_dir, &dot_st) == -1)
        mkdir(dot_dir, 0777);

    char dot_path[MAX_STR_LEN];
    sprintf(dot_path, "%s/%s.dot", dot_dir, file_name);

    FILE *dot_file = fopen(dot_path, "w");
    if (!dot_file)
        return FILE_OPEN_ERROR;

    export_to_dot(dot_file, "my_tree", root);
    fclose(dot_file);

    char command[MAX_STR_LEN];
    sprintf(command, "dot -Tpng %s/%s.dot -o %s/%s.png", dot_dir, file_name, img_dir, file_name);
    system(command);
    sprintf(command, "xdg-open %s/%s.png", img_dir, file_name);
    system(command);

    return EXIT_SUCCESS;
}
