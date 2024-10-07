#include "theatre.h"
#include "tools.h"

static char *get_str_age_limit(age_limit_t age_limit)
{
    switch (age_limit)
    {
        case AGE_3:
            return "3+";
        case AGE_10:
            return "10+";
        case AGE_16:
            return "16+";
    }
    return "None";
}

static char *get_str_performance_type(performance_type_t performance_type)
{
    switch (performance_type)
    {
        case PLAY:
            return "Play";
        case DRAMA:
            return "Drama";
        case COMEDY:
            return "Comedy";
        case FAIRY_TALE:
            return "Fairy tale";
        case MUSICAL:
            return "Musical";
    }
    return "None";
}

static char *get_str_musical_type(musical_type_t musical_type)
{
    switch (musical_type)
    {
        case BALLET:
            return "Ballet";
        case OPERA:
            return "Opera";
        case MUSICAL_SHOW:
            return "Musical";
    }
    return "None";
}


static int read_performance_type(performance_type_t *performance_type, FILE *file_input)
{
    int tmp;
    if (file_input == stdin)
        printf("Выберите тип спектакля:\n1) Пьеса\n2) Драма \n3) Комедия\n4) Сказка\n5) Музыкальный.\n");
    if (read_int(file_input, &tmp) != EXIT_SUCCESS)
        return WRONG_TYPE_PERFORMANCE_ERROR;

    if (tmp < 1 || tmp > 5)
        return WRONG_TYPE_PERFORMANCE_ERROR;

    *performance_type = tmp;

    return EXIT_SUCCESS;
}

int read_age_limit(age_limit_t *age_limit, FILE *file_input)
{
    int tmp;
    if (file_input == stdin)
        printf("Введите возрастное ограничение на спектакль:\n1) 3+\n2) 10+\n3) 16+\n");
    if (read_int(file_input, &tmp) != EXIT_SUCCESS)
        return WRONG_AGE_LIMIT_ERROR;

    if (tmp < 1 || tmp > 3)
        return WRONG_AGE_LIMIT_ERROR;

    *age_limit = tmp;

    return EXIT_SUCCESS;
}

static int read_musical_type(musical_type_t *musical_type, FILE *file_input)
{
    int tmp;
    if (file_input == stdin)
        printf("Выберите тип музыкального спектакля:\n1) Балет\n2) Опера\n3) Мюзикл\n");
    if (read_int(file_input, &tmp) != EXIT_SUCCESS)
        return WRONG_MUSICAL_TYPE_ERROR;

    if (tmp < 1 || tmp > 3)
        return WRONG_MUSICAL_TYPE_ERROR;

    *musical_type = tmp;

    return EXIT_SUCCESS;
}

static int read_musical(musical_t *musical, FILE *file_input)
{
    if (file_input == stdin)
        printf("Введите имя копозитора (максимальное кол-во символов %d):\n", MAX_STR_LEN);
    if (read_str(file_input, musical->composer, MAX_STR_LEN) != EXIT_SUCCESS)
    {
        while (getchar() != '\n');
        return MUSICAL_READ_ERROR;
    }

    if (file_input == stdin)
        printf("Введите название страны (максимальное кол-во символов %d):\n", MAX_STR_LEN);
    if (read_str(file_input, musical->country, MAX_STR_LEN) != EXIT_SUCCESS)
    {
        while (getchar() != '\n');
        return MUSICAL_READ_ERROR;
    }

    int rc = read_musical_type(&musical->type, file_input);
    if (rc != EXIT_SUCCESS)
        return MUSICAL_READ_ERROR;

    rc = read_age_limit(&musical->age, file_input);
    if (rc != EXIT_SUCCESS)
        return MUSICAL_READ_ERROR;

    if (file_input == stdin)
        printf("Введите продолжительность спектакля в минутах (больше нуля):\n");
    int tmp;
    if (read_int(file_input, &tmp) != EXIT_SUCCESS)
        return MUSICAL_READ_ERROR;
    if (tmp < 0)
        return MUSICAL_READ_ERROR;
    musical->duration = tmp;

    return EXIT_SUCCESS;
}

static int read_performance(theatre_t *theatre, FILE *file_input)
{
    int rc;
    int performance_type = theatre->performance_type;
    switch (performance_type)
    {
        case MUSICAL:
            rc = read_musical(&theatre->performance.musical, file_input);
            break;
        case FAIRY_TALE:
            rc = read_age_limit(&theatre->performance.fairy_tale_age_limit, file_input);
            break;
        default:
            rc = EXIT_SUCCESS;
            break;
    }
    return rc;
}

int fread_theatre(theatre_t *theatre, FILE *file_input)
{
    int rc = EXIT_SUCCESS;
    if (file_input == stdin)
        printf("Введите название театра (не более %d символов):\n", MAX_STR_LEN);
    rc = read_str(file_input, theatre->name, MAX_STR_LEN);
    if (rc != EXIT_SUCCESS)
    {
        if (file_input == stdin)
        {
            while (getchar() != '\n');
            printf("Ошибка ввода названия тетара\n");
            return rc;
        }
        else
            return rc;
    }

    if (file_input == stdin)
        printf("Введите название спектакля (не более %d):\n", MAX_STR_LEN);
    rc = read_str(file_input, theatre->performance_name, MAX_STR_LEN);
    if (rc != EXIT_SUCCESS)
    {
        if (file_input == stdin)
        {
            while (getchar() != '\n');
            printf("Ошибка ввода названия спектакля\n");
            return rc;
        }
        else
            return rc;
    }

    if (file_input == stdin)
        printf("Введите минмальную стоимость билета:\n");
    rc = read_int(file_input, &theatre->price_low);
    if (rc != EXIT_SUCCESS)
    {
        if (file_input == stdin)
        {
            printf("Ошибка ввода цены\n");
            return rc;
        }
        else
            return rc;
    }
    if (theatre->price_low < 0)
    {
        printf("Ошибка ввода цены\n");
        return WRONG_PRICES_ERROR;
    }

    if (file_input == stdin)
        printf("Введите максимальную стоимость билета:\n");

    rc = read_int(file_input, &theatre->price_high);
    if (rc != EXIT_SUCCESS)
    {
        if (file_input == stdin)
        {
            printf("Ошибка ввода цены\n");
            return rc;
        }
        else
            return rc;
    }
    if (theatre->price_high < 0)
    {
        printf("Ошибка ввода цены\n");
        return WRONG_PRICES_ERROR;
    }

    if (theatre->price_low > theatre->price_high)
    {
        printf("Минимальная цена не должна быть больше максимальной.\n");
        return WRONG_PRICES_ERROR;
    }

    rc = read_performance_type(&theatre->performance_type, file_input);
    if (rc != EXIT_SUCCESS)
    {
        if (file_input == stdin)
        {
            printf("Ошибка ввода типа спектакля\n");
            return rc;
        }
        else
            return rc;
    }
    rc = read_performance(theatre, file_input);
    if (rc != EXIT_SUCCESS)
    {
        if (file_input == stdin)
        {
            printf("Ошибка ввода параметров спетакля\n");
            return rc;
        }
        else
            return rc;
    }
    return EXIT_SUCCESS;
}

/*
 * Функции вывода
 */

void fprint_theatre(theatre_t theatre, FILE *file_output)
{
    fprintf(file_output, "| %-30s| %-30s| %-10d| %-10d| %-15s|",
            theatre.name,
            theatre.performance_name,
            theatre.price_low,
            theatre.price_high,
            get_str_performance_type(theatre.performance_type));
    switch (theatre.performance_type)
    {
        case (FAIRY_TALE):
            fprintf(file_output,
                    " %-9s|                               |                     |           |                   |\n",
                    get_str_age_limit(theatre.performance.fairy_tale_age_limit));
            break;
        case (MUSICAL):
            fprintf(file_output, " %-9s| %-30s| %-20s| %-10s| %-18d|\n",
                    get_str_age_limit(theatre.performance.musical.age), theatre.performance.musical.composer,
                    theatre.performance.musical.country, get_str_musical_type(theatre.performance.musical.type),
                    theatre.performance.musical.duration);
            break;
        default:
            fprintf(file_output,
                    "          |                               |                     |           |                   |\n");
            break;
    }
}
