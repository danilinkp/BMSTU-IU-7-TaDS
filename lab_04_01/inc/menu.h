#ifndef LAB_04_01_MENU_H
#define LAB_04_01_MENU_H

typedef enum
{
    EXIT,
    ARR_STACK_PUSH,
    LIST_STACK_PUSH,
    ARR_STACK_POP,
    LIST_STACK_POP,
    PRINT_ARR_STACK,
    PRINT_LIST_STACK,
    PRINT_FREE_LIST,
    PRINT_SEQUENCE_ON_ARR,
    PRINT_SEQUENCE_ON_LIST,
    PRINT_MEASUREMENTS
} menu_t;

enum menu_errors
{
    WRONG_VALUE_READ_ERROR = 11
};

void menu_print(void);

#endif //LAB_04_01_MENU_H
