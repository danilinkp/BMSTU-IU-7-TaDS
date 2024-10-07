from faker import Faker
import random
import os

# Константа
MAX_STR_LEN = 25

# Определение перечислений
AGE_LIMITS = {
    'AGE_3': 1,
    'AGE_10': 2,
    'AGE_16': 3
}

PERFORMANCE_TYPES = {
    'PLAY': 1,
    'DRAMA': 2,
    'COMEDY': 3,
    'FAIRY_TALE': 4,
    'MUSICAL': 5
}

MUSICAL_TYPES = {
    'BALLET': 1,
    'OPERA': 2,
    'MUSICAL_SHOW': 3
}


def generate_structs(count: int) -> str:
    fake = Faker()

    structs = ""
    for _ in range(count):
        name = fake.company()[:MAX_STR_LEN]
        performance_name = fake.catch_phrase()[:MAX_STR_LEN]
        price_high = fake.random_int(min=1000, max=5000)
        price_low = fake.random_int(min=150, max=300)
        performance_type_key = random.choice(list(PERFORMANCE_TYPES.keys()))
        performance_type_value = PERFORMANCE_TYPES[performance_type_key]

        if performance_type_key == 'FAIRY_TALE':
            age_limit_key = random.choice(list(AGE_LIMITS.keys()))
            age_limit_value = AGE_LIMITS[age_limit_key]
            performance_details = f"{age_limit_value}\n"
        elif performance_type_key == "MUSICAL":
            musical_type_key = random.choice(list(MUSICAL_TYPES.keys()))
            musical_type_value = MUSICAL_TYPES[musical_type_key]

            age_limit_key = random.choice(list(AGE_LIMITS.keys()))
            age_limit_value = AGE_LIMITS[age_limit_key]

            duration = fake.random_int(min=30, max=180)
            composer = fake.name()[:MAX_STR_LEN]
            country = fake.country()[:20]

            performance_details = f"{composer}\n{country}\n{age_limit_value}\n{musical_type_value}\n{duration}\n"
        else:
            performance_details = ""

        structs += f"{name}\n{performance_name}\n{price_low}\n{price_high}\n{performance_type_value}\n{performance_details}"

    return structs


def main():
    try:
        count = int(input("Введите количество структур, которое необходимо создать: "))
    except ValueError:
        print("Пожалуйста, введите целое число.")
        return

    struct_content = generate_structs(count)

    # Создание директории data, если она не существует
    os.makedirs('../data', exist_ok=True)

    filename = f"../data/test_{count}.txt"

    with open(filename, 'w', encoding='utf-8') as file:
        file.write(struct_content)

    print(f"Файл '{filename}' с {count} структурами успешно создан.")


if __name__ == "__main__":
    main()
