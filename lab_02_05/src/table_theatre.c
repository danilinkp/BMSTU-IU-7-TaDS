#include "table_theatre.h"
#include <string.h>

static int get_theatre_count(FILE *file, size_t *len)
{
    rewind(file);
    theatre_t theatre;
    *len = 0;
    int rc = fread_theatre(&theatre, file);
    while (rc == EXIT_SUCCESS)
    {
        (*len)++;
        rc = fread_theatre(&theatre, file);
    }
    if (!feof(file))
        return FILE_READ_ERROR;
    if (!*len)
        return EMPTY_FILE_ERROR;
    rewind(file);
    return EXIT_SUCCESS;
}

int fread_table(table_theatre_t *table_theatre, theatre_key_t table_key[], size_t *keys_size, FILE *file_input)
{
    size_t len;
    theatre_t theatre;
    int rc = get_theatre_count(file_input, &len);
    if (rc != EXIT_SUCCESS)
        return rc;

    for (size_t i = 0; i < len; i++)
    {
        fread_theatre(&theatre, file_input);
//        printf("%s, %s, %d, %d", theatre.name, theatre.performance_name, theatre.price_low, theatre.price_high);
        table_theatre->theatres[i] = theatre;
        table_theatre->rows++;
        (*keys_size)++;
        table_key[i].price_low = table_theatre->theatres[i].price_low;
        table_key[i].theatre_id = i;
    }
    return EXIT_SUCCESS;
}

void fprint_table(table_theatre_t table_theatre, FILE *file_output)
{
    printf("| %-7s | %-30s| %-30s| %-10s| %-10s| %-15s| %-9s| %-30s| %-20s| %-10s| %-18s|\n", "Индекс",
           "Theatre name", "Performance_name",
           "Min price", "Max price", "Performance", "Age", "Composer", "Country", "Type",
           "Duration");
    for (size_t i = 0; i < table_theatre.rows; i++)
    {
        printf("|--------|-------------------------------|-------------------------------|-----------|-----------|----------------|----------|-------------------------------|---------------------|-----------|-------------------|\n");
        printf("| %-7zu", i + 1);
        fprint_theatre(table_theatre.theatres[i], file_output);
    }
    printf("|--------+-------------------------------+-------------------------------+-----------+-----------+----------------+----------+-------------------------------+---------------------+-----------+-------------------|\n");
}

void fprint_key_table(theatre_key_t table_key[], size_t keys_size, FILE *file_output)
{
    fprintf(file_output, "| %-25s| %-10s|\n", "Original index", "Min price");
    for (size_t i = 0; i < keys_size; i++)
    {
        fprintf(file_output, "|--------------------------|-----------|\n");
        fprintf(file_output, "| %-25zu| %-10d|\n", table_key[i].theatre_id, table_key[i].price_low);
    }
}

void fprint_table_by_keys(table_theatre_t table_theatre, theatre_key_t table_key[], FILE *file_output)
{
    printf("| %-7s | %-30s| %-30s| %-10s| %-10s| %-15s| %-9s| %-30s| %-20s| %-10s| %-18s|\n", "Индекс",
           "Theatre name", "Performance_name",
           "Min price", "Max price", "Performance", "Age", "Composer", "Country", "Type",
           "Duration");
    for (size_t i = 0; i < table_theatre.rows; i++)
    {
        printf("|--------|-------------------------------|-------------------------------|-----------|-----------|----------------|----------|-------------------------------|---------------------|-----------|-------------------|\n");
        printf("| %-7zu", i + 1);
        fprint_theatre(table_theatre.theatres[table_key[i].theatre_id], file_output);
    }
    printf("|--------+-------------------------------+-------------------------------+-----------+-----------+----------------+----------+-------------------------------+---------------------+-----------+-------------------|\n");
}


void update_keys_with_table(table_theatre_t table_theatre, theatre_key_t table_key[], size_t *keys_size)
{
    *keys_size = table_theatre.rows;
    for (size_t i = 0; i < table_theatre.rows; i++)
    {
        table_key[i].theatre_id = i;
        table_key[i].price_low = table_theatre.theatres[i].price_low;
    }
}

int delete_theatre_by_name(table_theatre_t *table_theatre)
{
    char tmp[MAX_STR_LEN + 2];
    printf("Введите название театра, который хотите удалить: ");
    if (read_str(stdin, tmp, MAX_STR_LEN) != EXIT_SUCCESS)
    {
        printf("Ошибка ввода названия театра.\n");
        return NAME_THEATRE_READ_ERROR;
    }
    int is_theatre_in_table = 0;
    size_t index = 0;
    for (size_t i = 0; i < table_theatre->rows; i++)
    {
        if (strcmp(table_theatre->theatres[i].name, tmp) == 0)
        {
            index = i;
            is_theatre_in_table = 1;
            break;
        }
    }
    table_theatre->rows--;
    for (size_t i = index; i < table_theatre->rows; i++)
        table_theatre->theatres[i] = table_theatre->theatres[i + 1];
    if (!is_theatre_in_table)
        return THEATRE_NOT_FOUND_ERROR;
    return EXIT_SUCCESS;
}

int add_theatre(table_theatre_t *table_theatre)
{
    table_theatre->rows++;
    if (table_theatre->rows > MAX_ROW_COUNT)
    {
        table_theatre->rows--;
        return TABLE_OVERFLOW_ERROR;
    }
    theatre_t theatre;
    if (fread_theatre(&theatre, stdin) != EXIT_SUCCESS)
    {
        table_theatre->rows--;
        return THEATRE_READ_ERROR;
    }
    table_theatre->theatres[table_theatre->rows - 1] = theatre;
    return EXIT_SUCCESS;
}

// Поиск балетов для детей указаного возраста, продолжительностью меньше указанной
// Функция только выводит найденные баллеты в случае успеха, иначе код ошибки
int find_ballets(table_theatre_t table_theatre, age_limit_t age_limit, int duration)
{
    printf("| %-7s | %-30s | %-30s | %-10s | %-10s | %-15s | %-9s| %-30s| %-20s| %-10s| %-18s|\n", "Индекс",
           "Название театра", "Название спектакля",
           "Минимальная цена", "Максимальная цена", "Тип спектакля", "Возраст", "Композитор", "Страна", "Вид",
           "Продолжительность");
    int is_ballet_in_table = 0;
    for (size_t i = 0; i < table_theatre.rows; i++)
    {
        if (table_theatre.theatres[i].performance.musical.age == age_limit &&
            table_theatre.theatres[i].performance.musical.duration < duration)
        {
            is_ballet_in_table = 1;
            printf("|--------|------------------|--------------------|------------------|-------------------|---------------|--------|---------------------|---------------|--------|------------------|\n");
            printf("| %-7zu", i + 1);
            fprint_theatre(table_theatre.theatres[i], stdout);
        }
    }

    if (!is_ballet_in_table)
        return BALLET_NOT_FOUND_ERROR;
    return EXIT_SUCCESS;
}


// Сортировка вставками
void insertion_sort(void *base, size_t nmemb, size_t size, compar_t cmp)
{
    char *pb = base;
    char *pe = pb + nmemb * size;
    for (char *pi = pb + size; pi < pe; pi += size)
        for (char *pk = pi; pk > pb && cmp((pk - size), pk) > 0; pk -= size)
        {
            char buf[size];
            memcpy(buf, (pk - size), size);
            memcpy((pk - size), pk, size);
            memcpy(pk, buf, size);
        }
}


double time_sort_table(table_theatre_t table_theatre, size_t count, sort_t sort)
{
    unsigned long long beg, end;
    double sum = 0.0;
    for (size_t i = 0; i < count; i++)
    {
        table_theatre_t tmp_table = table_theatre;
        beg = microseconds_now();
        sort(&tmp_table.theatres, tmp_table.rows, sizeof(theatre_t), theatre_t_comp);
        end = microseconds_now();
        sum += end - beg;
    }
    return sum / count;
}

static void arrcpy(const void *src, void *dst, size_t nmemb, size_t size)
{
    const char *source = src;
    char *dest = dst;

    for (const char *i = src; i < source + nmemb * size; i++, dest++)
        *dest = *i;
}


double time_sort_keys(theatre_key_t table_keys[], size_t table_size, size_t count, sort_t sort)
{
    unsigned long long beg, end;
    double sum = 0.0;
    theatre_key_t tmp_keys[MAX_ROW_COUNT];
    for (size_t i = 0; i < count; i++)
    {
        arrcpy(table_keys, tmp_keys, table_size, sizeof(theatre_key_t));
        beg = microseconds_now();
        sort(&tmp_keys, table_size, sizeof(theatre_key_t), theatre_t_comp);
        end = microseconds_now();
        sum += end - beg;
    }
    return sum / count;
}
