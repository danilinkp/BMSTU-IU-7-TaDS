#ifndef LAB_02_05_THEATRE_H
#define LAB_02_05_THEATRE_H

#include <stdio.h>
#include <stdlib.h>

#define MAX_STR_LEN 30

// Возрастные ограничения
typedef enum
{
    AGE_3 = 1,
    AGE_10,
    AGE_16
} age_limit_t;

// Тип спекатакля
typedef enum
{
    PLAY = 1,
    DRAMA,
    COMEDY,
    FAIRY_TALE,
    MUSICAL
} performance_type_t;

// Тип музыыкального спекатакля
typedef enum
{
    BALLET = 1,
    OPERA,
    MUSICAL_SHOW
} musical_type_t;

// Структура музыкального спектакля
typedef struct
{
    char composer[MAX_STR_LEN + 1];
    char country[MAX_STR_LEN + 1];
    musical_type_t type;
    age_limit_t age;
    int duration;
} musical_t;

// Объединение для спектакля
typedef union
{
    age_limit_t fairy_tale_age_limit; // Для сказки
    musical_t musical; // Для музыкального
} performance_union;

typedef struct
{
    char name[MAX_STR_LEN + 1];
    char performance_name[MAX_STR_LEN + 1];
    int price_high;
    int price_low;
    performance_union performance;
    performance_type_t performance_type;
} theatre_t;

enum errors_theatre
{
    WRONG_PRICES_ERROR = 1,
    WRONG_TYPE_PERFORMANCE_ERROR,
    WRONG_AGE_LIMIT_ERROR,
    WRONG_MUSICAL_TYPE_ERROR,
    MUSICAL_READ_ERROR,
};

// Функция для чтения одного театра
int fread_theatre(theatre_t *theatre, FILE *file_input);

// Функция для вывода тетара
void fprint_theatre(theatre_t theatre, FILE *file_output);

int read_age_limit(age_limit_t *age_limit, FILE *file_input);

#endif //LAB_02_05_THEATRE_H
