#include "graph.h"
#include "input_tools.h"
#include <sys/stat.h>

void graph_free(graph_t *graph)
{
    for (size_t i = 0; i < graph->size; i++)
    {
        free(graph->adjacency_matrix[i]);
        free(graph->next[i]);
    }
    free(graph->adjacency_matrix);
    free(graph->next);
}

int graph_alloc(graph_t *graph)
{
    graph->adjacency_matrix = calloc(graph->size, sizeof(int *));
    if (!graph->adjacency_matrix)
        return MATRIX_ALLOC_ERROR;
    graph->next = calloc(graph->size, sizeof(int *));
    if (!graph->next)
    {
        free(graph->adjacency_matrix);
        return MATRIX_ALLOC_ERROR;
    }

    for (size_t i = 0; i < graph->size; i++)
    {
        graph->adjacency_matrix[i] = calloc(graph->size, sizeof(int));
        graph->next[i] = calloc(graph->size, sizeof(int));
        if (!graph->adjacency_matrix[i] || !graph->next[i])
        {
            graph_free(graph);
            return MATRIX_ALLOC_ERROR;
        }
    }

    return EXIT_SUCCESS;
}

int fread_graph(char *file_name, graph_t *graph)
{
    char data_file_path[2 * MAX_STR_LEN + 1];
    sprintf(data_file_path, "../data/%s", file_name);
    FILE *file = fopen(data_file_path, "r");
    if (!file)
        return FILE_OPEN_ERROR;

    if (fscanf(file, "%zu", &graph->size) != 1 || graph->size <= 0)
    {
        fclose(file);
        return FILE_READ_ERROR;
    }

    int rc = graph_alloc(graph);
    if (rc != EXIT_SUCCESS)
    {
        fclose(file);
        return rc;
    }

    for (size_t i = 0; i < graph->size; i++)
        for (size_t j = 0; j < graph->size; j++)
            if (fscanf(file, "%d", &graph->adjacency_matrix[i][j]) != 1)
            {
                fclose(file);
                return ELEM_READ_ERROR;
            }

    fclose(file);
    return EXIT_SUCCESS;
}

void find_shortest_paths(graph_t *graph)
{
    for (size_t i = 0; i < graph->size; i++)
        for (size_t j = 0; j < graph->size; j++)
            if (graph->adjacency_matrix[i][j] != 0)
                graph->next[i][j] = (int) j;
            else
                graph->next[i][j] = -1;

    for (size_t k = 0; k < graph->size; ++k)
        for (size_t i = 0; i < graph->size; ++i)
            for (size_t j = 0; j < graph->size; ++j)
                if (i != j && graph->adjacency_matrix[i][k] != 0 && graph->adjacency_matrix[k][j] != 0)
                {
                    int new_dist = graph->adjacency_matrix[i][k] + graph->adjacency_matrix[k][j];
                    if (graph->adjacency_matrix[i][j] == 0 || new_dist < graph->adjacency_matrix[i][j])
                    {
                        graph->adjacency_matrix[i][j] = new_dist;
                        graph->next[i][j] = graph->next[i][k];
                    }
                }
}

static void print_shortest_path(graph_t graph, size_t i, size_t j)
{
    if (graph.adjacency_matrix[i][j] == 0)
    {
        printf("Между вершинами %zu и %zu нет пути\n", i, j);
        return;
    }

    printf("Кратчайший путь от %zu до %zu:\n", i, j);
    size_t start = i;

    while (start != j)
    {
        printf("%zu -> ", start);
        start = graph.next[start][j];
    }
    printf("%zu = %d\n", j, graph.adjacency_matrix[i][j]);
}

void print_shortest_paths(graph_t graph)
{
    for (size_t i = 0; i < graph.size; i++)
        for (size_t j = 0; j < graph.size; j++)
            if (i != j)
                print_shortest_path(graph, i, j);
}

static void export_to_dot(FILE *dot_file, graph_t graph)
{
    fprintf(dot_file, "digraph my_graph {\n"
                      "    layout=sfdp;\n"
                      "    overlap=prism;\n"
                      "    splines=spline;\n"
                      "    sep=\"+25,25\";\n"
                      "    K=2;\n"
                      "    maxiter=2000;\n"

                      "    node [\n"
                      "        shape=circle,\n"
                      "        style=filled,\n"
                      "        fillcolor=lightblue,\n"
                      "        fontname=\"Arial Bold\",\n"
                      "        fontsize=16,\n"
                      "        width=0.9,\n"
                      "        height=0.9,\n"
                      "        margin=0.2\n"
                      "    ];\n"

                      "    edge [\n"
                      "        fontname=\"Arial Bold\",\n"
                      "        fontsize=12,\n"
                      "        penwidth=1.2,\n"
                      "        color=\"#666666\",\n"
                      "        fontcolor=\"#000000\",\n"
                      "        minlen=2\n"
                      "    ];\n");
    for (size_t i = 0; i < graph.size; i++)
        for (size_t j = 0; j < graph.size; j++)
            if (graph.adjacency_matrix[i][j] != 0)
                fprintf(dot_file, "\t %zu -> %zu [label=%d];\n", i, j, graph.adjacency_matrix[i][j]);
    fprintf(dot_file, "}\n");
}

int open_dot_img(const char *file_name, graph_t graph)
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
        return DOT_FILE_OPEN_ERROR;

    export_to_dot(dot_file, graph);
    fclose(dot_file);

    char command[MAX_STR_LEN];
    sprintf(command, "dot -Tpng %s/%s.dot -o %s/%s.png", dot_dir, file_name, img_dir, file_name);
    system(command);
    sprintf(command, "xdg-open %s/%s.png", img_dir, file_name);
    system(command);

    return EXIT_SUCCESS;
}
