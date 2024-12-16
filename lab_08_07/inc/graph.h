#ifndef LAB_08_07_GRAPH_H
#define LAB_08_07_GRAPH_H

#include <stdio.h>
#include <stdlib.h>

#define MAX_STR_LEN 256

typedef struct
{
    int **adjacency_matrix; // Матрица смежности
    int **next; // Матрица следующих вершин в пути
    size_t size; // Количество вершин в графе
} graph_t;

enum grah_errors
{
    FILE_READ_ERROR = 1,
    ELEM_READ_ERROR,
    FILE_OPEN_ERROR,
    MATRIX_ALLOC_ERROR,
    DOT_FILE_OPEN_ERROR,
};

// Очистка памяти, выделенной под граф
void graph_free(graph_t *graph);

// Выделение памяти под граф;
int graph_alloc(graph_t *graph);

// Считывание графа
int fread_graph(char *file_name, graph_t *graph);

// Поиск кратчайших путей для каждой пары вершин
void find_shortest_paths(graph_t *graph);

// Вывод кратчайшего пути для каждой пары вершин
void print_shortest_paths(graph_t graph);

// Вывод графа графически
int open_dot_img(const char *file_name, graph_t graph);


#endif //LAB_08_07_GRAPH_H
