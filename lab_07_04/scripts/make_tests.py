import os
from faker import Faker

fake = Faker('en_US')


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

    filename = os.path.join(directory, f"test_{height}.txt")

    with open(filename, 'w') as f:
        for word in words:
            f.write(word + '\n')

    print(f"Файл '{filename}' создан. Слова сохранены.")


def main():
    try:
        num_nodes = [150]
        for num in num_nodes:
            words = generate_words_for_tree(num)
            save_words_to_file(num, words)
    except ValueError as e:
        print(f"Ошибка: {e}")
    except Exception as e:
        print(f"Произошла непредвиденная ошибка: {e}")


if __name__ == "__main__":
    main()
