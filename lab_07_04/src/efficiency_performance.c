#include "efficiency_performance.h"
#include "tree.h"
#include "hash_table.h"
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int count_nodes(tree_node_t *root)
{
    if (root == NULL)
        return 0;
    return 1 + count_nodes(root->left) + count_nodes(root->right);
}

char *in_order_traversal(tree_node_t *node, int *current_index, int random_index)
{
    if (node == NULL)
        return NULL;

    char *left_result = in_order_traversal(node->left, current_index, random_index);
    if (left_result != NULL)
        return left_result;

    (*current_index)++;
    if (*current_index == random_index)
        return node->str;

    return in_order_traversal(node->right, current_index, random_index);
}

char *get_random(tree_node_t *root)
{
    int total_nodes = count_nodes(root);

    if (total_nodes == 0)
        return NULL;

    int random_index = rand() % total_nodes + 1;
    int current_index = 0;

    return in_order_traversal(root, &current_index, random_index);
}

static unsigned long long microseconds_now(void)
{
    struct timeval value;

    if (gettimeofday(&value, NULL))
        return (unsigned long long) -1;

    return value.tv_sec * 1000ULL * 1000ULL + value.tv_usec;
}

static void print_table_search_header(void)
{
    printf("Таблица среденего времени поиска:\n");
    printf("| %-15s| %-16s| %-16s| %-22s| %-22s|\n", "Num of elems", "Usual tree time", "AVL tree time", "Open hash table time", "Closed hash table time");
    printf("|----------------|-----------------|-----------------|-----------------------|-----------------------|\n");
}

static void print_table_compares_header(void)
{
    printf("\nТаблица среднего кол-ва сравнений:\n");
    printf("| %-15s| %-16s| %-16s| %-22s| %-22s|\n", "Num of elems", "Usual tree cmps", "AVL tree cmps", "Open hash table cmps", "Closed hash table cmps");
    printf("|----------------|-----------------|-----------------|-----------------------|-----------------------|\n");
}

static void print_table_del_header(void)
{
    printf("\nТаблица времени удаления начинающихся на заданную букву:\n");
    printf("| %-15s| %-16s| %-16s| %-22s| %-22s|\n", "Num of elems", "Usual tree del", "AVL tree del", "Open hash table del", "Closed hash table del");
    printf("|----------------|-----------------|-----------------|-----------------------|-----------------------|\n");
}

static void print_table_memory_header(void)
{
    printf("\nТаблица времени удаления начинающихся на заданную букву:\n");
    printf("| %-15s| %-16s| %-16s| %-22s| %-22s|\n", "Num of elems", "Usual tree mem", "AVL tree mem", "Open hash table mem", "Closed hash table mem");
    printf("|----------------|-----------------|-----------------|-----------------------|-----------------------|\n");
}

double tree_search_time(tree_node_t *root, int count)
{
    unsigned long long beg, end;
    double sum = 0;
    for (int i = 0; i < count; i++)
    {
        char *rand_str = get_random(root);
        beg = microseconds_now();
        tree_node_search(root, rand_str);
        end = microseconds_now();
        sum += end - beg;
    }
    return sum / count;
}

double tree_search_cmps(tree_node_t *root, int count)
{
    int cmps = 0;
    for (int i = 0; i < count; i++)
    {
        char *rand_str = get_random(root);
        cmps += tree_node_search_cmps(root, rand_str);
    }
    return (double) cmps / count;
}

double open_ht_search_time(open_hash_table_t *open_hash_table, tree_node_t *root, double *cmps, int count)
{
    unsigned long long beg, end;
    double sum = 0;
    *cmps = 0;
    for (int i = 0; i < count; i++)
    {
        char *rand_str = get_random(root);
        beg = microseconds_now();
        search_open_ht(open_hash_table, rand_str);
        end = microseconds_now();
        *cmps += cmps_search_open_ht(open_hash_table, rand_str);
        sum += end - beg;
    }
    *cmps /= count;
    return sum / count;
}

double closed_ht_search_time(closed_hash_table_t *closed_hash_table, tree_node_t *root, double *cmps, int count)
{
    unsigned long long beg, end;
    double sum = 0;
    *cmps = 0;
    for (int i = 0; i < count; i++)
    {
        char *rand_str = get_random(root);
        beg = microseconds_now();
        search_closed_ht(closed_hash_table, rand_str);
        end = microseconds_now();
        sum += end - beg;
        *cmps += cmps_search_closed_ht(closed_hash_table, rand_str);
    }
    *cmps /= count;
    return sum / count;
}

double bst_tree_delete_time(FILE *file, char letter, int count)
{
    unsigned long long beg, end;
    double sum = 0;
    size_t removed_elems = 0;
    for (int i = 0; i < count; i++)
    {
        rewind(file);
        tree_t bst_tree = { .root=NULL, .is_avl=0 };
        tree_t avl_tree = { .root=NULL, .is_avl=1 };
        tree_read(file, &bst_tree.root, &avl_tree.root);
        beg = microseconds_now();
        bst_tree.root = tree_remove_by_first_letter(bst_tree.root, letter, &removed_elems, 0);
        end = microseconds_now();
        sum += end - beg;
        free_tree(bst_tree.root);
        free_tree(avl_tree.root);
    }
    return sum / count;
}

double avl_tree_delete_time(FILE *file, char letter, int count)
{
    unsigned long long beg, end;
    double sum = 0;
    size_t removed_elems = 0;
    for (int i = 0; i < count; i++)
    {
        rewind(file);
        tree_t bst_tree = { .root=NULL, .is_avl=0 };
        tree_t avl_tree = { .root=NULL, .is_avl=1 };
        tree_read(file, &bst_tree.root, &avl_tree.root);
        beg = microseconds_now();
        avl_tree.root = tree_remove_by_first_letter(avl_tree.root, letter, &removed_elems, 1);
        end = microseconds_now();
        sum += end - beg;
        free_tree(bst_tree.root);
        free_tree(avl_tree.root);
    }
    return sum / count;
}

double open_ht_del_time(FILE *file, char letter, int count)
{
    unsigned long long beg, end;
    double sum = 0;
    open_hash_table_t hash_table;
    for (int i = 0; i < count; i++)
    {
        create_open_ht(&hash_table);
        fread_open_ht(file, &hash_table);
        beg = microseconds_now();
        del_by_first_letter_open_ht(&hash_table, letter);
        end = microseconds_now();
        sum += end - beg;
        clear_open_ht_data(&hash_table);
    }
    return sum / count;
}

double closed_ht_del_time(FILE *file, char letter, int count)
{
    unsigned long long beg, end;
    double sum = 0;
    closed_hash_table_t hash_table;
    for (int i = 0; i < count; i++)
    {
        create_closed_ht(&hash_table);
        fread_closed_ht(file, &hash_table);
        beg = microseconds_now();
        del_by_first_letter_closed_ht(&hash_table, letter);
        end = microseconds_now();
        sum += end - beg;
        clear_closed_ht_data(&hash_table);
    }
    return sum / count;
}

static void print_search_time(const char *file_name, int len)
{
    tree_t bst_tree = { .root = NULL, .is_avl = 0 };
    tree_t avl_tree = { .root = NULL, .is_avl = 1 };

    open_hash_table_t open_hash_table;
    create_open_ht(&open_hash_table);
    closed_hash_table_t closed_hash_table;
    create_closed_ht(&closed_hash_table);

    char file_test_name[256];
    sprintf(file_test_name, "../data/%s", file_name);

    FILE *file = fopen(file_test_name, "r");

    tree_read(file, &bst_tree.root, &avl_tree.root);
    fread_open_ht(file, &open_hash_table);
    fread_closed_ht(file, &closed_hash_table);

    double time_search_b = tree_search_time(avl_tree.root, NUM_OF_ITERATIONS);
    double time_search_nb = tree_search_time(bst_tree.root, NUM_OF_ITERATIONS);

    double cmps_search_open_ht;
    double cmps_search_closed_ht;
    double time_search_open_ht = open_ht_search_time(&open_hash_table, bst_tree.root, &cmps_search_open_ht, NUM_OF_ITERATIONS);
    double time_search_closed_ht = closed_ht_search_time(&closed_hash_table, bst_tree.root, &cmps_search_closed_ht, NUM_OF_ITERATIONS);

    printf("| %-15d| %-16lf| %-16lf| %-22lf| %-22lf|\n", len, time_search_nb, time_search_b, time_search_open_ht, time_search_closed_ht);
    printf("|----------------|-----------------|-----------------|-----------------------|-----------------------|\n");

    free_tree(bst_tree.root);
    free_tree(avl_tree.root);
    clear_open_ht_data(&open_hash_table);
    clear_closed_ht_data(&closed_hash_table);
}

static void print_cmps(const char *file_name, int len)
{
    tree_t bst_tree = { .root = NULL, .is_avl = 0 };
    tree_t avl_tree = { .root = NULL, .is_avl = 1 };

    open_hash_table_t open_hash_table;
    create_open_ht(&open_hash_table);
    closed_hash_table_t closed_hash_table;
    create_closed_ht(&closed_hash_table);

    char file_test_name[256];
    sprintf(file_test_name, "../data/%s", file_name);

    FILE *file = fopen(file_test_name, "r");

    tree_read(file, &bst_tree.root, &avl_tree.root);
    fread_open_ht(file, &open_hash_table);
    fread_closed_ht(file, &closed_hash_table);

    double cmps_search_open_ht;
    double cmps_search_closed_ht;
    open_ht_search_time(&open_hash_table, avl_tree.root, &cmps_search_open_ht, NUM_OF_ITERATIONS);
    closed_ht_search_time(&closed_hash_table, avl_tree.root, &cmps_search_closed_ht, NUM_OF_ITERATIONS);

    double cmps_search_b = tree_search_cmps(avl_tree.root, NUM_OF_ITERATIONS);
    double cmps_search_nb = tree_search_cmps(bst_tree.root, NUM_OF_ITERATIONS);

    printf("| %-15d| %-16lf| %-16lf| %-22lf| %-22lf|\n", len, cmps_search_nb, cmps_search_b, cmps_search_open_ht, cmps_search_closed_ht);
    printf("|----------------|-----------------|-----------------|-----------------------|-----------------------|\n");

    free_tree(bst_tree.root);
    free_tree(avl_tree.root);
    clear_open_ht_data(&open_hash_table);
    clear_closed_ht_data(&closed_hash_table);
}

static void print_del_time(const char *file_name, int len)
{
    tree_t bst_tree = { .root = NULL, .is_avl = 0 };
    tree_t avl_tree = { .root = NULL, .is_avl = 1 };

    char file_test_name[256];
    sprintf(file_test_name, "../data/%s", file_name);

    FILE *file = fopen(file_test_name, "r");

    tree_read(file, &bst_tree.root, &avl_tree.root);
    char rand_letter = get_random(bst_tree.root)[0];

    free_tree(bst_tree.root);
    free_tree(avl_tree.root);

    double time_delete_tree_b = avl_tree_delete_time(file, rand_letter, 100);
    double time_delete_tree_nb = bst_tree_delete_time(file, rand_letter, 100);

    double time_delete_open_ht = open_ht_del_time(file, rand_letter, 100);
    double time_delete_closed_ht = closed_ht_del_time(file, rand_letter, 100);

    printf("| %-15d| %-16lf| %-16lf| %-22lf| %-22lf|\n", len, time_delete_tree_nb, time_delete_tree_b, time_delete_open_ht, time_delete_closed_ht);
    printf("|----------------|-----------------|-----------------|-----------------------|-----------------------|\n");
}

static void print_memory(int len)
{
    size_t bst_tree_mem_size = len * sizeof(tree_node_t);
    size_t avl_tree_mem_size = len * sizeof(tree_node_t);

    size_t open_ht_mem_size = len * sizeof(key_node_t);
    size_t closed_ht_mem_size = len * sizeof(key_t);

    printf("| %-15d| %-16zu| %-16zu| %-22zu| %-22zu|\n", len, bst_tree_mem_size, avl_tree_mem_size, open_ht_mem_size, closed_ht_mem_size);
    printf("|----------------|-----------------|-----------------|-----------------------|-----------------------|\n");
}

void print_measurements(void)
{
    char *files[] = { "test_10.txt", "test_50.txt", "test_75.txt", "test_100.txt", "test_150.txt", "test_250.txt",
                      "test_300.txt" };
    int lens[] = { 10, 50, 75, 100, 150, 250, 300 };

    print_table_search_header();
    for (size_t i = 0; i < 7; i++)
        print_search_time(files[i], lens[i]);

    print_table_compares_header();
    for (size_t i = 0; i < 7; i++)
        print_cmps(files[i], lens[i]);

    print_table_del_header();
    for (size_t i = 0; i < 7; i++)
        print_del_time(files[i], lens[i]);

    print_table_memory_header();
    for (size_t i = 0; i < 7; i++)
        print_memory(lens[i]);

    printf("\n");
}

