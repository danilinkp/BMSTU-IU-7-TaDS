#ifndef LAB_06_03_MENU_H
#define LAB_06_03_MENU_H


typedef enum
{
    EXIT,
    BST_READ,
    BST_INSERT,
    BST_REMOVE,
    BST_SEARCH,
    BST_REMOVE_BY_LETTER,
    BST_PRINT,
    PRINT_MEASUREMENTS
} menu_t;

enum menu_errors
{
    WRONG_VALUE_READ_ERROR = 9
};

void menu_print(void);

#endif //LAB_06_03_MENU_H
