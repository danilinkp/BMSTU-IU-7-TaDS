#include "efficiency_performance.h"
#include "tree.h"
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

void remove_lines_by_letter(FILE *file, char letter)
{
    long write_pos = 0;

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    unsigned char *buffer = (unsigned char *) malloc(file_size);
    fread(buffer, 1, file_size, file);

    unsigned char *current_pos = buffer;
    unsigned char *word_start = buffer;

    while (current_pos < buffer + file_size)
    {
        if (*current_pos == ' ' || *current_pos == '\n' || current_pos == buffer + file_size - 1)
        {
            if (word_start[0] != letter)
            {
                size_t word_length = current_pos - word_start + 1;
                fseek(file, write_pos, SEEK_SET);
                fwrite(word_start, 1, word_length, file);
                write_pos += word_length;
            }

            if (*current_pos != ' ' && *current_pos != '\n')
                word_start = current_pos;
        }
        current_pos++;
    }

    ftruncate(fileno(file), write_pos);

    free(buffer);
}

void convert_text_to_binary(FILE *src, const char *binary_file)
{
    FILE *dst = fopen(binary_file, "wb");

    char ch;

    while ((ch = fgetc(src)) != EOF)
        fwrite(&ch, sizeof(char), 1, dst);

    fclose(dst);
}

int count_nodes(tree_node_t *root)
{
    if (root == NULL)
    {
        return 0;
    }
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

static void print_table_header(void)
{
    printf("| %-15s| %-16s| %-16s| %-16s| %-16s| %-16s|\n", "Num of elems", "Balanced", "Time sort", "Time search", "Time delete tree", "Time delete file");
    printf("|----------------|-----------------|-----------------|-----------------|-----------------|-----------------|\n");
}

static double tree_sort_time(tree_node_t **root, FILE *file, int count)
{
    unsigned long long beg, end;
    double sum = 0;
    for (int i = 0; i < count; i++)
    {
        beg = microseconds_now();
        tree_read(file, root);
        end = microseconds_now();
        sum += end - beg;
    }
    return sum / count;
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

double tree_delete_time(FILE *file, char letter, int count)
{
    unsigned long long beg, end;
    double sum = 0;
    size_t removed_elems = 0;
    for (int i = 0; i < count; i++)
    {
        rewind(file);
        tree_node_t *root_to_del = NULL;
        tree_read(file, &root_to_del);
        beg = microseconds_now();
        root_to_del = tree_remove_by_first_letter(root_to_del, letter, &removed_elems);
        end = microseconds_now();
        sum += end - beg;
        free_tree(root_to_del);
    }
    return sum / count;
}

double file_delete_time(FILE *file, char letter, int count)
{
    unsigned long long beg, end;
    double sum = 0;
    rewind(file);
    char *copy_bin_name = "../out/copy.bin";
    for (int i = 0; i < count; i++)
    {
        convert_text_to_binary(file, copy_bin_name);
        FILE *copy_bin = fopen(copy_bin_name, "r+b");
        beg = microseconds_now();
        remove_lines_by_letter(copy_bin, letter);
        end = microseconds_now();
        sum += end - beg;
        fclose(copy_bin);
    }
    return sum / count;
}

static void print_time(const char *file_name, int len)
{
    tree_node_t *root_b = NULL;
    tree_node_t *root_nb = NULL;

    char file_b_name[256];
    sprintf(file_b_name, "../data/b%s", file_name);
    char file_nb_name[256];
    sprintf(file_nb_name, "../data/nb%s", file_name);

    FILE *file_b = fopen(file_b_name, "r");
    FILE *file_nb = fopen(file_nb_name, "r");

    double time_sort_b = tree_sort_time(&root_b, file_b, NUM_OF_ITERATIONS);
    double time_sort_nb = tree_sort_time(&root_nb, file_nb, NUM_OF_ITERATIONS);

    double time_search_b = tree_search_time(root_b, NUM_OF_ITERATIONS);
    double time_search_nb = tree_search_time(root_nb, NUM_OF_ITERATIONS);

    char rand_letter = get_random(root_b)[0];
    double time_delete_root_b = tree_delete_time(file_b, rand_letter, NUM_OF_ITERATIONS);
    double time_delete_file_b = file_delete_time(file_b, rand_letter, NUM_OF_ITERATIONS);

    rand_letter = get_random(root_nb)[0];
    double time_delete_root_nb = tree_delete_time(file_nb, rand_letter, NUM_OF_ITERATIONS);
    double time_delete_file_nb = file_delete_time(file_nb, rand_letter, NUM_OF_ITERATIONS);

    printf("| %-15d| %-16s| %-16lf| %-16lf| %-16lf| %-16lf|\n", len, "Balanced", time_sort_b, time_search_b,
           time_delete_root_b, time_delete_file_b);
    printf("| %-15d| %-16s| %-16lf| %-16lf| %-16lf| %-16lf|\n", len, "Left", time_sort_nb, time_search_nb,
           time_delete_root_nb, time_delete_file_nb);
    printf("|----------------|-----------------|-----------------|-----------------|-----------------|-----------------|\n");

    free_tree(root_b);
    free_tree(root_nb);
    fclose(file_b);
    fclose(file_nb);
}

void print_measurements(void)
{
    char *files[] = { "_test_10.txt", "_test_20.txt", "_test_50.txt", "_test_75.txt", "_test_100.txt" };
    int lens[] = { 10, 20, 50, 75, 100 };
    print_table_header();
    for (size_t i = 0; i < 5; i++)
    {
        print_time(files[i], lens[i]);
    }
    printf("\n");
}

