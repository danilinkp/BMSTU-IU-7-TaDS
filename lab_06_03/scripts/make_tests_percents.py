import os
import random
import nltk
from nltk.corpus import words

nltk.download('words')
word_list = words.words()


def create_balanced_order(words):
    if not words:
        return []
    mid = len(words) // 2
    return [words[mid]] + create_balanced_order(words[:mid]) + create_balanced_order(words[mid + 1:])


def generate_words_for_tree(num_nodes):
    if num_nodes < 1:
        raise ValueError("Высота дерева должна быть больше 0.")

    if num_nodes > len(word_list):
        raise ValueError("В словаре недостаточно слов для создания дерева этой высоты.")
    return random.sample(word_list, num_nodes)


def save_words_to_file(height, words):
    directory = "../data"
    if not os.path.exists(directory):
        os.makedirs(directory)

    filename = os.path.join(directory, f"nb_test_{height}.txt")

    with open(filename, 'w') as f:
        for word in words:
            f.write(word + '\n')

    print(f"Файл '{filename}' создан. Слова сохранены.")


def main():
    try:
        num_nodes = [10, 20, 50, 75, 100]
        for num_node in num_nodes:
            words = generate_words_for_tree(num_node)
            sorted_words = sorted(words)[::-1]
            save_words_to_file(num_node, sorted_words)
    except ValueError as e:
        print(f"Ошибка: {e}")
    except Exception as e:
        print(f"Произошла непредвиденная ошибка: {e}")


if __name__ == "__main__":
    main()
