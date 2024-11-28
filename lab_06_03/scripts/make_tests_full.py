import os
import random
import nltk
from nltk.corpus import words

# Убедимся, что словарь загружен
nltk.download('words')
word_list = words.words()


def create_balanced_order(words):
    """
    Рекурсивно создает сбалансированное дерево из списка слов.
    Слова должны быть отсортированы.
    """
    if not words:
        return []
    mid = len(words) // 2
    return [words[mid]] + create_balanced_order(words[:mid]) + create_balanced_order(words[mid + 1:])


def generate_words_for_tree(num_nodes):
    """
    Генерирует список осмысленных слов для построения полного бинарного дерева заданной высоты.
    Количество слов соответствует числу узлов в полном бинарном дереве.
    """
    if num_nodes < 1:
        raise ValueError("Высота дерева должна быть больше 0.")

    if num_nodes > len(word_list):
        raise ValueError("В словаре недостаточно слов для создания дерева этой высоты.")
    return random.sample(word_list, num_nodes)


def save_words_to_file(height, words):
    """
    Сохраняет список слов в текстовый файл в папке data.
    Название файла формируется автоматически: test_{height}.txt.
    """
    # Создаем папку data, если её нет
    directory = "../data"  # Папка будет создаваться в текущей директории
    if not os.path.exists(directory):
        os.makedirs(directory)

    # Формируем имя файла
    filename = os.path.join(directory, f"b_test_{height}.txt")

    # Сохраняем слова в файл
    with open(filename, 'w') as f:
        for word in words:
            f.write(word + '\n')

    print(f"Файл '{filename}' создан. Слова сохранены.")


def main():
    try:
        num_nodes = [10, 20, 50, 75, 100]
        for num_node in num_nodes:
            words = generate_words_for_tree(num_node)
            sorted_words = sorted(words)  # Сортируем слова для сбалансированного дерева
            balanced_words = create_balanced_order(sorted_words)  # Создаем сбалансированный порядок
            save_words_to_file(num_node, balanced_words)
    except ValueError as e:
        print(f"Ошибка: {e}")
    except Exception as e:
        print(f"Произошла непредвиденная ошибка: {e}")


if __name__ == "__main__":
    main()
