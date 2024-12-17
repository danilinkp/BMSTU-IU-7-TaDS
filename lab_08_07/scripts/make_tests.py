import random
import numpy as np


def generate_graph(num_cities, max_distance, density):
    adjacency_matrix = np.zeros((num_cities, num_cities), dtype=int)

    # Максимальное количество возможных рёбер в неориентированном графе
    max_edges = (num_cities * (num_cities - 1)) // 2
    # Желаемое количество рёбер исходя из плотности
    target_edges = int(max_edges * density)

    # Сначала создаем остовное дерево для гарантии связности
    edges = [(i, i + 1) for i in range(num_cities - 1)]
    current_edges = len(edges)

    # Добавляем случайные рёбра до достижения желаемой плотности
    while current_edges < target_edges:
        i = random.randint(0, num_cities - 1)
        j = random.randint(0, num_cities - 1)
        if i != j and (i, j) not in edges and (j, i) not in edges:
            edges.append((i, j))
            current_edges += 1

    # Заполняем матрицу смежности
    for i, j in edges:
        weight_1 = np.random.randint(5, max_distance)
        weight_2 = np.random.randint(5, max_distance - 10)
        adjacency_matrix[i][j] = weight_1
        adjacency_matrix[j][i] = weight_2

    return adjacency_matrix


def save_graph_to_file(adjacency_matrix, filename):
    with open(filename, 'w') as f:
        len_matrix = len(adjacency_matrix)
        f.write(f"{len_matrix}\n")
        for row in adjacency_matrix:
            f.write(' '.join(map(lambda x: str(int(x)) if x != float('inf') else 'INF', row)) + '\n')


if __name__ == "__main__":
    num_cities = list(range(5, 16))
    max_distance = 50
    for num_city in num_cities:
        if num_city <= 7:
            dencity = 0.7
        elif 7 < num_city <= 12:
            dencity = 0.45
        else:
            dencity = 0.2
        graph = generate_graph(num_city, max_distance, dencity)
        save_graph_to_file(graph, f"../data/graph_{num_city}.txt")
        print(f"Матрица смежности сгенерирована и сохранена в 'graph_{num_city}.txt'.")
