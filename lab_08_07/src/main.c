#include "graph.h"
#include "input_tools.h"
#include <stdio.h>

int main(void)
{
    graph_t graph;
    char file_name[MAX_STR_LEN + 1];
    char *dot_file_name = "graph";

    printf("Введите название файла: ");
    int rc = read_str(stdin, file_name, MAX_STR_LEN);
    if (rc != EXIT_SUCCESS)
    {
        printf("Ошибка ввода имени файла.\n");
        return FILE_OPEN_ERROR;
    }
    rc = fread_graph(file_name, &graph);
    if (rc != EXIT_SUCCESS)
    {
        printf("Ошибка при чтении файла. Файл не найден или он записан неверно.\n");
        return rc;
    }

    open_dot_img(dot_file_name, graph);

    find_shortest_paths(&graph);
    print_shortest_paths(graph);

    graph_free(&graph);

    return EXIT_SUCCESS;
}
