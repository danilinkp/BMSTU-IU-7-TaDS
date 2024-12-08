import os
import random
from faker import Faker

fake = Faker('en_US')

def create_balanced_order(words):
    if not words:
        return []
    mid = len(words) // 2
    return [words[mid]] + create_balanced_order(words[:mid]) + create_balanced_order(words[mid + 1:])


def generate_words_for_tree(num_nodes):
    if num_nodes < 1:
        raise ValueError("Высота дерева должна быть больше 0.")

    words = set()
    while len(words) < num_nodes:
        words.add(fake.word().lower())
    return list(words)


def save_words_to_file(height, words):
    directory = "../data"
    if not os.path.exists(directory):
        os.makedirs(directory)

    filename = os.path.join(directory, f"words_test_{height}.txt")

    with open(filename, 'w') as f:
        for word in words:
            f.write(word + '\n')

    print(f"Файл '{filename}' создан. Слова сохранены.")


def main():
    try:
        num_nodes = int(input("Введите кол-во вершин: "))

        words = generate_words_for_tree(num_nodes)
        sorted_words = sorted(words)
        balanced_words = create_balanced_order(sorted_words)
        save_words_to_file(num_nodes, balanced_words)
    except ValueError as e:
        print(f"Ошибка: {e}")
    except Exception as e:
        print(f"Произошла непредвиденная ошибка: {e}")


if __name__ == "__main__":
    main()
