#ifndef LAB_03_03_MENU_H
#define LAB_03_03_MENU_H

typedef enum
{
    EXIT,
    READ_MATRIX,
    READ_VECTOR,
    PRINT_MATRIX,
    PRINT_VECTOR,
    MULTIPLICATION,
    COMPARING
} menu_t;

enum menu_errors
{
    WRONG_CHOICE_ERROR = 11,
};

void menu_print(void);

#endif //LAB_03_03_MENU_H
